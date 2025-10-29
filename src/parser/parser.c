/**
 * @file parser.c
 * @brief JSHL language parser implementation
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "note_table.h"

/**
 * @brief Recursive parser for JSHL language constructs
 * @param lines Array of source code lines
 * @param line_count Total number of lines in array
 * @param current_line Pointer to current line index (modified during parsing)
 * @param start_time Starting time offset in seconds
 * @param state Current synthesizer state (modified by commands)
 * @param list Note list to populate with parsed events
 * @return Final time position after parsing block
 * 
 * Handles JSHL commands:
 * - WAVE <type>: Sets waveform (SINE, SQUARE, SAWTOOTH, TRIANGLE)
 * - ENVELOPE <A> <D> <S> <R>: Configures ADSR envelope
 * - SLIDE <duration>: Sets portamento time
 * - PAUSE <duration>: Adds silence
 * - LOOP <count> { ... }: Repeats enclosed block
 * - <note> <duration>: Adds note event
 * 
 * @note Uses local buffer to preserve line data across loop iterations
 */
static float parse_jshl_recursive(char** lines, int line_count, int* current_line, 
                                  float start_time, SynthState* state, NoteList* list) {
    
    float current_time = start_time;
    int i = *current_line;

    while (i < line_count) {
        *current_line = i;
        
        char line_buffer[256];
        strncpy(line_buffer, lines[i], sizeof(line_buffer) - 1);
        line_buffer[sizeof(line_buffer) - 1] = '\0';
        char* line = line_buffer;
        
        i++;

        while (*line == ' ' || *line == '\t') line++;
        line[strcspn(line, "\r\n")] = 0;
        
        if (line[0] == '\0' || line[0] == '#') continue;

        char* saveptr;
        char* command = strtok_r(line, " \t", &saveptr);
        if (!command) continue;

        if (strcmp(command, "WAVE") == 0) {
            char* type = strtok_r(NULL, " \t", &saveptr);
            if (!type) continue;
            if (strcmp(type, "SINE") == 0) state->wave = WAVE_SINE;
            else if (strcmp(type, "SQUARE") == 0) state->wave = WAVE_SQUARE;
            else if (strcmp(type, "SAWTOOTH") == 0) state->wave = WAVE_SAWTOOTH;
            else if (strcmp(type, "TRIANGLE") == 0) state->wave = WAVE_TRIANGLE;
        } 
        else if (strcmp(command, "ENVELOPE") == 0) {
            char* tok;
            tok = strtok_r(NULL, " \t", &saveptr);
            state->envelope.attack = tok ? atof(tok) : 0.01f;
            tok = strtok_r(NULL, " \t", &saveptr);
            state->envelope.decay = tok ? atof(tok) : 0.0f;
            tok = strtok_r(NULL, " \t", &saveptr);
            state->envelope.sustain = tok ? atof(tok) : 1.0f;
            tok = strtok_r(NULL, " \t", &saveptr);
            state->envelope.release = tok ? atof(tok) : 0.01f;
        }
        else if (strcmp(command, "SLIDE") == 0) {
            char* tok = strtok_r(NULL, " \t", &saveptr);
            state->slide = tok ? atof(tok) : 0.0f;
        }
        else if (strcmp(command, "PAUSE") == 0) {
            char* tok = strtok_r(NULL, " \t", &saveptr);
            float duration = tok ? atof(tok) : 0.0f;
            if (duration > 0) current_time += duration;
            state->last_freq = 0.0f;
        }
        else if (strcmp(command, "LOOP") == 0) {
            char* tok = strtok_r(NULL, " \t", &saveptr);
            int loop_count = tok ? atoi(tok) : 1;
            
            int loop_start_line = i;
            int loop_end_line = -1;
            int j;
            for (j = i; j < line_count; j++) {
                if (strstr(lines[j], "}")) {
                    loop_end_line = j;
                    break;
                }
            }

            if (loop_end_line != -1) {
                for (int k = 0; k < loop_count; k++) {
                    int temp_line = loop_start_line;
                    current_time = parse_jshl_recursive(lines, loop_end_line, &temp_line,
                                                        current_time, state, list);
                }
                i = loop_end_line + 1;
            } else {
                fprintf(stderr, "Syntax error: Unclosed LOOP at line %d\n", i);
            }
        }
        else if (strcmp(command, "}") == 0) {
            return current_time;
        }
        else {
            float freq = get_note_freq(command);
            char* tok = strtok_r(NULL, " \t", &saveptr);
            float duration = tok ? atof(tok) : 0.0f;

            if (freq > 0 && duration > 0) {
                NoteEvent note;
                note.freq = freq;
                note.duration = duration;
                note.start_time = current_time;
                note.state = *state;

                note_list_add(list, note);

                state->last_freq = freq;
                current_time += duration;
            } else if (freq == 0.0f && duration > 0.0f) {
                fprintf(stderr, "Warning: Unknown note '%s' at line %d\n", command, i);
            }
        }
    }
    *current_line = i;
    return current_time;
}

void parse_jshl(char* code, NoteList* list) {
    char* code_copy = strdup(code);
    if (!code_copy) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    int line_count = 1;
    char* p = code_copy;
    while ((p = strchr(p, '\n'))) {
        line_count++;
        p++;
    }

    char** lines = (char**)malloc(line_count * sizeof(char*));
    if (!lines) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(code_copy);
        exit(1);
    }
    
    int i = 0;
    p = code_copy;
    char* line_start = p;
    while (*p) {
        if (*p == '\n') {
            *p = '\0';
            lines[i++] = line_start;
            line_start = p + 1;
        }
        p++;
    }
    lines[i] = line_start;

    SynthState state;
    state.wave = WAVE_SQUARE;
    state.envelope = (Envelope){ 0.01f, 0.0f, 1.0f, 0.01f };
    state.slide = 0.0f;
    state.last_freq = 0.0f;

    int current_line = 0;
    parse_jshl_recursive(lines, line_count, &current_line, 0.0f, &state, list);
    
    free(code_copy);
    free(lines);
}