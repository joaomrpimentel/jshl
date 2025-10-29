/**
 * @file parser.h
 * @brief JSHL language parser interface
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef PARSER_H
#define PARSER_H

#include "jshl_compiler.h"

/**
 * @brief Main entry point for JSHL source code parsing
 * @param code Null-terminated JSHL source code string
 * @param list Output note list to populate
 * 
 * Splits source into lines, initializes default synthesizer state,
 * and invokes recursive parser. Default state:
 * - Waveform: SQUARE
 * - Envelope: A=0.01s, D=0s, S=1.0, R=0.01s
 * - Slide: 0s (disabled)
 */
void parse_jshl(char* code, NoteList* list);

#endif /* PARSER_H */