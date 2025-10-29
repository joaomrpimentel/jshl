/**
 * @file wav_writer.h
 * @brief WAV file export interface
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef WAV_WRITER_H
#define WAV_WRITER_H

/**
 * @brief Exports audio buffer to WAV file
 * @param filename Output file path
 * @param buffer Float PCM audio data in range [-1.0, 1.0]
 * @param sample_count Number of samples in buffer
 * 
 * WAV format specification:
 * - Format: IEEE Float (format code 3)
 * - Bit depth: 32-bit
 * - Channels: 1 (mono)
 * - Sample rate: Defined by SAMPLE_RATE constant
 * 
 * @note Standard RIFF/WAVE structure with fmt and data chunks
 */
void write_wav_file(const char* filename, float* buffer, long sample_count);

#endif /* WAV_WRITER_H */