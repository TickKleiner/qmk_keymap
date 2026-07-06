///////////////////////////////////////////////////////////////////////////////
// Debug logging
///////////////////////////////////////////////////////////////////////////////
#if !defined(NO_DEBUG) && defined(COMMUNITY_MODULE_KEYCODE_STRING_ENABLE)
#    include "src/tick_kleiner.h"
#    include "src/keycodes.h"

#    pragma message "dlog_record: enabled"

#    ifdef COMMUNITY_MODULE_RU_EN_ENABLE
#        define RUEN_KEYCODE_STRING_NAMES KEYCODE_STRING_NAME(SET_RU), KEYCODE_STRING_NAME(SET_EN), KEYCODE_STRING_NAME(R_E_WRD), KEYCODE_STRING_NAME(TG_LNG)
#    else
#        define RUEN_KEYCODE_STRING_NAMES
#    endif // COMMUNITY_MODULE_RU_EN_ENABLE

KEYCODE_STRING_NAMES_USER(KEYCODE_STRING_NAME(UPDIR), KEYCODE_STRING_NAME(TMUXESC), KEYCODE_STRING_NAME(SRCHSEL), KEYCODE_STRING_NAME(NLK_HLD), KEYCODE_STRING_NAME(SLK_HLD), KEYCODE_STRING_NAME(M_THE), KEYCODE_STRING_NAME(M_ION), KEYCODE_STRING_NAME(M_MENT), KEYCODE_STRING_NAME(M_QUEN), KEYCODE_STRING_NAME(M_TMENT), KEYCODE_STRING_NAME(M_UPDIR), KEYCODE_STRING_NAME(M_INCLUDE), KEYCODE_STRING_NAME(M_EQEQ), KEYCODE_STRING_NAME(M_NBSP), KEYCODE_STRING_NAME(M_MKGRVS), KEYCODE_STRING_NAME(M_DOCSTR), KEYCODE_STRING_NAME(M_NOOP), KEYCODE_STRING_NAME(SELWORD), KEYCODE_STRING_NAME(SELWBAK), KEYCODE_STRING_NAME(SELLINE), RUEN_KEYCODE_STRING_NAMES);

void dlog_record(uint16_t keycode, keyrecord_t* record) {
    if (!debug_enable) {
        return;
    }
    uint8_t layer       = read_source_layers_cache(record->event.key);
    bool    is_tap_hold = IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode);
    xprintf("L%-2u ", layer);           // Log the layer.
    if (IS_COMBOEVENT(record->event)) { // Combos don't have a position.
        xprintf("combo   ");
    } else { // Log the "(row,col)" position.
        xprintf("(%2u,%2u) ", record->event.key.row, record->event.key.col);
    }
    xprintf("%-4s %-7s %s\n", // "(tap|hold) (press|release) <keycode>".
            is_tap_hold ? (record->tap.count ? "tap" : "hold") : "", record->event.pressed ? "press" : "release", get_keycode_string(keycode));
}
#else
#    pragma message "dlog_record: disabled"
#endif // NO_DEBUG
