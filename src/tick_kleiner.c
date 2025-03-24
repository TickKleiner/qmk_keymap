#include QMK_KEYBOARD_H

#include "tick_kleiner.h"
#include "keycodes.h"
#include "layout.h"

#include "utils/global_state.h"

#ifdef CUSTOM_SHIFT_KEYS_ENABLE
#include "getreuer/features/custom_shift_keys.h"
#endif  //* CUSTOM_SHIFT_KEYS_ENABLE
#ifdef ORBITAL_MOUSE_ENABLE
#include "getreuer/features/orbital_mouse.h"
#endif  //* ORBITAL_MOUSE_ENABLE
#ifdef SELECT_WORD_ENABLE
#include "getreuer/features/select_word.h"
#endif  //* SELECT_WORD_ENABLE

static bool layer_word_on = false;
static language_t layer_word_lng = LNG_START;

bool get_layer_word(uint16_t keycode) {
    if (IS_QK_MOD_TAP(keycode)) return true;
    if (IS_QK_LAYER_TAP(keycode)) return true;
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            return true;
        case KC_LBRC ... KC_RBRC:
        case KC_SCLN ... KC_DOT:
            if (GLOBAL_STATE->layer == RU) {
                return true;
            }
            break;
        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case CW_TOGG:
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

///////////////////////////////////////////////////////////////////////////////
// User macro callbacks (https://docs.qmk.fm/feature_macros)
///////////////////////////////////////////////////////////////////////////////
void keyboard_post_init_user(void) {
    register_code16(LNG_SET(LNG_START));
    unregister_code16(LNG_SET(LNG_START));
}
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    pre_process_record_set_state_user(keycode, record);
    if (layer_word_on && keycode != LT_WORD) {
        layer_word_on = get_layer_word(keycode);
        if (!layer_word_on) {
            if (layer_word_lng == LNG_1) {
                set_single_default_layer(RU);
                layer_on(RU);
            } else if (layer_word_lng == LNG_2) {
                set_single_default_layer(EN);
                layer_on(EN);
            }
        }
    }
    if (!(is_layer_locked(NAV)
        || GLOBAL_STATE->home_held[LEFT_HOME_ROW][INDEX_COL]
        || GLOBAL_STATE->home_held[LEFT_HOME_ROW][MIDDLE_COL]
        || GLOBAL_STATE->home_held[LEFT_HOME_ROW][RING_COL])) {
        layer_off(NAV);
    }
    if (GLOBAL_STATE->layer == SYM && record->event.pressed) {
        clear_weak_mods();
        send_keyboard_report();
    }
    if (GLOBAL_STATE->layer == RU) {
        if (GLOBAL_STATE->cur_lng != LNG_2) {
            set_lng(LNG_2);
        }
    } else {
        if (GLOBAL_STATE->cur_lng != LNG_1) {
            set_lng(LNG_1);
        }
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
#ifdef ORBITAL_MOUSE_ENABLE
    if (!process_orbital_mouse(keycode, record)) return false;
#endif  //* ORBITAL_MOUSE_ENABLE
#ifdef SELECT_WORD_ENABLE
    if (!process_select_word(keycode, record)) return false;
#endif  //* SELECT_WORD_ENABLE
#ifdef CUSTOM_SHIFT_KEYS_ENABLE
    if (!process_custom_shift_keys(keycode, record)) return false;
#endif  //* CUSTOM_SHIFT_KEYS_ENABLE
    dlog_record(keycode, record);

    switch (keycode) {
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
        // Hold behavior: switches to EXT layer.
        // Tap behavior:
        //  * Unmodified:       :
        //  * With Shift:       ;
        case EXT_COL:
            if (record->tap.count) {
                if (record->event.pressed) {
                    if (GLOBAL_STATE->all_mods & MOD_MASK_SHIFT) {
                        del_weak_mods(MOD_MASK_SHIFT);
                        unregister_mods(MOD_MASK_SHIFT);
                        if (GLOBAL_STATE->layer == RU) {
                            tap_code16_delay(RU_SCLN, TAP_CODE_DELAY);
                        } else {
                            tap_code_delay(KC_SCLN, TAP_CODE_DELAY);
                        }
                        set_mods(GLOBAL_STATE->mods);
                    } else {
                        if (GLOBAL_STATE->layer == RU) {
                            tap_code16_delay(RU_COLN, TAP_CODE_DELAY);
                        } else {
                            tap_code16_delay(KC_COLN, TAP_CODE_DELAY);
                        }
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
        //* MACRO KEYS
        case SELWBAK:  // Backward word selection.
            if (record->event.pressed) {
                select_word_register('B');
            } else {
                select_word_unregister();
            }
            break;
        case SELWFWD:  // Forward word selection.
            if (record->event.pressed) {
                select_word_register('W');
            } else {
                select_word_unregister();
            }
            break;
        case SELLINE:  // Line selection.
            if(record->event.pressed) {
                select_word_register('L');
            } else {
                select_word_unregister();
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
            //* RU KEYS
            case RU_IE:
                if (GLOBAL_STATE->layer == RU) {
                    if (GLOBAL_STATE->all_mods & MOD_BIT_LALT) {
                        del_weak_mods(MOD_BIT_LALT);
                        unregister_mods(MOD_BIT_LALT);
                        tap_code16_delay(RU_YO, TAP_CODE_DELAY);
                    } else {
                        tap_code16_delay(RU_IE, TAP_CODE_DELAY);
                    }
                    return false;
                }
                break;
            case KC_SLSH:
                if (GLOBAL_STATE->layer == RU) {
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
            case KC_MINS:
                if (GLOBAL_STATE->layer == RU) {
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
            //* LAYER WORD
            case LT_WORD:
                layer_word_on = !layer_word_on;
                if (layer_word_on) {
                    if (GLOBAL_STATE->layer == EN) {
                        layer_word_lng = LNG_2;
                        set_single_default_layer(RU);
                        layer_on(RU);
                    } else if (GLOBAL_STATE->layer == RU) {
                        layer_word_lng = LNG_1;
                        set_single_default_layer(EN);
                        layer_on(EN);
                    }
                } else {
                    if (layer_word_lng == LNG_1) {
                        set_single_default_layer(RU);
                        layer_on(RU);
                    } else if (layer_word_lng == LNG_2) {
                        set_single_default_layer(EN);
                        layer_on(EN);
                    }
                }
                return false;
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
#ifdef ORBITAL_MOUSE_ENABLE
    orbital_mouse_task();
#endif  //*ORBITAL_MOUSE_ENABLE
#ifdef SELECT_WORD_ENABLE
    select_word_task();
#endif  //*SELECT_WORD_ENABLE
}
