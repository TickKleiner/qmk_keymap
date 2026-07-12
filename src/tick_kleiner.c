#include QMK_KEYBOARD_H

#include "tick_kleiner.h"
#include "keycodes.h"
#include "layout.h"

#include "utils/global_state.h"

///////////////////////////////////////////////////////////////////////////////
// User macro callbacks (https://docs.qmk.fm/feature_macros)
///////////////////////////////////////////////////////////////////////////////

enum nav_hold_source {
    NAV_SRC_HOME      = 1 << 0,
    NAV_SRC_SYM_SHIFT = 1 << 1,
    NAV_SRC_SLS       = 1 << 2,
    NAV_SRC_EQL       = 1 << 3,
};

enum sym_hold_source {
    SYM_SRC_HOME = 1 << 0,
};

enum injected_mod_source {
    MOD_SRC_REPEAT_SPACE = 1 << 0,
    MOD_SRC_SYM_SHIFT    = 1 << 1,
    MOD_SRC_NAV_SLS      = 1 << 2,
    MOD_SRC_NAV_EQL      = 1 << 3,
    MOD_SRC_WIN_TAB      = 1 << 4,
};

static uint8_t nav_hold_sources;
static uint8_t sym_hold_sources;
static uint8_t injected_lctrl_sources;
static uint8_t injected_lshift_sources;
static uint8_t injected_lalt_sources;

static void layer_off_if_unlocked(uint8_t layer) {
    if (!is_layer_locked(layer)) {
        layer_off(layer);
    }
}

static void nav_hold_on(uint8_t source) {
    nav_hold_sources |= source;
    layer_on(NAV);
}

static void nav_hold_off(uint8_t source) {
    if ((nav_hold_sources & source) == 0) return;
    nav_hold_sources &= ~source;
    if (nav_hold_sources == 0) {
        layer_off_if_unlocked(NAV);
    }
}

static void sym_hold_on(uint8_t source) {
    sym_hold_sources |= source;
    layer_on(SYM);
}

static void sym_hold_off(uint8_t source) {
    if ((sym_hold_sources & source) == 0) return;
    sym_hold_sources &= ~source;
    if (sym_hold_sources == 0) {
        layer_off_if_unlocked(SYM);
    }
}

static void inject_mod(uint8_t source, uint8_t mod, uint8_t *sources) {
    if (*sources != 0) {
        *sources |= source;
    } else if (((get_mods() | get_weak_mods()) & mod) == 0) {
        // Weak mods are independently owned by this injection mechanism. This
        // avoids unregistering a physically held modifier when the last
        // injected source is released.
        add_weak_mods(mod);
        *sources |= source;
        send_keyboard_report();
    }
}

static void release_injected_mod(uint8_t source, uint8_t mod, uint8_t *sources) {
    if ((*sources & source) == 0) return;
    *sources &= ~source;
    if (*sources == 0) {
        del_weak_mods(mod);
        send_keyboard_report();
    }
}

// The core clears all weak mods at the start of every key press event
// (action.c), so injected weak mods must be re-asserted on each record before
// the key registers and a report goes out.
static void reassert_injected_mods(void) {
    uint8_t mods = 0;
    if (injected_lctrl_sources) mods |= MOD_BIT_LCTRL;
    if (injected_lshift_sources) mods |= MOD_BIT_LSHIFT;
    if (injected_lalt_sources) mods |= MOD_BIT_LALT;
    if (mods) add_weak_mods(mods);
}

static void inject_mods(uint8_t source, uint8_t mods) {
    if (mods & MOD_BIT_LCTRL) {
        inject_mod(source, MOD_BIT_LCTRL, &injected_lctrl_sources);
    }
    if (mods & MOD_BIT_LSHIFT) {
        inject_mod(source, MOD_BIT_LSHIFT, &injected_lshift_sources);
    }
    if (mods & MOD_BIT_LALT) {
        inject_mod(source, MOD_BIT_LALT, &injected_lalt_sources);
    }
}

