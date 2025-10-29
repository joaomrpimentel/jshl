/**
 * @file note_table.h
 * @brief Musical note frequency lookup table
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef NOTE_TABLE_H
#define NOTE_TABLE_H

/**
 * @brief Converts scientific pitch notation to frequency
 * @param note_name String in format "C4", "F#3", "Bb5", etc.
 * @return Frequency in Hz using equal temperament (A4=440Hz), or 0.0 if unknown
 * 
 * Supports chromatic scale from C3 (130.81 Hz) to B5 (987.77 Hz).
 * Accepts both sharp (#) and flat (b) notation for accidentals.
 */
float get_note_freq(const char* note_name);

#endif /* NOTE_TABLE_H */