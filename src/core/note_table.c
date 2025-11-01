/**
 * @file note_table.c
 * @brief Implementation of musical note frequency lookup
 * @author joaomrpimentel
 * @version 1.1
 */

#include <string.h>
#include <ctype.h>
#include <math.h>
#include "note_table.h"

/**
 * @brief Converts note name to MIDI note number
 * @param note_name Scientific pitch notation (e.g., "C4", "F#3")
 * @return MIDI note number (0-127), or -1 if invalid
 * 
 * MIDI note numbering:
 * - C-1 = 0, C0 = 12, C4 (middle C) = 60, A4 = 69, G9 = 127
 * - Each octave spans 12 semitones
 */
static int note_to_midi(const char* note_name) {
    if (!note_name || strlen(note_name) < 2) {
        return -1;
    }
    
    // Parse note letter (C, D, E, F, G, A, B)
    char note_letter = toupper(note_name[0]);
    int note_offset;
    
    switch (note_letter) {
        case 'C': note_offset = 0; break;
        case 'D': note_offset = 2; break;
        case 'E': note_offset = 4; break;
        case 'F': note_offset = 5; break;
        case 'G': note_offset = 7; break;
        case 'A': note_offset = 9; break;
        case 'B': note_offset = 11; break;
        default: return -1;
    }
    
    // Parse accidental (# or b)
    int accidental = 0;
    int octave_pos = 1;
    
    if (note_name[1] == '#') {
        accidental = 1;
        octave_pos = 2;
    } else if (note_name[1] == 'b') {
        accidental = -1;
        octave_pos = 2;
    }
    
    // Parse octave number (can be negative or two digits)
    if (!isdigit(note_name[octave_pos]) && note_name[octave_pos] != '-') {
        return -1;
    }
    
    int octave;
    if (note_name[octave_pos] == '-') {
        // Negative octave (e.g., "C-1")
        if (!isdigit(note_name[octave_pos + 1])) {
            return -1;
        }
        octave = -(note_name[octave_pos + 1] - '0');
    } else {
        octave = note_name[octave_pos] - '0';
        
        // Handle two-digit octave numbers (e.g., "C10")
        if (note_name[octave_pos + 1] && isdigit(note_name[octave_pos + 1])) {
            octave = octave * 10 + (note_name[octave_pos + 1] - '0');
        }
    }
    
    // Calculate MIDI note number
    // MIDI: C-1 = 0, C0 = 12, C4 = 60
    int midi_note = (octave + 1) * 12 + note_offset + accidental;
    
    // Validate range (MIDI: 0-127)
    if (midi_note < 0 || midi_note > 127) {
        return -1;
    }
    
    return midi_note;
}

/**
 * @brief Converts MIDI note number to frequency
 * @param midi_note MIDI note number (0-127)
 * @return Frequency in Hz using equal temperament
 * 
 * Formula: f = 440 * 2^((n - 69) / 12)
 * Where n is MIDI note number, 69 = A4 = 440 Hz
 */
static float midi_to_freq(int midi_note) {
    // A4 (MIDI note 69) = 440 Hz
    return 440.0f * powf(2.0f, (midi_note - 69) / 12.0f);
}

float get_note_freq(const char* note_name) {
    int midi_note = note_to_midi(note_name);
    
    if (midi_note < 0) {
        return 0.0f;
    }
    
    return midi_to_freq(midi_note);
}