/**
 * @file synth.c
 * @brief Audio synthesis engine implementation
 * @author joaomrpimentel
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "synth.h"

/**
 * @brief Generates oscillator sample for given waveform type
 * @param wave Waveform type (SINE, SQUARE, SAWTOOTH, TRIANGLE)
 * @param freq Frequency in Hz
 * @param t Global time in seconds
 * @return Sample value in range [-1.0, 1.0]
 * 
 * Implementation details:
 * - SINE: Pure sinusoidal wave
 * - SQUARE: Sign of sine wave (hard transitions)
 * - SAWTOOTH: Linear ramp with period-based wrapping
 * - TRIANGLE: Arcsin of sine wave for smooth triangle shape
 */
static float get_osc_value(WaveType wave, float freq, float t) {
    float phase = t * freq * 2.0f * PI;
    switch (wave) {
        case WAVE_SINE:     return sinf(phase);
        case WAVE_SQUARE:   return sinf(phase) >= 0.0f ? 1.0f : -1.0f;
        case WAVE_SAWTOOTH: return 2.0f * ( (t * freq) - floorf(0.5f + (t * freq)) );
        case WAVE_TRIANGLE: return asinf(sinf(phase)) * (2.0f / PI);
        default:            return 0.0f;
    }
}

float* render_audio(NoteList* list, long* total_samples) {
    if (list->size == 0) {
        *total_samples = 0;
        return NULL;
    }

    NoteEvent last_note = list->notes[list->size - 1];
    float total_duration = last_note.start_time + last_note.duration + 
                          last_note.state.envelope.release + 1.0f;
    *total_samples = (long)(SAMPLE_RATE * total_duration);

    float* buffer = (float*)calloc(*total_samples, sizeof(float));
    if (!buffer) {
        fprintf(stderr, "Error: Audio buffer allocation failed\n");
        exit(1);
    }

    for (size_t i = 0; i < list->size; i++) {
        NoteEvent note = list->notes[i];
        SynthState s = note.state;
        Envelope e = s.envelope;
        
        float gain = (s.wave == WAVE_SINE) ? 1.0f : 
                    (s.wave == WAVE_TRIANGLE ? 0.8f : 0.25f);
        
        long start_sample = (long)(note.start_time * SAMPLE_RATE);
        long end_sample = (long)((note.start_time + note.duration + e.release) * SAMPLE_RATE);

        for (long j = start_sample; j < end_sample && j < *total_samples; j++) {
            float t = (float)j / SAMPLE_RATE;
            float note_t = t - note.start_time;

            float env_gain = 0.0f;
            if (note_t < 0.0f) continue;
            if (note_t < e.attack) {
                env_gain = note_t / e.attack;
            } else if (note_t < e.attack + e.decay) {
                env_gain = 1.0f - (1.0f - e.sustain) * ((note_t - e.attack) / e.decay);
            } else if (note_t < note.duration) {
                env_gain = e.sustain;
            } else if (note_t < note.duration + e.release) {
                env_gain = e.sustain * (1.0f - ((note_t - note.duration) / e.release));
            }
            env_gain = fmaxf(0.0f, fminf(1.0f, env_gain));

            float current_freq = note.freq;
            if (s.slide > 0.0f && s.last_freq > 0.0f && note_t < s.slide) {
                current_freq = s.last_freq + (note.freq - s.last_freq) * (note_t / s.slide);
            }

            float osc_value = get_osc_value(s.wave, current_freq, t);
            buffer[j] += osc_value * env_gain * gain * MASTER_GAIN;
        }
    }

    for (long i = 0; i < *total_samples; i++) {
        if (buffer[i] > 1.0f) buffer[i] = 1.0f;
        if (buffer[i] < -1.0f) buffer[i] = -1.0f;
    }

    return buffer;
}