/**
 * @file wav_writer.c
 * @brief WAV file export implementation
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include "wav_writer.h"
#include "jshl_compiler.h"

void write_wav_file(const char* filename, float* buffer, long sample_count) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot write to file '%s'\n", filename);
        return;
    }

    int16_t num_channels = 1;
    int16_t bits_per_sample = 32;
    int32_t sample_rate = SAMPLE_RATE;
    int32_t byte_rate = sample_rate * num_channels * (bits_per_sample / 8);
    int16_t block_align = num_channels * (bits_per_sample / 8);
    int32_t subchunk2_size = sample_count * num_channels * (bits_per_sample / 8);
    int32_t chunk_size = 36 + subchunk2_size;

    fwrite("RIFF", 1, 4, fp);
    fwrite(&chunk_size, 4, 1, fp);
    fwrite("WAVE", 1, 4, fp);

    fwrite("fmt ", 1, 4, fp);
    int32_t subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, fp);
    int16_t audio_format = 3;
    fwrite(&audio_format, 2, 1, fp);
    fwrite(&num_channels, 2, 1, fp);
    fwrite(&sample_rate, 4, 1, fp);
    fwrite(&byte_rate, 4, 1, fp);
    fwrite(&block_align, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);

    fwrite("data", 1, 4, fp);
    fwrite(&subchunk2_size, 4, 1, fp);
    fwrite(buffer, sizeof(float), sample_count, fp);

    fclose(fp);
}