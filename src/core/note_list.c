/**
 * @file note_list.c
 * @brief Dynamic note list data structure implementation
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "note_list.h"

void note_list_init(NoteList* list) {
    list->capacity = 16;
    list->size = 0;
    list->notes = (NoteEvent*)malloc(list->capacity * sizeof(NoteEvent));
    if (!list->notes) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
}

void note_list_add(NoteList* list, NoteEvent note) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->notes = (NoteEvent*)realloc(list->notes, list->capacity * sizeof(NoteEvent));
        if (!list->notes) {
            fprintf(stderr, "Error: Memory reallocation failed\n");
            exit(1);
        }
    }
    list->notes[list->size++] = note;
}

void note_list_free(NoteList* list) {
    free(list->notes);
    list->size = 0;
    list->capacity = 0;
}