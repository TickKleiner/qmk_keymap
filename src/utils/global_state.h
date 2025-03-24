#pragma once
#include "src/keycodes.h"

typedef struct global_state {
    language_t cur_lng;
    uint8_t layer;
    uint8_t mods;
    uint8_t weak_mods;
#ifndef NO_ACTION_ONESHOT
    uint8_t oneshot_mods;
#endif
    uint8_t all_mods;
    bool home_held[MATRIX_ROWS][MATRIX_COLS];
} global_state_t;

#ifndef LNG_DELAY
#   define LNG_DELAY 50
#endif  //* LNG_DELAY
#ifndef LNG_SET
#   define LNG_SET(LNG) C(KC_1 + LNG)
#endif  //* LNG_SET

extern const global_state_t *GLOBAL_STATE;

void set_lng(language_t lng);
bool pre_process_record_set_state_user(uint16_t keycode, keyrecord_t *record);
