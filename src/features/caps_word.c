#include "src/keycodes.h"
#include "src/utils/global_state.h"
///////////////////////////////////////////////////////////////////////////////
// Caps word (https://docs.qmk.fm/features/caps_word)
///////////////////////////////////////////////////////////////////////////////
#ifdef CAPS_WORD_ENABLE
bool caps_word_press_user(uint16_t keycode) {
    if (IS_QK_MOD_TAP(keycode)) return true;
    if (IS_QK_LAYER_TAP(keycode)) return true;
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT_LSHIFT);  // Apply shift to the next key.
            return true;
        case KC_LBRC ... KC_RBRC:
        case KC_SCLN ... KC_DOT:
            if (GLOBAL_STATE->layer == RU) {
                add_weak_mods(MOD_BIT_LSHIFT);  // Apply shift to the next key.
                return true;
            }
            break;
        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case LT_WORD:
        case M_THE:
        case M_ION:
        case M_MENT:
        case M_QUEN:
        case M_TMENT:
            return true;
        default:
            break;  // Deactivate Caps Word.
    }
    return false;
}
#endif  //* CAPS_WORD_ENABLE
