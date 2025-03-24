#include "src/keycodes.h"

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case SFT_R:     // EN
        case SFT_E:     // EN
        case SFT_VE:    // RU
        case SFT_EL:    // RU
            return TAPPING_TERM - 45;
        default:
            return TAPPING_TERM;
    }
}
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
    // If you quickly hold a tap-hold key after tapping it, the tap action is
    // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
    // lead to missed triggers in fast typing. Here, returning 0 means we
    // instead want to "force hold" and disable key repeating.
    switch (keycode) {
        case NUM_N:     // EN
        case CTL_H:     // EN
            return QUICK_TAP_TERM;  // Enable key repeating.
        default:
            return 0;  // Otherwise, force hold and disable key repeating.
    }
}
