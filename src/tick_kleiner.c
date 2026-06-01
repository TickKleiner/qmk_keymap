#include QMK_KEYBOARD_H

#include "tick_kleiner.h"
#include "keycodes.h"
#include "layout.h"

#include "utils/global_state.h"

///////////////////////////////////////////////////////////////////////////////
// User macro callbacks (https://docs.qmk.fm/feature_macros)
///////////////////////////////////////////////////////////////////////////////

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    pre_process_record_set_state_user(keycode, record);
    if (GLOBAL_STATE->layer == NAV
        && !(is_layer_locked((NAV))
            || GLOBAL_STATE->home_held[LEFT_HOME_ROW][INDEX_COL]
            || GLOBAL_STATE->home_held[LEFT_HOME_ROW][MIDDLE_COL]
            || GLOBAL_STATE->home_held[LEFT_HOME_ROW][RING_COL]))
    {
            layer_off(NAV);
    }

    if (GLOBAL_STATE->layer == SYM && record->event.pressed) {
        clear_weak_mods();
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
    if (get_repeat_key_count() < 0 && GLOBAL_STATE->layer == EN && (GLOBAL_STATE->all_mods & ~MOD_MASK_SHIFT) == 0
        && (keycode == KC_A || keycode == KC_E || keycode == KC_I ||
            keycode == KC_O || keycode == KC_U || keycode == KC_Y)) {
        set_last_keycode(KC_N);
        set_last_mods(0);
    }
#endif  //* REPEAT_KEY_ENABLE
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    dlog_record(keycode, record);
    switch (GLOBAL_STATE->layer) {
        case RU:
            switch (keycode) {
                case RU_SLSH:
                    if (record->event.pressed)
                    {
                        if (GLOBAL_STATE->all_mods & MOD_MASK_SHIFT) {
                            del_weak_mods(MOD_MASK_SHIFT);
                            unregister_mods(MOD_MASK_SHIFT);
                            tap_code16_delay(RU_SLSH, TAP_CODE_DELAY);
                            set_mods(GLOBAL_STATE->mods);
                        } else {
                            tap_code16_delay(RU_BSLS, TAP_CODE_DELAY);
                        }
                        return false;
                    }
                    break;
                case RU_MINS:
                    if (record->event.pressed)
                    {
                        if (GLOBAL_STATE->mods & MOD_MASK_SHIFT) {
                            del_weak_mods(MOD_MASK_SHIFT);
                            unregister_mods(MOD_MASK_SHIFT);
                            tap_code16_delay(RU_SCLN, TAP_CODE_DELAY);
                            set_mods(GLOBAL_STATE->mods);
                        } else {
                            tap_code16_delay(RU_MINS, TAP_CODE_DELAY);
                        }
                        return false;
                    }
                    break;
                // Hold behavior: switches to EXT layer.
                // Tap behavior:
                //  * Unmodified:       :
                //  * With Shift:       ;
                case EXT_COL:
                    if (record->tap.count) {
                        if (record->event.pressed) {
                            if (GLOBAL_STATE->all_mods & MOD_MASK_SHIFT)
                            {
                                del_weak_mods(MOD_MASK_SHIFT);
                                unregister_mods(MOD_MASK_SHIFT);
                                tap_code16_delay(RU_SCLN, TAP_CODE_DELAY);
                                set_mods(GLOBAL_STATE->mods);
                            } else {
                                tap_code16_delay(RU_COLN, TAP_CODE_DELAY);
                            }
                        }
                        return false;
                    }
                    break;
                case WIN_YU:
                    if (!record->tap.count) {
                        if (!record->event.pressed) {
                            unregister_mods(MOD_BIT_LALT);
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
                // Hold behavior: switches to EXT layer.
                // Tap behavior:
                //  * Unmodified:       :
                //  * With Shift:       ;
                case EXT_COL:
                    if (record->tap.count) {
                        if (record->event.pressed) {
                            if (GLOBAL_STATE->all_mods & MOD_MASK_SHIFT)
                            {
                                del_weak_mods(MOD_MASK_SHIFT);
                                unregister_mods(MOD_MASK_SHIFT);
                                tap_code16_delay(KC_SCLN, TAP_CODE_DELAY);
                                set_mods(GLOBAL_STATE->mods);
                            } else {
                                tap_code16_delay(KC_COLN, TAP_CODE_DELAY);
                            }
                        }
                        return false;
                    }
                    break;
                case WIN_DOT:
                    if (!record->tap.count) {
                        if (!record->event.pressed) {
                            unregister_mods(MOD_BIT_LALT);
                        }
                    }
                    break;
            }
            break;
    }
    switch (keycode) {
        // When the Repeat key follows Space, it behaves as one-shot shift.
        case KC_SPC:
            if (get_repeat_key_count() > 0) {
                if (record->event.pressed) {
                    add_oneshot_mods(MOD_LSFT);
                    register_mods(MOD_LSFT);
                } else {
                    unregister_mods(MOD_LSFT);
                }
                return false;
            }
            break;
        //* HOME ROW MODS
        case SYM_T:     // EN
        case SYM_YRU:   // RU
            if (!record->tap.count) {
                if (record->event.pressed) {
                    if (GLOBAL_STATE->mods & MOD_BIT_LSHIFT) {
                        register_mods(MOD_BIT_LCTRL);
                        layer_on(NAV);
                    } else {
                        layer_on(SYM);
                    }
                } else {
                    unregister_mods(MOD_BIT_LCTRL);
                    layer_off(SYM);
                }
                return false;
            }
            break;
        case NAV_D:     // EN
        case NAV_A:     // RU
            if (!record->tap.count) {
                if (record->event.pressed) {
                    layer_on(NAV);
                }
                return false; // NAV switch.
            }
            break;
        case NAV_SLS:
            if (!record->tap.count) {
                if (!record->event.pressed) {
                    unregister_mods(MOD_BIT_LSHIFT);
                } else if (GLOBAL_STATE->home_held[LEFT_HOME_ROW][RING_COL]) {
                    register_mods(MOD_BIT_LCTRL | MOD_BIT_LSHIFT);
                    layer_on(NAV);
                }
                return false;
            }
            break;  // Default handling taps /.
        case NAV_EQL:
            if (!record->tap.count) {
                if (record->event.pressed && GLOBAL_STATE->home_held[LEFT_HOME_ROW][RING_COL]) {
                    register_mods(MOD_BIT_LCTRL);
                    layer_on(NAV);
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
            case KC_MPLY:
                if (GLOBAL_STATE->all_mods & MOD_MASK_SHIFT) {
                    del_weak_mods(MOD_MASK_SHIFT);
                    unregister_mods(MOD_MASK_SHIFT);
                    tap_code16_delay(KC_MNXT, TAP_CODE_DELAY);
                    set_mods(GLOBAL_STATE->mods);
                } else {
                    tap_code16_delay(KC_MPLY, TAP_CODE_DELAY);
                }
                return false;
            //* WIN TAB KEYS
            case BWD_TAB:
            case FWD_TAB:
                register_mods(MOD_BIT_LALT);
                break;
            // Behavior:
            //  * Unmodified:       _ (KC_UNDS)
            //  * With Shift:       - (KC_MINS)
            case KC_UNDS:
                if (GLOBAL_STATE->mods & MOD_MASK_SHIFT) {
                    del_weak_mods(MOD_MASK_SHIFT);
                    unregister_mods(MOD_MASK_SHIFT);
                    tap_code_delay(KC_MINS, TAP_CODE_DELAY);
                    set_mods(GLOBAL_STATE->mods);
                } else {
                    tap_code16_delay(KC_UNDS, TAP_CODE_DELAY);
                }
                return false;
            case UPDIR:
                SEND_STRING_DELAY("../", TAP_CODE_DELAY);
                return false;
            case TMUXESC:  // Enter copy mode in Tmux.
                SEND_STRING_DELAY(SS_LCTL("a") SS_TAP(X_ESC), TAP_CODE_DELAY);
                set_last_keycode(C(KC_U));
                return false;
            case SRCHSEL:  // Searches the current selection in a new tab.
                SEND_STRING_DELAY(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER), TAP_CODE_DELAY);
                return false;
#ifdef REPEAT_KEY_ENABLE
            // Macros invoked through the MAGIC key.
            case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
            case M_ION:     MAGIC_STRING(/*i*/"on", KC_S); break;
            case M_MENT:    MAGIC_STRING(/*m*/"ent", KC_S); break;
            case M_QUEN:    MAGIC_STRING(/*q*/"uen", KC_C); break;
            case M_TMENT:   MAGIC_STRING(/*t*/"ment", KC_S); break;
            case M_UPDIR:   MAGIC_STRING(/*.*/"./", UPDIR); break;
#endif
            case M_INCLUDE: SEND_STRING_DELAY(/*#*/"include ", TAP_CODE_DELAY); break;
            case M_EQEQ:    SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY); break;
            case M_NBSP:    SEND_STRING_DELAY(/*&*/"nbsp;", TAP_CODE_DELAY); break;
            case M_MKGRVS: SEND_STRING_DELAY(/*`*/"``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY); break;
            case M_DOCSTR: SEND_STRING_DELAY(/*"*/"\"\"\"\"\""
                SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
                break;
        }
    }
    return true;
}

void housekeeping_task_user(void) {
}