static void release_injected_mods(uint8_t source) {
    release_injected_mod(source, MOD_BIT_LCTRL, &injected_lctrl_sources);
    release_injected_mod(source, MOD_BIT_LSHIFT, &injected_lshift_sources);
    release_injected_mod(source, MOD_BIT_LALT, &injected_lalt_sources);
}

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    pre_process_record_set_state_user(keycode, record);
    if (GLOBAL_STATE->layer == SYM && record->event.pressed) {
        clear_weak_mods();
        reassert_injected_mods();
        send_keyboard_report();
    }

#ifdef REPEAT_KEY_ENABLE
    // If alt repeating key A, E, I, O, U, Y with no mods other than Shift, set
    // the last key to KC_N. Above, alternate repeat of KC_N is defined to be
    // again KC_N. This way, either tapping alt repeat and then repeat (or
    // equivalently double tapping alt repeat) is useful to type certain patterns
    // without SFBs:
    //
    //   D <altrep> <rep> -> DYN (as in "dynamic")
    //   O <altrep> <rep> -> OAN (as in "loan")
    if (get_repeat_key_count() < 0 && GLOBAL_STATE->layer == EN && (GLOBAL_STATE->all_mods & ~MOD_MASK_SHIFT) == 0 && (keycode == KC_A || keycode == KC_E || keycode == KC_I || keycode == KC_O || keycode == KC_U || keycode == KC_Y)) {
        set_last_keycode(KC_N);
        set_last_mods(0);
    }
