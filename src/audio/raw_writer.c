
/**
 * @file raw_writer.c
 * @brief Raw PCM file export implementation
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include "raw_writer.h"

void write_raw_file(const char* filename, float* buffer, long sample_count) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot write to file '%s'\n", filename);
        return;
    }

    // Write raw float samples directly
    fwrite(buffer, sizeof(float), sample_count, fp);
    fclose(fp);
}