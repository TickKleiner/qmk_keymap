#include "src/tick_kleiner.h"
#include "src/keycodes.h"

///////////////////////////////////////////////////////////////////////////////
// Repeat key (https://docs.qmk.fm/features/repeat_key)
///////////////////////////////////////////////////////////////////////////////
#ifdef REPEAT_KEY_ENABLE
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    keycode = get_tap_keycode(keycode);
#ifdef COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
    if (is_sentence_case_primed() && sentence_case_press_user(keycode, record, *remembered_mods) == 'a') {
        *remembered_mods |= MOD_BIT_LSHIFT;
    }
#endif  //*COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
    switch (get_highest_layer(layer_state)) {
        case RU:
            switch (keycode) {
                case RU_EF ... RU_YA:
                case RU_ZHE ... RU_YU:
                case RU_HA ... RU_HARD:
                    if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
                        *remembered_mods &= ~MOD_MASK_SHIFT;
                    }
                    break;
            }
            break;
        case EN:
            switch (keycode) {
                case KC_A ... KC_H:
                case KC_K ... KC_M:
                case KC_O ... KC_U:
                    if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
                        *remembered_mods &= ~MOD_MASK_SHIFT;
                    }
                    break;
            }
    }
    return true;
}
// The following describes the magic key functionality, where * represents the
// magic key and @ the repeat key. For example, tapping A and then the magic key
// types "ao". Most of this is coded in my `get_alt_repeat_key_keycode_user()`
// definition below.
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PY      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (like "adjustments")
//     = *   -> ===             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    if (get_highest_layer(layer_state) != EN)
        return KC_TRNS;
    keycode = get_tap_keycode(keycode);
    if (mods == MOD_BIT_LALT) {
        switch (keycode) {
            case KC_U: return A(KC_O);
            case KC_O: return A(KC_U);
            case KC_N: return A(KC_I);
            case KC_I: return A(KC_N);
        }
    } else if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
            case KC_SPC:  // spc -> THE
            case KC_ENT:
            case KC_TAB:
                return M_THE;
            // For navigating next/previous search results in Vim:
            // N -> Shift + N, Shift + N -> N.
            case KC_N:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return S(KC_N);
                }
                return KC_N;

            // Fix SFBs and awkward strokes.
            case KC_A: return KC_O;         // A -> O
            case KC_O: return KC_A;         // O -> A
            case KC_E: return KC_U;         // E -> U
            case KC_U: return KC_E;         // U -> E
            case KC_I:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_ION;           // I -> ON
                } else {
                    return KC_QUOT;         // Shift I -> '
                }
            case KC_M: return M_MENT;       // M -> ENT
            case KC_Q: return M_QUEN;       // Q -> UEN
            case KC_T: return M_TMENT;      // T -> TMENT

            case KC_C: return KC_Y;         // C -> Y
            case KC_D: return KC_Y;         // D -> Y
            case KC_G: return KC_Y;         // G -> Y
            case KC_P: return KC_Y;         // P -> Y
            case KC_Y: return KC_P;         // Y -> P

            case KC_L: return KC_K;         // L -> K
            case KC_S: return KC_K;         // S -> K

            case KC_R: return KC_L;         // R -> L
            case KC_DOT:
                if ((mods & MOD_MASK_SHIFT) == 0) {
                    return M_UPDIR;         // . -> ./
                }
                return M_NOOP;
            case KC_HASH: return M_INCLUDE; // # -> include
            case KC_AMPR: return M_NBSP;    // & -> nbsp;
            case KC_EQL: return M_EQEQ;     // = -> ==
            case KC_RBRC: return KC_SCLN;   // ] -> ;

            case KC_COMM:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return KC_EQL;          // ! -> =
                }
                return M_NOOP;
            case KC_QUOT:
                if ((mods & MOD_MASK_SHIFT) != 0) {
                    return M_DOCSTR;        // " -> ""<cursor>"""
                }
                return M_NOOP;
            case KC_GRV:                    // ` -> ``<cursor>``` (for Markdown code)
                return M_MKGRVS;
            case KC_LABK:                   // < -> - (for Haskell)
                return KC_MINS;
            case KC_SLSH:
                return KC_SLSH;             // / -> / (easier reach than Repeat)
            case KC_PLUS:
            case KC_MINS:
            case KC_ASTR:
            case KC_PERC:
            case KC_PIPE:
            case KC_CIRC:
            case KC_TILD:
            case KC_EXLM:
            case KC_DLR:
            case KC_RABK:
            case KC_LPRN:
            case KC_RPRN:
            case KC_UNDS:
            case KC_COLN:
                return KC_EQL;

            case KC_F:
            case KC_V:
            case KC_X:
            case KC_SCLN:
            case KC_1 ... KC_0:
                return M_NOOP;
        }
    }

    switch (keycode) {
        case MS_WHLU: return MS_WHLD;
        case MS_WHLD: return MS_WHLU;
        case SELWBAK: return SELWORD;
        case SELWORD: return SELWBAK;
    }
    return KC_TRNS;
}
// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used for several macros below in my process_record_user() function.
void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
  uint8_t saved_mods = 0;
  // If Caps Word is on, save the mods and hold Shift.
  if (is_caps_word_on()) {
    saved_mods = get_mods();
    register_mods(MOD_BIT_LSHIFT);
  }
  send_string_P(str);  // Send the string.
  set_last_keycode(repeat_keycode);
  // If Caps Word is on, restore the mods.
  if (is_caps_word_on()) {
    set_mods(saved_mods);
  }
}
#endif  //* REPEAT_KEY_ENABLE
