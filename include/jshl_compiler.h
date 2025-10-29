#ifndef JSHL_COMPILER_H
#define JSHL_COMPILER_H

#include <stdint.h> // Para tipos de inteiros como uint16_t

// --- Configurações de Áudio ---
#define SAMPLE_RATE 44100
#define MASTER_GAIN 0.1f
#define PI 3.1415926535f

// --- Definições da Linguagem ---

// Tipos de forma de onda
typedef enum {
    WAVE_SINE,
    WAVE_SQUARE,
    WAVE_SAWTOOTH,
    WAVE_TRIANGLE
} WaveType;

// Envelope ADSR
typedef struct {
    float attack;
    float decay;
    float sustain;
    float release;
} Envelope;

// Estado do sintetizador (o que pode mudar durante a música)
typedef struct {
    WaveType wave;
    Envelope envelope;
    float slide;
    float last_freq;
} SynthState;

// Evento de nota, capturado durante o parsing
typedef struct {
    float freq;
    float duration;
    float start_time;
    SynthState state; // Uma cópia do estado do sintetizador NO MOMENTO da nota
} NoteEvent;

// Lista dinâmica para armazenar as notas
typedef struct {
    NoteEvent* notes;
    size_t size;
    size_t capacity;
} NoteList;

// --- Funções da Lista de Notas ---
void note_list_init(NoteList* list);
void note_list_add(NoteList* list, NoteEvent note);
void note_list_free(NoteList* list);

// --- Funções do Compilador ---
void parse_jshl(char* code, NoteList* list);
float* render_audio(NoteList* list, long* total_samples);

// --- Função de Escrita WAV ---
void write_wav_file(const char* filename, float* buffer, long sample_count);

// --- Função de Parsing de Notas ---
float get_note_freq(const char* note_name);

#endif // JSHL_COMPILER_H
