/**
 * @file note_table.h
 * @brief Musical note frequency lookup table
 * @author joaomrpimentel
 * @version 1.1
 */

#ifndef NOTE_TABLE_H
#define NOTE_TABLE_H

/**
 * @brief Converts scientific pitch notation to frequency
 * @param note_name String in format "C4", "F#3", "Bb5", etc.
 * @return Frequency in Hz using equal temperament (A4=440Hz), or 0.0 if unknown
 * 
 * Supports:
 * - Full MIDI range: C-1 (8.18 Hz) to G9 (12543.85 Hz)
 * - Sharp notation: C#, D#, F#, G#, A#
 * - Flat notation: Db, Eb, Gb, Ab, Bb
 * - Any octave from -1 to 9
 * 
 * Examples:
 * - "C4" → 261.63 Hz (middle C)
 * - "A4" → 440.00 Hz (concert pitch)
 * - "C#5" → 554.37 Hz
 * - "Bb3" → 233.08 Hz
 * - "C-1" → 8.18 Hz (lowest MIDI note)
 * - "G9" → 12543.85 Hz (highest MIDI note)
 */
float get_note_freq(const char* note_name);

#endif /* NOTE_TABLE_H */