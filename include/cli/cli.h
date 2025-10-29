/**
 * @file cli.h
 * @brief Command-line interface argument parser
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef CLI_H
#define CLI_H

#include <stdbool.h>

/**
 * @brief Supported output audio formats
 */
typedef enum {
    FORMAT_WAV,
    FORMAT_RAW,
    FORMAT_UNKNOWN
} OutputFormat;

/**
 * @brief Command-line configuration structure
 */
typedef struct {
    const char* input_file;      /**< Input JSHL file path */
    const char* output_file;     /**< Output audio file path */
    OutputFormat format;         /**< Output format (WAV, RAW) */
    int sample_rate;             /**< Sample rate in Hz */
    bool verbose;                /**< Enable verbose output */
    bool show_help;              /**< Show help message */
    bool show_version;           /**< Show version info */
} CliConfig;

/**
 * @brief Parses command-line arguments into configuration
 * @param argc Argument count from main()
 * @param argv Argument vector from main()
 * @param config Output configuration structure
 * @return true if parsing succeeded, false if help/version requested or error
 */
bool cli_parse_args(int argc, char* argv[], CliConfig* config);

/**
 * @brief Displays usage information
 * @param program_name Name of the program (argv[0])
 */
void cli_print_help(const char* program_name);

/**
 * @brief Displays version information
 */
void cli_print_version(void);

#endif /* CLI_H */