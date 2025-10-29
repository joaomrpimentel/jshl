/**
 * @file raw_writer.h
 * @brief Raw PCM file export interface
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef RAW_WRITER_H
#define RAW_WRITER_H

/**
 * @brief Exports audio buffer to raw PCM file
 * @param filename Output file path
 * @param buffer Float PCM audio data in range [-1.0, 1.0]
 * @param sample_count Number of samples in buffer
 * 
 * Format specification:
 * - Format: Raw PCM (no header)
 * - Bit depth: 32-bit float (IEEE 754)
 * - Byte order: Little-endian
 * - Channels: 1 (mono)
 * 
 * @note Output can be imported into audio editors as raw data
 */
void write_raw_file(const char* filename, float* buffer, long sample_count);

#endif /* RAW_WRITER_H */