#endif //* REPEAT_KEY_ENABLE
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    dlog_record(keycode, record);
    reassert_injected_mods();
    switch (GLOBAL_STATE->layer) {
        case RU:
            switch (keycode) {
                case WIN_YU:
                    if (!record->tap.count) {
                        if (!record->event.pressed) {
                            release_injected_mods(MOD_SRC_WIN_TAB);
                        }
                    }
                    break;
            }
            break;
        case EN:
            switch (keycode) {
                // Behavior:
                //  * Unmodified:       _ (KC_UNDS)
                //  * With Alt:         Unicode en dash
                //  * With Shift + Alt: Unicode em dash
                case KC_UNDS:
                    if (GLOBAL_STATE->all_mods & MOD_BIT_LALT) {
                        if (record->event.pressed) {
                            send_unicode_string(GLOBAL_STATE->all_mods & MOD_MASK_SHIFT ? "\xe2\x80\x94" : "\xe2\x80\x93");
                        }
                        return false;
                    }
                    break;
                case WIN_DOT:
                    if (!record->tap.count) {
                        if (!record->event.pressed) {
                            release_injected_mods(MOD_SRC_WIN_TAB);
                        }
                    }
                    break;
            }
            break;
    }
    switch (keycode) {
        // When the Repeat key follows Space, it behaves as one-shot shift.
        case KC_SPC:
            if (record->event.pressed) {
                if (get_repeat_key_count() > 0) {
                    add_oneshot_mods(MOD_LSFT);
                    inject_mods(MOD_SRC_REPEAT_SPACE, MOD_BIT_LSHIFT);
                    return false;
                }
            } else if (injected_lshift_sources & MOD_SRC_REPEAT_SPACE) {
                release_injected_mods(MOD_SRC_REPEAT_SPACE);
                return false;
            }
            break;
        //* HOME ROW MODS
        case SYM_T:   // EN
        case SYM_YRU: // RU
            if (!record->tap.count) {
                if (record->event.pressed) {
                    if (GLOBAL_STATE->mods & MOD_BIT_LSHIFT) {
                        inject_mods(MOD_SRC_SYM_SHIFT, MOD_BIT_LCTRL);
                        nav_hold_on(NAV_SRC_SYM_SHIFT);
                    } else {
                        sym_hold_on(SYM_SRC_HOME);
                    }
                } else {
                    release_injected_mods(MOD_SRC_SYM_SHIFT);
                    nav_hold_off(NAV_SRC_SYM_SHIFT);
                    sym_hold_off(SYM_SRC_HOME);
                }
                return false;
            }
            break;
        case NAV_D: // EN
        case NAV_A: // RU
            if (!record->tap.count) {
                if (record->event.pressed) {
                    nav_hold_on(NAV_SRC_HOME);
                } else {
                    nav_hold_off(NAV_SRC_HOME);
                }
                return false; // NAV switch.
            }
            break;
        case NAV_SLS:
            if (!record->tap.count) {
                if (record->event.pressed) {
                    if (GLOBAL_STATE->home_held[LEFT_HOME_ROW][RING_COL]) {
                        inject_mods(MOD_SRC_NAV_SLS, MOD_BIT_LCTRL | MOD_BIT_LSHIFT);
                        nav_hold_on(NAV_SRC_SLS);
                    }
                } else {
                    release_injected_mods(MOD_SRC_NAV_SLS);
                    nav_hold_off(NAV_SRC_SLS);
                }
                return false;
            }
            break; // Default handling taps /.
        case NAV_EQL:
            if (!record->tap.count) {
                if (record->event.pressed) {
                    if (GLOBAL_STATE->home_held[LEFT_HOME_ROW][RING_COL]) {
                        inject_mods(MOD_SRC_NAV_EQL, MOD_BIT_LCTRL);
                        nav_hold_on(NAV_SRC_EQL);
                    }
                } else {
                    release_injected_mods(MOD_SRC_NAV_EQL);
                    nav_hold_off(NAV_SRC_EQL);
                }
                return false;
            }
            break;
        case NLK_HLD:
            if (!record->tap.count) {
                if (record->event.pressed) {
                    tap_code16_delay(KC_NUM_LOCK, TAP_CODE_DELAY);
                } else {
                    tap_code16_delay(KC_NUM_LOCK, TAP_CODE_DELAY);
                }
            }
            break;
        case SLK_HLD:
            if (!record->tap.count) {
                if (record->event.pressed) {
                    tap_code16_delay(KC_SCROLL_LOCK, TAP_CODE_DELAY);
                } else {
                    tap_code16_delay(KC_SCROLL_LOCK, TAP_CODE_DELAY);
                }
            }
            break;
    }
    if (record->event.pressed) {
        switch (keycode) {
            //* WIN TAB KEYS
            case BWD_TAB:
            case FWD_TAB:
                inject_mods(MOD_SRC_WIN_TAB, MOD_BIT_LALT);
                break;
            case UPDIR:
                SEND_STRING_DELAY("../", TAP_CODE_DELAY);
                return false;
            case TMUXESC: // Enter copy mode in Tmux.
                SEND_STRING_DELAY(SS_LCTL("a") SS_TAP(X_ESC), TAP_CODE_DELAY);
                set_last_keycode(C(KC_U));
                return false;
            case SRCHSEL: // Searches the current selection in a new tab.
                SEND_STRING_DELAY(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER), TAP_CODE_DELAY);
                return false;
#ifdef REPEAT_KEY_ENABLE
            // Macros invoked through the MAGIC key.
            case M_THE:
                MAGIC_STRING(/* */ "the", KC_N);
                break;
            case M_ION:
                MAGIC_STRING(/*i*/ "on", KC_S);
                break;
            case M_MENT:
                MAGIC_STRING(/*m*/ "ent", KC_S);
                break;
            case M_QUEN:
                MAGIC_STRING(/*q*/ "uen", KC_C);
                break;
            case M_TMENT:
                MAGIC_STRING(/*t*/ "ment", KC_S);
                break;
            case M_UPDIR:
                MAGIC_STRING(/*.*/ "./", UPDIR);
                break;
#endif
            case M_INCLUDE:
                SEND_STRING_DELAY(/*#*/ "include ", TAP_CODE_DELAY);
                break;
            case M_EQEQ:
                SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY);
                break;
            case M_NBSP:
                SEND_STRING_DELAY(/*&*/ "nbsp;", TAP_CODE_DELAY);
                break;
            case M_MKGRVS:
                SEND_STRING_DELAY(/*`*/ "``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY);
                break;
            case M_DOCSTR:
                SEND_STRING_DELAY(/*"*/ "\"\"\"\"\"" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
                break;
        }
    }
    return true;
}
