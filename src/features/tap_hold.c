#include "src/keycodes.h"

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

#ifdef FLOW_TAP_TERM
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t* record, uint16_t prev_keycode) {
    // Only apply Flow Tap when following a letter key, and not hotkeys.
    if (get_tap_keycode(prev_keycode) <= KC_Z && (get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) == 0) {
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
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {
    switch (get_highest_layer(layer_state)) {
        case RU:
            switch (tap_hold_keycode) {
                case NAV_A:
                    if (other_keycode == RU_TSE || other_keycode == RU_U || other_keycode == RU_PE || other_keycode == RU_CHE || other_keycode == RU_ES) return true;
                    break;
                case NUM_O:
                    if (other_keycode == QK_REP) {
                        return true;
                    }
                    break;
                case WIN_YU:
                    if (other_keycode == CTL_SFT || other_keycode == RU_BE) return true;
                    break;
            }
            break;
        case EN:
            switch (tap_hold_keycode) {
                case NAV_D:
                    if (other_keycode == KC_M || other_keycode == KC_L || other_keycode == KC_Y || other_keycode == KC_K || other_keycode == KC_J) return true;
                    break;
                case NUM_N: // Allow one-handed N + Repeat chord to type "0" on num layer.
                    if (other_keycode == QK_REP) {
                        return true;
                    }
                    break;
                case WIN_DOT:
                    if (other_keycode == CTL_H || other_keycode == KC_COMM) return true;
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

#ifdef COMMUNITY_MODULE_SPECULATIVE_HOLD_ENABLE
bool get_speculative_hold(uint16_t keycode, keyrecord_t* record) {
    return true; // Enable for all mods.
}
#endif //* COMMUNITY_MODULE_SPECULATIVE_HOLD_ENABLE
