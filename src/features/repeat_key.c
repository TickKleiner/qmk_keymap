#include "src/tick_kleiner.h"
#include "src/keycodes.h"
#include "src/utils/global_state.h"

///////////////////////////////////////////////////////////////////////////////
// Repeat key (https://docs.qmk.fm/features/repeat_key)
///////////////////////////////////////////////////////////////////////////////
#ifdef REPEAT_KEY_ENABLE
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {
    switch (keycode) {
// Unpack tapping keycode for tap-hold keys.
#ifndef NO_ACTION_TAPPING
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
            break;
#ifndef NO_ACTION_LAYER
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
            break;
#endif  //*NO_ACTION_LAYER
#endif  //*NO_ACTION_TAPPING
        default:
            break;
    }
    if (GLOBAL_STATE->layer == RU) {
// Forget Shift on most letters when Shift or AltGr are the only mods. Some
// letters are excluded, e.g. for "NN" and "ZZ" in Vim.
        switch (keycode) {
            case KC_A ... KC_Z:
            case KC_LBRC ... KC_RBRC:
            case KC_SCLN ... KC_DOT:
                if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
                    *remembered_mods &= ~MOD_MASK_SHIFT;
                }
                break;
            default:
                break;
        }

    } else {
        switch (keycode) {
// Forget Shift on most letters when Shift or AltGr are the only mods. Some
// letters are excluded, e.g. for "NN" and "ZZ" in Vim.
            case KC_A ... KC_H:
            case KC_K ... KC_M:
            case KC_O ... KC_U:
                if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT_RALT)) == 0) {
                    *remembered_mods &= ~MOD_MASK_SHIFT;
                }
                break;
            default:
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
    const bool SHIFT_MASK = (mods & ~MOD_MASK_SHIFT) == 0;
    const bool SHIFTED = SHIFT_MASK && (mods & MOD_MASK_SHIFT) != 0;
    const bool ALTED = mods == MOD_BIT_LALT;
    // This is where most of the "magic" for the MAGIC key is implemented.
    switch (keycode) {
        case KC_SPC:  // spc -> THE
        case KC_ENT:
        case KC_TAB:
                        if (SHIFT_MASK) return M_THE;
                        break;
        // For navigating next/previous search results in Vim:
        // N -> Shift + N, Shift + N -> N.
        case NUM_N:     if (!mods)      return S(KC_N);
                        if (ALTED)      return A(KC_I);
                        break;
        // Fall through intended.
        case KC_N:      if (SHIFT_MASK) return KC_N;
                        break;
        // Fix SFBs and awkward strokes.
        case SYM_A:     if (SHIFT_MASK) return KC_O;        // A -> O
                        break;
        case KC_O:      if (SHIFT_MASK) return KC_A;        // O -> A
                        if (ALTED)      return A(KC_U);
                        break;
        case SFT_E:     if (SHIFT_MASK) return KC_U;        // E -> U
                        break;
        case KC_U:      if (SHIFT_MASK) return KC_E;        // U -> E
                        if (ALTED)      return A(KC_O);
                        break;
        case ALT_I:     if (!mods)      return M_ION;       // I -> ON
                        if (SHIFT_MASK) return KC_QUOT;     // Shift I -> '
                        if (ALTED)      return A(KC_N);
                        break;
        case KC_M:      if (SHIFT_MASK) return M_MENT;      // M -> ENT
                        break;
        case KC_Q:      if (SHIFT_MASK) return M_QUEN;      // Q -> UEN
                        break;
        case SYM_T:     if (SHIFT_MASK) return M_TMENT;     // T -> TMENT
                        break;
        case KC_C:      if (SHIFT_MASK) return KC_Y;        // C -> Y
                        break;
        case NAV_D:     if (SHIFT_MASK) return KC_Y;        // D -> Y
                        break;
        case CTL_G:     if (SHIFT_MASK) return KC_Y;        // G -> Y
                        break;
        case KC_P:      if (SHIFT_MASK) return KC_Y;        // P -> Y
                        break;
        case KC_Y:      if (SHIFT_MASK) return KC_P;        // Y -> P
                        break;
        case KC_L:      if (SHIFT_MASK) return KC_K;        // L -> K
                        break;
        case ALT_S:     if (SHIFT_MASK) return KC_K;        // S -> K
                        break;
        case SFT_R:     if (SHIFT_MASK) return KC_L;        // R -> L
                        break;
        case WIN_DOT:   if (!mods)      return M_UPDIR;     // . -> ./
                        break;
        case KC_EQL:    return M_EQEQ;                      // = -> ==
                        break;
        case KC_RBRC:   if (SHIFT_MASK) return KC_SCLN;     // ] -> ;
                        break;
        case KC_COMM:
                        if (SHIFTED) return KC_EQL;         // ! -> =
                        return M_NOOP;
        case GUI_QUO:   if (SHIFTED)    return M_DOCSTR;    // " -> ""<cursor>"""
                        break;
        case KC_GRV:    if (SHIFT_MASK) return M_MKGRVS;    // ` -> ``<cursor>``` (for Markdown code)
                        break;
        case KC_LABK:   if (SHIFT_MASK) return KC_MINS;     // < -> - (for Haskell)
                        break;
        case KC_SLSH:   if (SHIFT_MASK) return KC_SLSH;     // / -> / (easier reach than Repeat)
                        break;
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
        case KC_HASH:
        case KC_AMPR:
                        if (SHIFT_MASK) return KC_EQL;
                        break;
        case KC_F:
        case KC_V:
        case GUI_X:
        case KC_SCLN:
        case KC_1 ... KC_0:
                        if (SHIFT_MASK) return M_NOOP;
                        break;

        case KC_WH_U:                   return KC_WH_D;
        case KC_WH_D:                   return KC_WH_U;
        case SELWBAK:                   return SELWFWD;
        case SELWFWD:                   return SELWBAK;
        default:
            break;
    }
    return KC_TRNS;
}
// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`. This helper is
// used for several macros below in my process_record_user() function.
void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
    const uint8_t saved_mods = get_mods();
    // If Caps Word is on, save the mods and hold Shift.
    if (is_caps_word_on()) register_mods(MOD_BIT_LSHIFT);
    send_string_P(str);  // Send the string.
    set_last_keycode(repeat_keycode);
    // If Caps Word is on, restore the mods.
    if (is_caps_word_on()) set_mods(saved_mods);
}
#endif  //* REPEAT_KEY_ENABLE
