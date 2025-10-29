/**
 * @file note_list.h
 * @brief Dynamic note list data structure interface
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef NOTE_LIST_H
#define NOTE_LIST_H

#include "jshl_compiler.h"

/**
 * @brief Initializes a note list with default capacity
 * @param list Pointer to NoteList structure to initialize
 */
void note_list_init(NoteList* list);

/**
 * @brief Adds a note event to the list, expanding capacity if needed
 * @param list Pointer to target NoteList
 * @param note NoteEvent structure to add
 */
void note_list_add(NoteList* list, NoteEvent note);

/**
 * @brief Frees all memory associated with a note list
 * @param list Pointer to NoteList to deallocate
 */
void note_list_free(NoteList* list);

#endif /* NOTE_LIST_H */