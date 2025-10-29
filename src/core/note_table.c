/**
 * @file note_table.c
 * @brief Implementation of musical note frequency lookup
 * @author joaomrpimentel
 * @version 1.0
 */

#include <string.h>
#include "note_table.h"

float get_note_freq(const char* note_name) {
    if (strcmp(note_name, "C3") == 0) return 130.81f;
    if (strcmp(note_name, "C#3") == 0 || strcmp(note_name, "Db3") == 0) return 138.59f;
    if (strcmp(note_name, "D3") == 0) return 146.83f;
    if (strcmp(note_name, "D#3") == 0 || strcmp(note_name, "Eb3") == 0) return 155.56f;
    if (strcmp(note_name, "E3") == 0) return 164.81f;
    if (strcmp(note_name, "F3") == 0) return 174.61f;
    if (strcmp(note_name, "F#3") == 0 || strcmp(note_name, "Gb3") == 0) return 185.00f;
    if (strcmp(note_name, "G3") == 0) return 196.00f;
    if (strcmp(note_name, "G#3") == 0 || strcmp(note_name, "Ab3") == 0) return 207.65f;
    if (strcmp(note_name, "A3") == 0) return 220.00f;
    if (strcmp(note_name, "A#3") == 0 || strcmp(note_name, "Bb3") == 0) return 233.08f;
    if (strcmp(note_name, "B3") == 0) return 246.94f;
    
    if (strcmp(note_name, "C4") == 0) return 261.63f;
    if (strcmp(note_name, "C#4") == 0 || strcmp(note_name, "Db4") == 0) return 277.18f;
    if (strcmp(note_name, "D4") == 0) return 293.66f;
    if (strcmp(note_name, "D#4") == 0 || strcmp(note_name, "Eb4") == 0) return 311.13f;
    if (strcmp(note_name, "E4") == 0) return 329.63f;
    if (strcmp(note_name, "F4") == 0) return 349.23f;
    if (strcmp(note_name, "F#4") == 0 || strcmp(note_name, "Gb4") == 0) return 369.99f;
    if (strcmp(note_name, "G4") == 0) return 392.00f;
    if (strcmp(note_name, "G#4") == 0 || strcmp(note_name, "Ab4") == 0) return 415.30f;
    if (strcmp(note_name, "A4") == 0) return 440.00f;
    if (strcmp(note_name, "A#4") == 0 || strcmp(note_name, "Bb4") == 0) return 466.16f;
    if (strcmp(note_name, "B4") == 0) return 493.88f;
    
    if (strcmp(note_name, "C5") == 0) return 523.25f;
    if (strcmp(note_name, "C#5") == 0 || strcmp(note_name, "Db5") == 0) return 554.37f;
    if (strcmp(note_name, "D5") == 0) return 587.33f;
    if (strcmp(note_name, "D#5") == 0 || strcmp(note_name, "Eb5") == 0) return 622.25f;
    if (strcmp(note_name, "E5") == 0) return 659.25f;
    if (strcmp(note_name, "F5") == 0) return 698.46f;
    if (strcmp(note_name, "F#5") == 0 || strcmp(note_name, "Gb5") == 0) return 739.99f;
    if (strcmp(note_name, "G5") == 0) return 783.99f;
    if (strcmp(note_name, "G#5") == 0 || strcmp(note_name, "Ab5") == 0) return 830.61f;
    if (strcmp(note_name, "A5") == 0) return 880.00f;
    if (strcmp(note_name, "A#5") == 0 || strcmp(note_name, "Bb5") == 0) return 932.33f;
    if (strcmp(note_name, "B5") == 0) return 987.77f;
    
    return 0.0f;
}