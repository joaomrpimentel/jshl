/**
 * @file synth.h
 * @brief Audio synthesis engine interface
 * @author joaomrpimentel
 * @version 1.0
 */

#ifndef SYNTH_H
#define SYNTH_H

#include "jshl_compiler.h"

/**
 * @brief Renders note list to PCM audio buffer
 * @param list Input note list containing all events
 * @param total_samples Output parameter for buffer length
 * @return Pointer to float audio buffer (caller must free), or NULL if empty
 * 
 * Rendering pipeline per note:
 * 1. ADSR envelope calculation (attack/decay/sustain/release phases)
 * 2. Frequency slide interpolation (if configured)
 * 3. Oscillator generation
 * 4. Gain compensation by waveform type
 * 5. Additive mixing into output buffer
 * 6. Hard clipping to [-1.0, 1.0] range
 * 
 * @note Buffer is zero-initialized for clean mixing
 * @note Adds 1 second tail after final note for release fade
 */
float* render_audio(NoteList* list, long* total_samples);

#endif /* SYNTH_H */