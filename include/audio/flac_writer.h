/**
 * @file flac_writer.h
 * @brief FLAC file export interface using libFLAC
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef FLAC_WRITER_H
#define FLAC_WRITER_H

/**
 * @brief Exports audio buffer to FLAC file
 * @param filename Output file path
 * @param buffer Float PCM audio data in range [-1.0, 1.0]
 * @param sample_count Number of samples in buffer
 * @param sample_rate Sample rate in Hz
 * @return 0 on success, -1 on error
 * 
 * Format specification:
 * - Codec: FLAC (Free Lossless Audio Codec)
 * - Bit depth: 24-bit (converted from float)
 * - Compression level: 8 (maximum compression)
 * - Channels: 1 (mono)
 * - Requires: libFLAC
 * 
 * @note Lossless compression - no quality loss from original audio
 */
int write_flac_file(const char* filename, float* buffer, long sample_count, int sample_rate);

#endif /* FLAC_WRITER_H */