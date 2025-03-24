#pragma once
#include "quantum.h"

#ifdef REPEAT_KEY_ENABLE
void magic_send_string_P(const char* str, uint16_t repeat_keycode);
#define MAGIC_STRING(str, repeat_keycode) \
    magic_send_string_P(PSTR(str), (repeat_keycode))
#endif  //* REPEAT_KEY_ENABLE

#if !defined(NO_DEBUG) && defined(KEYCODE_STRING_ENABLE)
void dlog_record(uint16_t keycode, keyrecord_t* record);
#else
#   pragma message "dlog_record: disabled"
#   define dlog_record(keycode, record)
#endif  //* !defined(NO_DEBUG) && defined(KEYCODE_STRING_ENABLE)
