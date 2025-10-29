/**
 * @file mp3_writer.c
 * @brief MP3 file export implementation using LAME
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <lame/lame.h>
#include "mp3_writer.h"

int write_mp3_file(const char* filename, float* buffer, long sample_count, int sample_rate) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot write to file '%s'\n", filename);
        return -1;
    }
    
    // Initialize LAME encoder
    lame_t lame = lame_init();
    if (!lame) {
        fprintf(stderr, "Error: Failed to initialize LAME encoder\n");
        fclose(fp);
        return -1;
    }
    
    // Configure encoder
    lame_set_in_samplerate(lame, sample_rate);
    lame_set_num_channels(lame, 1);  // Mono
    lame_set_mode(lame, MONO);
    lame_set_brate(lame, 320);       // 320 kbps CBR
    lame_set_quality(lame, 2);       // High quality (0=best, 9=worst)
    
    if (lame_init_params(lame) < 0) {
        fprintf(stderr, "Error: Failed to set LAME parameters\n");
        lame_close(lame);
        fclose(fp);
        return -1;
    }
    
    // Allocate MP3 output buffer (conservative size estimate)
    size_t mp3_buffer_size = sample_count * 5 / 4 + 7200;
    unsigned char* mp3_buffer = (unsigned char*)malloc(mp3_buffer_size);
    if (!mp3_buffer) {
        fprintf(stderr, "Error: Failed to allocate MP3 buffer\n");
        lame_close(lame);
        fclose(fp);
        return -1;
    }
    
    // Encode audio in chunks
    const int chunk_size = 8192;
    long samples_processed = 0;
    size_t total_mp3_bytes = 0;
    
    while (samples_processed < sample_count) {
        int samples_to_encode = (sample_count - samples_processed < chunk_size) 
                               ? (sample_count - samples_processed) 
                               : chunk_size;
        
        int mp3_bytes = lame_encode_buffer_ieee_float(
            lame,
            buffer + samples_processed,  // Left channel (mono)
            NULL,                         // Right channel (NULL for mono)
            samples_to_encode,
            mp3_buffer + total_mp3_bytes,
            mp3_buffer_size - total_mp3_bytes
        );
        
        if (mp3_bytes < 0) {
            fprintf(stderr, "Error: MP3 encoding failed\n");
            free(mp3_buffer);
            lame_close(lame);
            fclose(fp);
            return -1;
        }
        
        total_mp3_bytes += mp3_bytes;
        samples_processed += samples_to_encode;
    }
    
    // Flush remaining data
    int flush_bytes = lame_encode_flush(lame, mp3_buffer + total_mp3_bytes, 
                                       mp3_buffer_size - total_mp3_bytes);
    if (flush_bytes > 0) {
        total_mp3_bytes += flush_bytes;
    }
    
    // Write MP3 data to file
    fwrite(mp3_buffer, 1, total_mp3_bytes, fp);
    
    // Cleanup
    free(mp3_buffer);
    lame_close(lame);
    fclose(fp);
    
    return 0;
}