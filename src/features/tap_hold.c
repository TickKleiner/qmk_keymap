#include "src/keycodes.h"
#include "src/utils/global_state.h"
#include "layout.h"

///////////////////////////////////////////////////////////////////////////////
// Tap-hold configuration (https://docs.qmk.fm/tap_hold)
///////////////////////////////////////////////////////////////////////////////
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t* record) {
    switch (get_highest_layer(layer_state)) {
        case RU:
            switch (keycode) {
                case SFT_VE: // RU
                case SFT_EL: // RU
                    return TAPPING_TERM - 45;
                    break;
            }
            break;
        case EN:
            switch (keycode) {
                case SFT_R: // EN
                case SFT_E: // EN
                    return TAPPING_TERM - 45;
            }
            break;
    }
    return TAPPING_TERM;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t* record) {
    // If you quickly hold a tap-hold key after tapping it, the tap action is
    // repeated. Key repeating is useful e.g. for Vim navigation keys, but can
    // lead to missed triggers in fast typing. Here, returning 0 means we
    // instead want to "force hold" and disable key repeating.
    switch (get_highest_layer(layer_state)) {
        case RU:
            switch (keycode) {
                case NUM_O:   // RU
                case CTL_SFT: // RU
                    return QUICK_TAP_TERM;
            }
            break;
        case EN:
            switch (keycode) {
                case NUM_N: // EN
                case CTL_H: // EN
                    return QUICK_TAP_TERM;
            }
            break;
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

#ifdef FLOW_TAP_TERM
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record, uint16_t prev_keycode) {
    // Only apply Flow Tap when following a letter key, and not hotkeys.
    const uint8_t active_mods = get_mods() | get_weak_mods() | get_oneshot_mods() | get_speculative_mods();
    if (get_tap_keycode(prev_keycode) <= KC_Z && (active_mods & (MOD_MASK_CG | MOD_MASK_ALT)) == 0) {
        switch (get_highest_layer(layer_state)) {
            case RU:
                switch (keycode) {
                    case ALT_EF:
                    case GUI_YA:
                    case ALT_ZHE:
                    case GUI_E:
                    case WIN_YU:
                        return FLOW_TAP_TERM;
                    case CTL_EM:
                    case CTL_SFT:
                        return FLOW_TAP_TERM - 25;
                }
                break;
            case EN:
                switch (keycode) {
                    case ALT_S:
                    case GUI_X:
                    case ALT_I:
                    case GUI_QUO:
                    case WIN_DOT:
                        return FLOW_TAP_TERM;
                    case CTL_G:
                    case CTL_H:
                        return FLOW_TAP_TERM - 25;
                }
                break;
        }
    }
    return 0; // Disable Flow Tap otherwise.
}
#endif //* FLOW_TAP_TERM

#ifdef COMMUNITY_MODULE_TAP_FLOW_ENABLE
uint16_t get_tap_flow_term(uint16_t keycode, keyrecord_t* record, uint16_t prev_keycode) {
    // Only apply Tap Flow when following a letter key.
    if (get_tap_keycode(prev_keycode) <= KC_Z) {
        switch (get_highest_layer(layer_state)) {
            case RU:
                switch (keycode) {
                    case ALT_EF:
                    case GUI_YA:
                    case ALT_ZHE:
                    case GUI_E:
                    case WIN_YU:
                        return g_tap_flow_term;
                    case SYM_YRU:
                    case NAV_A:
                    case CTL_EM:
                    case NUM_O:
                    case CTL_SFT:
                    case SYM_DE:
                        return g_tap_flow_term - 25;
                }
                break;
            case EN:
                switch (keycode) {
                    case ALT_S:
                    case GUI_X:
                    case ALT_I:
                    case GUI_QUO:
                    case WIN_DOT:
                        return g_tap_flow_term;
                    case SYM_T:
                    case NAV_D:
                    case CTL_G:
                    case NUM_N:
                    case CTL_H:
                    case SYM_A:
                        return g_tap_flow_term - 25;
                }
                break;
        }
    }
    return 0;
}
#endif // COMMUNITY_MODULE_TAP_FLOW_ENABLE

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
    // A Shift MT never chords with an Alt/GUI MT, in either order: if both settled as held
    // (permissive hold, or the chordal queue scan settling a second queued MT as held),
    // Alt+Shift lands in a report = Windows layout-switch hotkey. Such rolls are always taps.
    if (IS_QK_MOD_TAP(tap_hold_keycode) && IS_QK_MOD_TAP(other_keycode)) {
        const uint8_t a = mt_mods(tap_hold_keycode);
        const uint8_t b = mt_mods(other_keycode);
        if (((a & MOD_MASK_SHIFT) && (b & (MOD_MASK_ALT | MOD_MASK_GUI))) || ((b & MOD_MASK_SHIFT) && (a & (MOD_MASK_ALT | MOD_MASK_GUI)))) return false;
    }
    switch (get_highest_layer(layer_state)) {
        case RU:
            switch (tap_hold_keycode) {
                case NAV_A:
                    // Physical NAV shortcut positions: Select all, Undo, Cut,
                    // Copy, and Paste.
                    if (other_keycode == RU_SHTI || other_keycode == RU_TSE || other_keycode == RU_U || other_keycode == RU_ES || other_keycode == CTL_EM) return true;
                    break;
                case NUM_O:
                    if (other_keycode == QK_REP) {
                        return true;
                    }
                    break;
                case WIN_YU:
                    if (other_keycode == CTL_SFT || other_keycode == RU_BE) return true;
                    break;
                case CTL_EM:
                    // Same-hand Ctrl+A/S/C/X rolls on the RU alpha layer.
                    if (other_keycode == ALT_EF || other_keycode == SYM_YRU || other_keycode == RU_ES || other_keycode == MUS_CH) return true;
                    break;
            }
            break;
        case EN:
            switch (tap_hold_keycode) {
                case NAV_D:
                    // Physical NAV shortcut positions: Select all, Undo, Cut,
                    // Copy, and Paste.
                    if (other_keycode == KC_V || other_keycode == KC_M || other_keycode == KC_L || other_keycode == KC_J || other_keycode == CTL_G) return true;
                    break;
                case NUM_N: // Allow one-handed N + Repeat chord to type "0" on num layer.
                    if (other_keycode == QK_REP) {
                        return true;
                    }
                    break;
                case WIN_DOT:
                    if (other_keycode == CTL_H || other_keycode == KC_COMM) return true;
                    break;
                case CTL_G: // Same-hand Ctrl shortcuts without waiting out TAPPING_TERM.
                    if (other_keycode == KC_V || other_keycode == KC_C || other_keycode == GUI_X || other_keycode == ALT_S) return true;
                    break;
            }
            break;
        case NAV:
            switch (tap_hold_keycode) {
                case NAV_SLS:
                    return true;
            }
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif //* CHORDAL_HOLD

#ifdef SPECULATIVE_HOLD
// Alt/GUI mod-tap keys of both base layers, by matrix position (same physical keys on EN and RU).
static bool alt_or_gui_mt_is_down(void) {
    return GLOBAL_STATE->home_held[LEFT_HOME_ROW][PINKY_COL]        // ALT_S  / ALT_EF
           || GLOBAL_STATE->home_held[LEFT_BOTTOM_ROW][PINKY_COL]   // GUI_X  / GUI_YA
           || GLOBAL_STATE->home_held[RIGHT_HOME_ROW][PINKY_COL]    // ALT_I  / ALT_ZHE
           || GLOBAL_STATE->home_held[RIGHT_BOTTOM_ROW][RING_COL]   // WIN_DOT / WIN_YU
           || GLOBAL_STATE->home_held[RIGHT_BOTTOM_ROW][PINKY_COL]; // GUI_QUO / GUI_E
}

// Shift mod-tap keys of both base layers, by matrix position.
static bool shift_mt_is_down(void) {
    return GLOBAL_STATE->home_held[LEFT_HOME_ROW][MIDDLE_COL]      // SFT_R / SFT_VE
           || GLOBAL_STATE->home_held[RIGHT_HOME_ROW][MIDDLE_COL]; // SFT_E / SFT_EL
}

// All mods are speculative (lone Alt/GUI flashes are suppressed via DUMMY_MOD_NEUTRALIZER_KEYCODE),
// except combinations that could put Alt+Shift in a report: that is the Windows layout-switch
// hotkey, and a spurious flash both switches the OS language and desyncs ru_en's current_language.
// Two ways it can happen:
//  * the new mod plus already-active mods (real, weak or one-shot) combines Alt with Shift;
//  * a Shift MT flashes while an Alt/GUI MT is still physically down and unsettled (or vice
//    versa): if the other MT later settles as hold (permissive hold in a roll like "ir" or
//    "ei"), the real mod joins the still-flashed speculative one.
bool get_speculative_hold(uint16_t keycode, keyrecord_t* record) {
    uint8_t mods = QK_MOD_TAP_GET_MODS(keycode);
    if (mods & 0x10) mods <<= 4; // 5-bit MT mods: bit 4 set means right-hand mods.
    if ((mods & MOD_MASK_SHIFT) && alt_or_gui_mt_is_down()) return false;
    if ((mods & (MOD_MASK_ALT | MOD_MASK_GUI)) && shift_mt_is_down()) return false;
    const uint8_t all_mods = get_mods() | get_weak_mods() | get_oneshot_mods() | get_speculative_mods() | mods;
    if ((all_mods & MOD_MASK_ALT) && (all_mods & MOD_MASK_SHIFT)) return false;
    return true;
}
#endif //* SPECULATIVE_HOLD
