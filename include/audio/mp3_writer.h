/**
 * @file mp3_writer.h
 * @brief MP3 file export interface using LAME encoder
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef MP3_WRITER_H
#define MP3_WRITER_H

/**
 * @brief Exports audio buffer to MP3 file
 * @param filename Output file path
 * @param buffer Float PCM audio data in range [-1.0, 1.0]
 * @param sample_count Number of samples in buffer
 * @param sample_rate Sample rate in Hz
 * @return 0 on success, -1 on error
 * 
 * Format specification:
 * - Codec: MP3 (MPEG-1 Layer 3)
 * - Bitrate: 320 kbps CBR (high quality)
 * - Channels: 1 (mono)
 * - Requires: libmp3lame
 * 
 * @note Requires LAME MP3 encoder library installed
 */
int write_mp3_file(const char* filename, float* buffer, long sample_count, int sample_rate);

#endif /* MP3_WRITER_H */