/**
 * @file cli.c
 * @brief Command-line interface implementation
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <getopt.h>
#include <ctype.h>
#include "cli.h"
#include "jshl_compiler.h"

#define VERSION "1.0.0"
#define DEFAULT_OUTPUT "output.wav"

/**
 * @brief Determines output format from file extension
 * @param filename File path to analyze
 * @return Detected format or FORMAT_UNKNOWN
 */
static OutputFormat detect_format_from_extension(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return FORMAT_UNKNOWN;
    
    if (strcmp(ext, ".wav") == 0) return FORMAT_WAV;
    if (strcmp(ext, ".raw") == 0 || strcmp(ext, ".pcm") == 0) return FORMAT_RAW;
    
    return FORMAT_UNKNOWN;
}

/**
 * @brief Parses format string to enum
 * @param format_str Format string ("wav", "raw")
 * @return Corresponding OutputFormat enum
 */
static OutputFormat parse_format_string(const char* format_str) {
    if (strcasecmp(format_str, "wav") == 0) return FORMAT_WAV;
    if (strcasecmp(format_str, "raw") == 0) return FORMAT_RAW;
    if (strcasecmp(format_str, "pcm") == 0) return FORMAT_RAW;
    
    return FORMAT_UNKNOWN;
}

void cli_print_help(const char* program_name) {
    printf("Usage: %s [OPTIONS] <input.jshl> [output]\n\n", program_name);
    printf("JSHL Compiler - Converts JSHL music notation to audio files\n\n");
    
    printf("Arguments:\n");
    printf("  <input.jshl>        Input JSHL source file (required)\n");
    printf("  [output]            Output audio file (default: output.wav)\n\n");
    
    printf("Options:\n");
    printf("  -f, --format FORMAT Output format: wav, raw (default: wav)\n");
    printf("  -r, --rate RATE     Sample rate in Hz (default: %d)\n", SAMPLE_RATE);
    printf("  -v, --verbose       Enable verbose output\n");
    printf("  -h, --help          Show this help message\n");
    printf("  -V, --version       Show version information\n\n");
    
    printf("Formats:\n");
    printf("  wav                 WAV file with RIFF header (32-bit float PCM)\n");
    printf("  raw                 Raw PCM data, 32-bit float, no header\n\n");
    
    printf("Examples:\n");
    printf("  %s song.jshl                    # Compile to output.wav\n", program_name);
    printf("  %s song.jshl music.wav          # Compile to music.wav\n", program_name);
    printf("  %s -f raw song.jshl audio.raw   # Output raw PCM data\n", program_name);
    printf("  %s -r 48000 song.jshl           # Use 48kHz sample rate\n", program_name);
    printf("  %s -v song.jshl                 # Verbose compilation\n\n", program_name);
    
    printf("JSHL Language:\n");
    printf("  WAVE <type>         Set waveform: SINE, SQUARE, SAWTOOTH, TRIANGLE\n");
    printf("  ENVELOPE A D S R    Configure ADSR envelope (seconds, 0-1)\n");
    printf("  SLIDE <time>        Set pitch slide duration (seconds)\n");
    printf("  <note> <duration>   Play note (C3-B5, sharps/flats supported)\n");
    printf("  PAUSE <duration>    Add silence\n");
    printf("  LOOP <count> { }    Repeat enclosed block\n\n");
    
    printf("Report bugs to: <your-email@example.com>\n");
}

void cli_print_version(void) {
    printf("JSHL Compiler v%s\n", VERSION);
    printf("Build date: %s %s\n", __DATE__, __TIME__);
    printf("Sample rate: %d Hz\n", SAMPLE_RATE);
    printf("Copyright (c) 2025 - MIT License\n");
}

bool cli_parse_args(int argc, char* argv[], CliConfig* config) {
    // Initialize defaults
    config->input_file = NULL;
    config->output_file = DEFAULT_OUTPUT;
    config->format = FORMAT_WAV;
    config->sample_rate = SAMPLE_RATE;
    config->verbose = false;
    config->show_help = false;
    config->show_version = false;
    
    // Define long options
    static struct option long_options[] = {
        {"format",  required_argument, 0, 'f'},
        {"rate",    required_argument, 0, 'r'},
        {"verbose", no_argument,       0, 'v'},
        {"help",    no_argument,       0, 'h'},
        {"version", no_argument,       0, 'V'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    // Parse options
    while ((opt = getopt_long(argc, argv, "f:r:vhV", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'f':
                config->format = parse_format_string(optarg);
                if (config->format == FORMAT_UNKNOWN) {
                    fprintf(stderr, "Error: Unknown format '%s'\n", optarg);
                    fprintf(stderr, "Supported formats: wav, raw\n");
                    return false;
                }
                break;
                
            case 'r':
                config->sample_rate = atoi(optarg);
                if (config->sample_rate < 8000 || config->sample_rate > 192000) {
                    fprintf(stderr, "Error: Sample rate must be between 8000 and 192000 Hz\n");
                    return false;
                }
                break;
                
            case 'v':
                config->verbose = true;
                break;
                
            case 'h':
                config->show_help = true;
                return false;
                
            case 'V':
                config->show_version = true;
                return false;
                
            case '?':
                // getopt_long already printed error message
                return false;
                
            default:
                return false;
        }
    }
    
    // Parse positional arguments
    int remaining_args = argc - optind;
    
    if (remaining_args < 1) {
        fprintf(stderr, "Error: No input file specified\n");
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return false;
    }
    
    config->input_file = argv[optind];
    
    if (remaining_args >= 2) {
        config->output_file = argv[optind + 1];
        
        // Auto-detect format from output filename if not explicitly set
        if (opt == -1) {  // No -f option was used
            OutputFormat detected = detect_format_from_extension(config->output_file);
            if (detected != FORMAT_UNKNOWN) {
                config->format = detected;
            }
        }
    }
    
    // Validate input file extension
    const char* ext = strrchr(config->input_file, '.');
    if (!ext || strcmp(ext, ".jshl") != 0) {
        fprintf(stderr, "Warning: Input file '%s' doesn't have .jshl extension\n", 
                config->input_file);
    }
    
    return true;
}