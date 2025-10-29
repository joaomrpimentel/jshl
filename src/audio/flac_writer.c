/**
 * @file flac_writer.c
 * @brief FLAC file export implementation using libFLAC
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <FLAC/stream_encoder.h>
#include "flac_writer.h"

/**
 * @brief Converts float samples to 24-bit integer samples
 * @param float_buffer Input float buffer [-1.0, 1.0]
 * @param int_buffer Output 32-bit integer buffer (24-bit precision)
 * @param sample_count Number of samples to convert
 */
static void float_to_int24(const float* float_buffer, FLAC__int32* int_buffer, 
                          long sample_count) {
    const float scale = 8388607.0f;  // 2^23 - 1 (24-bit max)
    
    for (long i = 0; i < sample_count; i++) {
        float sample = float_buffer[i];
        
        // Clamp to [-1.0, 1.0]
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;
        
        // Scale to 24-bit integer range
        int_buffer[i] = (FLAC__int32)(sample * scale);
    }
}

int write_flac_file(const char* filename, float* buffer, long sample_count, int sample_rate) {
    FLAC__StreamEncoder* encoder = NULL;
    FLAC__int32* int_buffer = NULL;
    int status = 0;
    
    // Allocate integer buffer for conversion
    int_buffer = (FLAC__int32*)malloc(sample_count * sizeof(FLAC__int32));
    if (!int_buffer) {
        fprintf(stderr, "Error: Failed to allocate conversion buffer\n");
        return -1;
    }
    
    // Convert float to 24-bit integer
    float_to_int24(buffer, int_buffer, sample_count);
    
    // Create encoder
    encoder = FLAC__stream_encoder_new();
    if (!encoder) {
        fprintf(stderr, "Error: Failed to create FLAC encoder\n");
        free(int_buffer);
        return -1;
    }
    
    // Configure encoder
    FLAC__stream_encoder_set_channels(encoder, 1);              // Mono
    FLAC__stream_encoder_set_bits_per_sample(encoder, 24);     // 24-bit
    FLAC__stream_encoder_set_sample_rate(encoder, sample_rate);
    FLAC__stream_encoder_set_compression_level(encoder, 8);    // Max compression
    FLAC__stream_encoder_set_total_samples_estimate(encoder, sample_count);
    
    // Initialize encoder
    FLAC__StreamEncoderInitStatus init_status = 
        FLAC__stream_encoder_init_file(encoder, filename, NULL, NULL);
    
    if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
        fprintf(stderr, "Error: FLAC encoder initialization failed: %s\n",
                FLAC__StreamEncoderInitStatusString[init_status]);
        FLAC__stream_encoder_delete(encoder);
        free(int_buffer);
        return -1;
    }
    
    // Encode audio in chunks
    const int chunk_size = 4096;
    long samples_encoded = 0;
    
    while (samples_encoded < sample_count) {
        int samples_to_encode = (sample_count - samples_encoded < chunk_size)
                               ? (sample_count - samples_encoded)
                               : chunk_size;
        
        FLAC__bool encode_ok = FLAC__stream_encoder_process_interleaved(
            encoder,
            int_buffer + samples_encoded,
            samples_to_encode
        );
        
        if (!encode_ok) {
            fprintf(stderr, "Error: FLAC encoding failed\n");
            status = -1;
            break;
        }
        
        samples_encoded += samples_to_encode;
    }
    
    // Finalize encoding
    if (status == 0) {
        if (!FLAC__stream_encoder_finish(encoder)) {
            fprintf(stderr, "Error: Failed to finalize FLAC file\n");
            status = -1;
        }
    }
    
    // Cleanup
    FLAC__stream_encoder_delete(encoder);
    free(int_buffer);
    
    return status;
}