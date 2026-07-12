#pragma once

#ifdef VIA_ENABLE
#    define VIA_EEPROM_LAYOUT_OPTIONS_SIZE 3
#    define VIA_EEPROM_LAYOUT_OPTIONS_DEFAULT 0x00000fc0
#endif

// -----------------------------------------------------------------------------
// Sets the delay between register_code and unregister_code, if you're having issues with it registering properly (common on VUSB boards). The value is in milliseconds and defaults to 0.
// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
#define TAP_CODE_DELAY 5
// Do NOT lower MATRIX_IO_DELAY below the 30 us default: the matrix column lines need time to
// recharge after row unselect; 5 us caused phantom presses down the same column on the right half.
// -----------------------------------------------------------------------------
#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE
// -----------------------------------------------------------------------------
// how long before a key press becomes a hold
#define TAPPING_TERM 225
// enables handling for per key TAPPING_TERM settings
#define TAPPING_TERM_PER_KEY
// -----------------------------------------------------------------------------
// Enable Chordal Hold (https://github.com/qmk/qmk_firmware/pull/24560)
#define CHORDAL_HOLD
// -----------------------------------------------------------------------------
// tap anyway, even after TAPPING_TERM, if there was no other key interruption between press and release
//* #define RETRO_TAPPING
// enables handling for per key RETRO_TAPPING settings
//* #define RETRO_TAPPING_PER_KEY
// -----------------------------------------------------------------------------
// selects the hold action of a dual-role key as soon as the tap of the dual-role key is interrupted by the press of another key.
// See "hold on other key press" for details
//* #define HOLD_ON_OTHER_KEY_PRESS
// enables handling for per key HOLD_ON_OTHER_KEY_PRESS settings
//* #define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// -----------------------------------------------------------------------------
// tap-then-hold timing to use a dual role key to repeat keycode
// See Quick Tap Term
// Changes the timing of Tap Toggle functionality (TT or the One Shot Tap Toggle)
// Defaults to TAPPING_TERM if not defined
//* #define QUICK_TAP_TERM 120
// enables handling for per key QUICK_TAP_TERM settings
#define QUICK_TAP_TERM_PER_KEY
// -----------------------------------------------------------------------------
// makes tap and hold keys trigger the hold if another key is pressed before releasing, even if it hasn't hit the TAPPING_TERM
// See Permissive Hold for details
#define PERMISSIVE_HOLD
// enabled handling for per key PERMISSIVE_HOLD settings
//* #define PERMISSIVE_HOLD_PER_KEY
// -----------------------------------------------------------------------------
// how many taps before triggering the toggle
#define TAPPING_TOGGLE 2
// how long before oneshot times out
#define ONESHOT_TIMEOUT 60000
// how many taps before oneshot toggle is triggered
#define ONESHOT_TAP_TOGGLE 2
// -----------------------------------------------------------------------------
#ifdef COMBO_ENABLE
//  how long for the Combo keys to be detected. Defaults to TAPPING_TERM if not defined.
#    define COMBO_TERM 50
//  Flag for enabling extending timeout on Combos containing modifiers
#    define COMBO_MUST_HOLD_MODS
//  Allows for extending COMBO_TERM for mod keys while mid-combo.
#    define COMBO_MOD_TERM 200
//  Flag to enable per-combo COMBO_TERM extension and get_combo_must_hold() function
#    define COMBO_MUST_HOLD_PER_COMBO
//  Flag to enable per-combo COMBO_TERM extension and get_combo_term() function
#    define COMBO_TERM_PER_COMBO
//  Only start the combo timer on the first key press instead of on all key presses.
#    define COMBO_STRICT_TIMER
#endif //* COMBO_ENABLE
// -----------------------------------------------------------------------------
/* FEATURES THAT CAN BE ENABLED */
// force a key release to be evaluated using the current layer stack instead of remembering which layer it came from (used for advanced cases)
//* #define STRICT_LAYER_RELEASE
// -----------------------------------------------------------------------------
#ifdef COMMUNITY_MODULE_LAYER_SHIFT_KEYS_ENABLE
// Don't apply layer shift keys with mods other than Shift.
#    define LAYER_SHIFT_KEYS_NEGMODS ~MOD_MASK_SHIFT
#endif //* COMMUNITY_MODULE_LAYER_SHIFT_KEYS_ENABLE
// -----------------------------------------------------------------------------
#ifdef COMMUNITY_MODULE_ORBITAL_MOUSE_ENABLE
// Define speed curve for Orbital Mouse.
#    define ORBITAL_MOUSE_SPEED_CURVE {24, 24, 24, 32, 62, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72}
//     |               |               |               |           |
// t = 0.000           1.024           2.048           3.072       3.840 s
#endif //* COMMUNITY_MODULE_ORBITAL_MOUSE_ENABLE
// -----------------------------------------------------------------------------
#ifdef CAPS_WORD_ENABLE
// Holding Shift while Caps Word is active inverts the shift state.
#    define CAPS_WORD_INVERT_ON_SHIFT
// When idle, turn off Caps Word after 5 seconds.
#    define CAPS_WORD_IDLE_TIMEOUT 5000
#endif //* CAPS_WORD_ENABLE
// -----------------------------------------------------------------------------
#ifdef LAYER_LOCK_ENABLE
// When idle, turn off Layer Lock after 60 seconds.
#    define LAYER_LOCK_IDLE_TIMEOUT 60000
#endif //*LAYER_LOCK_ENABLE
// -----------------------------------------------------------------------------
#ifdef LEADER_ENABLE
//  how long before the leader key times out
//  If you're having issues finishing the sequence before it times out, you may need to increase the timeout setting. Or you may want to enable the LEADER_PER_KEY_TIMING option, which resets the timeout after each key is tapped.
#    define LEADER_TIMEOUT 300
//  sets the timer for leader key chords to run on each key press rather than overall
#    define LEADER_PER_KEY_TIMING
//  Disables keycode filtering for Mod-Tap and Layer-Tap keycodes. Eg, if you enable this, you would need to specify MT(MOD_CTL, KC_A) if you want to use KC_A.
#    define LEADER_KEY_STRICT_KEY_PROCESSING
#endif //*LEADER_ENABLE
// -----------------------------------------------------------------------------
#ifdef MOUSEKEY_ENABLE
#    define MOUSEKEY_INTERVAL 20
#    define MOUSEKEY_DELAY 0
#    define MOUSEKEY_TIME_TO_MAX 60
#    define MOUSEKEY_MAX_SPEED 7
#    define MOUSEKEY_WHEEL_DELAY 0
//  Enables support for extended reports (-32767 to 32767, instead of -127 to 127), which may allow for smoother reporting, and prevent maxing out of the reports. Applies to both Pointing Device and Mousekeys.
#    define MOUSE_EXTENDED_REPORT
#endif //*MOUSEKEY_ENABLE

// Flow Tap makes hold decisions depend on typing rhythm, which is unsuitable
// for this keymap. Keep it an explicitly forbidden architectural dependency.
#ifdef FLOW_TAP_TERM
#    error "FLOW_TAP_TERM must remain disabled for this keymap"
#endif

#ifdef COMMUNITY_MODULE_RU_EN_ENABLE
#    define RUEN_DEFAULT_LANGUAGE RUEN_EN
#    define RUEN_RU_LANGUAGE_LAYER 1
#    define RUEN_RU_LANGUAGE_KEY C(KC_2)
#    define RUEN_EN_LANGUAGE_LAYER 0
#    define RUEN_EN_LANGUAGE_KEY C(KC_1)
#    define RUEN_TOGGLE_LANGUAGE_MOD_KEY KC_LALT
#    define RUEN_TOGGLE_LANGUAGE_KEY KC_LSFT
#endif

#ifdef COMMUNITY_MODULE_SENTENCE_CASE_ENABLE
#    define SENTENCE_CASE_TIMEOUT 2000
#endif //* COMMUNITY_MODULE_SENTENCE_CASE_ENABLE

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
