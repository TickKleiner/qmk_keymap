#pragma once

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#define UNICODE_SELECTED_MODES UNICODE_MODE_WINDOWS

// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
#define TAP_CODE_DELAY 5

// Tap-hold configuration for home row mods.
#define TAPPING_TERM 225
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY

// Enable Chordal Hold (https://github.com/qmk/qmk_firmware/pull/24560)
#define CHORDAL_HOLD

// Use Left Ctrl + Right Ctrl to activate the Command feature.
#define IS_COMMAND() (get_mods() == MOD_MASK_CTRL)

#ifdef CUSTOM_SHIFT_KEYS_ENABLE
// Don't apply custom shift keys with mods other than Shift.
#   define CUSTOM_SHIFT_KEYS_NEGMODS ~MOD_MASK_SHIFT
// Only apply custom shift keys on layers 0 and 4.
#   define CUSTOM_SHIFT_KEYS_LAYER_MASK (1 << 0) | (1 << 5)
#endif  //*CUSTOM_SHIFT_KEYS_ENABLE

#ifdef ORBITAL_MOUSE_ENABLE
// Define speed curve for Orbital Mouse.
#   define ORBITAL_MOUSE_SPEED_CURVE \
    {24, 24, 24, 32, 62, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72}
//     |               |               |               |           |
// t = 0.000           1.024           2.048           3.072       3.840 s
#endif  //*ORBITAL_MOUSE_ENABLE

#ifdef CAPS_WORD_ENABLE
// Holding Shift while Caps Word is active inverts the shift state.
#   define CAPS_WORD_INVERT_ON_SHIFT
// When idle, turn off Caps Word after 5 seconds.
#   define CAPS_WORD_IDLE_TIMEOUT 5000
#endif  //*CAPS_WORD_ENABLE

#ifdef LAYER_LOCK_ENABLE
// When idle, turn off Layer Lock after 60 seconds.
#   define LAYER_LOCK_IDLE_TIMEOUT 60000
#endif  //*LAYER_LOCK_ENABLE
