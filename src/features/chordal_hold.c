#include "src/keycodes.h"
#include "src/utils/global_state.h"

///////////////////////////////////////////////////////////////////////////////
// Handedness for Chordal Hold (https://github.com/qmk/qmk_firmware/pull/24560)
///////////////////////////////////////////////////////////////////////////////
#ifdef CHORDAL_HOLD
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
    switch (tap_hold_keycode) {
        case NAV_SLS:
            return true;
        case NAV_D:
            if (other_keycode == KC_M       ||
                other_keycode == KC_L       ||
                other_keycode == KC_Y       ||
                other_keycode == KC_K       ||
                other_keycode == KC_J)      return true;
            break;
        case NAV_A:
            if (other_keycode == RU_TSE     ||
                other_keycode == RU_U       ||
                other_keycode == RU_PE      ||
                other_keycode == RU_CHE     ||
                other_keycode == RU_ES)     return true;
            break;
        case WIN_DOT:
            if (GLOBAL_STATE->layer == EN && (
                other_keycode == CTL_H      ||
                other_keycode == KC_COMM))  return true;
            else if (GLOBAL_STATE->layer == RU && (
                other_keycode == CTL_SFT    ||
                other_keycode == RU_BE))     return true;
            break;
        default:
            break;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif  //* CHORDAL_HOLD
