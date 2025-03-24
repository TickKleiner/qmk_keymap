#include "src/keycodes.h"

///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////
#ifdef CUSTOM_SHIFT_KEYS_ENABLE
#include "getreuer/features/custom_shift_keys.h"
const custom_shift_key_t custom_shift_keys[] = {
    {WIN_DOT, KC_QUES}, /* .    -> ?    */
    {KC_COMM, KC_EXLM}, /* ,    -> !    */
    {KC_MINS, KC_SCLN}, /* -    -> ;    */
    {KC_SLSH, KC_BSLS}, /* /    -> \    */
    {KC_LPRN, KC_LCBR}, /* (    -> {    */
    {KC_RPRN, KC_RCBR}, /* )    -> }    */
};
uint8_t NUM_CUSTOM_SHIFT_KEYS = sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);
#endif  //* CUSTOM_SHIFT_KEYS_ENABLE
