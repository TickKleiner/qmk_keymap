///////////////////////////////////////////////////////////////////////////////
// Debug logging
///////////////////////////////////////////////////////////////////////////////
#ifndef NO_DEBUG
#pragma message "dlog_record: enabled"

#ifdef COMMUNITY_MODULE_KEYCODE_STRING_ENABLE
KEYCODE_STRING_NAMES_USER(
  KEYCODE_STRING_NAME(ARROW),
  KEYCODE_STRING_NAME(UPDIR),
  KEYCODE_STRING_NAME(STDCC),
  KEYCODE_STRING_NAME(USRNAME),
  KEYCODE_STRING_NAME(TMUXESC),
  KEYCODE_STRING_NAME(SRCHSEL),
  KEYCODE_STRING_NAME(SELWORD),
  KEYCODE_STRING_NAME(SELWBAK),
  KEYCODE_STRING_NAME(SELLINE),
  KEYCODE_STRING_NAME(RGBNEXT),
  KEYCODE_STRING_NAME(RGBHUP),
  KEYCODE_STRING_NAME(RGBHRND),
  KEYCODE_STRING_NAME(RGBDEF1),
  KEYCODE_STRING_NAME(RGBDEF2),
);
#endif  //* COMMUNITY_MODULE_KEYCODE_STRING_ENABLE

static void dlog_record(uint16_t keycode, keyrecord_t* record) {
  if (!debug_enable) { return; }
  uint8_t layer = read_source_layers_cache(record->event.key);
  bool is_tap_hold = IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode);
  xprintf("L%-2u ", layer);  // Log the layer.
  if (IS_COMBOEVENT(record->event)) {  // Combos don't have a position.
    xprintf("combo   ");
  } else {  // Log the "(row,col)" position.
    xprintf("(%2u,%2u) ", record->event.key.row, record->event.key.col);
  }
  xprintf("%-4s %-7s %s\n",  // "(tap|hold) (press|release) <keycode>".
      is_tap_hold ? (record->tap.count ? "tap" : "hold") : "",
      record->event.pressed ? "press" : "release",
      get_keycode_string(keycode));
}
#else
#pragma message "dlog_record: disabled"
#define dlog_record(keycode, record)
#endif  // NO_DEBUG
