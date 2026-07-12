#include "src/keycodes.h"

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case SFT_R:
        case SFT_VE:
        case SFT_E:
        case SFT_EL:
            return TAPPING_TERM - 45;
    }
    return TAPPING_TERM;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
    // If you quickly hold a tap-hold key after tapping it, the tap action is
    // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
    // lead to missed triggers in fast typing. Here, returning 0 means we
    // instead want to "force hold" and disable key repeating.
    switch (keycode) {
        case NUM_N:
        case NUM_O:
        case CTL_H:
        case CTL_SFT:
            return QUICK_TAP_TERM;
    }
    return 0;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t* record) {
    // These keys are intentionally used for shortcuts on the same hand. Once
    // Chordal Hold has accepted the pair, settle the hold on the second key's
    // press instead of waiting for its release (Permissive Hold) or the term.
    switch (keycode) {
        case CTL_G:
        case CTL_EM:
        case NAV_D:
        case NAV_A:
        case WIN_DOT: // WIN_YU has the same encoded LT keycode.
        case NAV_SLS:
        case NAV_EQL:
            return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// Handedness for Chordal Hold (https://github.com/qmk/qmk_firmware/pull/24560)
///////////////////////////////////////////////////////////////////////////////
#ifdef CHORDAL_HOLD
static uint8_t mt_mods(uint16_t keycode) {
    uint8_t mods = QK_MOD_TAP_GET_MODS(keycode);
    if (mods & 0x10) mods <<= 4; // 5-bit MT mods: bit 4 set means right-hand mods.
    return mods;
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
    // A Shift MT never chords with an Alt/GUI MT, in either order: if both settled as held,
    // Alt+Shift would land in a report and trigger the Windows layout-switch hotkey.
    if (IS_QK_MOD_TAP(tap_hold_keycode) && IS_QK_MOD_TAP(other_keycode)) {
        const uint8_t a = mt_mods(tap_hold_keycode);
        const uint8_t b = mt_mods(other_keycode);
        if (((a & MOD_MASK_SHIFT) && (b & (MOD_MASK_ALT | MOD_MASK_GUI))) || ((b & MOD_MASK_SHIFT) && (a & (MOD_MASK_ALT | MOD_MASK_GUI)))) return false;
    }

    // Space after the home-row NAV letter is ordinary word completion, not a
    // request to activate NAV. In particular, this keeps "cd " reliable.
    if ((tap_hold_keycode == NAV_D || tap_hold_keycode == NAV_A) && other_keycode == KC_SPC) return false;

    switch (tap_hold_keycode) {
        case NAV_D:
            // Physical NAV shortcut positions: Select all, Undo, Cut, Copy, and Paste.
            if (other_keycode == KC_V || other_keycode == KC_M || other_keycode == KC_L || other_keycode == KC_J || other_keycode == CTL_G) return true;
            break;
        case NAV_A:
            // Physical NAV shortcut positions: Select all, Undo, Cut, Copy, and Paste.
            if (other_keycode == RU_SHTI || other_keycode == RU_TSE || other_keycode == RU_U || other_keycode == RU_ES || other_keycode == CTL_EM) return true;
            break;
        case NUM_N:
        case NUM_O:
            // Allow the one-handed alpha + Repeat chord to type "0" on NUM.
            if (other_keycode == QK_REP) return true;
            break;
        case WIN_DOT: // Also WIN_YU: both macros encode to the same layer-tap keycode.
            // Alt+Tab and Shift+Alt+Tab, in both language layouts.
            if (other_keycode == CTL_H || other_keycode == CTL_SFT || other_keycode == KC_COMM || other_keycode == RU_BE) return true;
            break;
        case CTL_G:
            // Same-hand Ctrl+A/S/C/X/V shortcuts on EN.
            if (other_keycode == KC_V || other_keycode == KC_C || other_keycode == GUI_X || other_keycode == ALT_S) return true;
            break;
        case CTL_EM:
            // Same-hand Ctrl+A/S/C/X/V shortcuts on RU.
            if (other_keycode == ALT_EF || other_keycode == SYM_YRU || other_keycode == RU_ES || other_keycode == MUS_CH) return true;
            break;
        case NAV_SLS:
            return true;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif //* CHORDAL_HOLD
