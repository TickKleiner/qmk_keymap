#include "src/keycodes.h"

///////////////////////////////////////////////////////////////////////////////
// Caps word (https://docs.qmk.fm/features/caps_word)
///////////////////////////////////////////////////////////////////////////////
#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
    if (IS_QK_MOD_TAP(keycode)) return true;
    if (IS_QK_LAYER_TAP(keycode)) return true;
    switch (get_highest_layer(layer_state)) {
        case EN:
            switch (keycode) {
                case KC_A ... KC_Z:
                case KC_MINS:
                    add_weak_mods(MOD_BIT_LSHIFT);  // Apply shift to the next key.
                    return true;
                case KC_1 ... KC_0:
                case KC_BSPC:
                case KC_DEL:
                case KC_UNDS:
                case M_THE:
                case M_ION:
                case M_MENT:
                case M_QUEN:
                case M_TMENT:
                    return true;
            }
            break;
        case RU:
            switch (keycode) {
                case RU_EF ... RU_YA:
                case RU_ZHE ... RU_YU:
                case RU_HA ... RU_HARD:
                    add_weak_mods(MOD_BIT_LSHIFT);
                    return true;
                case RU_1 ... RU_0:
                case KC_BSPC:
                case KC_DEL:
                case KC_UNDS:
                    return true;
            }
            break;
        default:
            break;
    }
    return false;
}
#endif  //* CAPS_WORD_ENABLE
