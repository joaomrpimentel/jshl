/**
 * @file main.c
 * @brief JSHL Compiler - Main program entry point
 * @author joaomrpimentel
 * @version 1.0
 * 
 * Orchestrates the compilation pipeline: file I/O, parsing, synthesis, and export.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "jshl_compiler.h"
#include "note_list.h"
#include "parser.h"
#include "synth.h"
#include "wav_writer.h"

/**
 * @brief Main program execution
 * @param argc Argument count
 * @param argv Argument vector [program, input.jshl, output.wav]
 * @return 0 on success, 1 on error
 * 
 * Usage: jshl <input.jshl> [output.wav]
 * 
 * Pipeline:
 * 1. Load JSHL source file
 * 2. Parse into note event list
 * 3. Render to audio buffer
 * 4. Export as WAV file
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.jshl> [output.wav]\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = (argc > 2) ? argv[2] : "output.wav";

    FILE* fp = fopen(input_file, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot read file '%s'\n", input_file);
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* code_buffer = (char*)malloc(file_size + 1);
    if (!code_buffer) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(fp);
        return 1;
    }
    fread(code_buffer, 1, file_size, fp);
    code_buffer[file_size] = '\0';
    fclose(fp);

    NoteList note_list;
    note_list_init(&note_list);
    parse_jshl(code_buffer, &note_list);
    
    long total_samples = 0;
    float* audio_buffer = render_audio(&note_list, &total_samples);

    if (audio_buffer) {
        write_wav_file(output_file, audio_buffer, total_samples);
        printf("Compiled: %zu notes, %.2fs → %s\n", 
               note_list.size, 
               (float)total_samples / SAMPLE_RATE, 
               output_file);
        free(audio_buffer);
    } else {
        fprintf(stderr, "Error: No notes to render\n");
    }

    free(code_buffer);
    note_list_free(&note_list);

    return 0;
}