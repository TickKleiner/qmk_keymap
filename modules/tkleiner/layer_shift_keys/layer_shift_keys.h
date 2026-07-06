#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t trigger;
    uint16_t unshifted_keycode;
    uint16_t shifted_keycode;
    layer_state_t layers;
} layer_shift_key_t;

extern const layer_shift_key_t layer_shift_keys[];

#ifdef __cplusplus
}
#endif
