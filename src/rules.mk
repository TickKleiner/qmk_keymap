#[HARDWARE FEATURES]
CONSOLE_ENABLE				?= no #(https://docs.qmk.fm/config_options)
VIA_ENABLE					?= no #(https://docs.qmk.fm/config_options)
LTO_ENABLE 					?= no #(https://docs.qmk.fm/config_options)

ENCODER_MAP_ENABLE			?= no #(https://docs.qmk.fm/features/encoders)
DIP_SWITCH_ENABLE			?= no #(https://docs.qmk.fm/features/dip_switch)
BOOTMAGIC_ENABLE			?= no #(https://docs.qmk.fm/features/bootmagic)
RGBLIGHT_ENABLE				?= no #(http://docs.qmk.fm/features/rgblight)
ENCODER_ENABLE				?= no #(https://docs.qmk.fm/features/encoders)
STENO_ENABLE				?= no #(https://docs.qmk.fm/features/stenography)

#[SOFTWARE FEATURES]
DYNAMIC_TAPPING_TERM_ENABLE	?= no #(https://docs.qmk.fm/tap_hold)
DYNAMIC_KEYMAP_ENABLE		?= no
KEY_OVERRIDE_ENABLE			?= no #https://docs.qmk.fm/features/key_overrides
SEND_STRING_ENABLE			?= no #(https://docs.qmk.fm/features/send_string)
AUTOCORRECT_ENABLE			?= no #(https://docs.qmk.fm/features/autocorrect)
LAYER_LOCK_ENABLE			?= no #(https://docs.qmk.fm/features/layer_lock)
AUTO_SHIFT_ENABLE			?= no #(https://docs.qmk.fm/features/auto_shift)
CAPS_WORD_ENABLE			?= no #(https://docs.qmk.fm/features/caps_word)
TAP_DANCE_ENABLE			?= no #(https://docs.qmk.fm/features/tap_dance)
TRI_LAYER_ENABLE			?= no #(https://docs.qmk.fm/features/tri_layer)
KEY_LOCK_ENABLE				?= no #(https://docs.qmk.fm/features/key_lock)
COMBO_ENABLE				?= no #(https://docs.qmk.fm/features/combo)

OS_DETECTION_ENABLE			?= no #(https://docs.qmk.fm/features/os_detection)
SWAP_HANDS_ENABLE			?= no #(https://docs.qmk.fm/features/swap_hands)
SECURE_ENABLE				?= no #(https://docs.qmk.fm/features/secure)
RAW_ENABLE					?= no #(https://docs.qmk.fm/features/rawhid)

UNICODEMAP_ENABLE			?= no #(https://docs.qmk.fm/features/unicode)
UNICODE_ENABLE				?= no #(https://docs.qmk.fm/features/unicode)
UNICODE_COMMON				?= no #(https://docs.qmk.fm/features/unicode)
UCIS_ENABLE					?= no #(https://docs.qmk.fm/features/unicode)

DIGITIZER_ENABLE			?= no #(https://docs.qmk.fm/features/digitizer)
SEQUENCER_ENABLE			?= no #(https://docs.qmk.fm/features/sequencer)

#[ADVANCED KEYCODES]
REPEAT_KEY_ENABLE			?= no #(https://docs.qmk.fm/features/repeat_key)
EXTRAKEY_ENABLE				?= no #(https://docs.qmk.fm/config_options)
MAGIC_ENABLE				?= no #(https://docs.qmk.fm/keycodes_magic)
MOUSEKEY_ENABLE				?= no #(https://docs.qmk.fm/features/mouse_keys)
COMMAND_ENABLE				?= no #(https://docs.qmk.fm/features/command)
GRAVE_ESC_ENABLE			?= no #(https://docs.qmk.fm/features/grave_esc)
SPACE_CADET_ENABLE			?= no #(https://docs.qmk.fm/features/space_cadet)
DYNAMIC_MACRO_ENABLE		?= no #(https://docs.qmk.fm/features/dynamic_macros)
PROGRAMMABLE_BUTTON_ENABLE	?= no #(https://docs.qmk.fm/features/programmable_button)
LEADER_ENABLE				?= no #(https://docs.qmk.fm/features/leader_key)
MACROS_ENABLED				?= no #(https://docs.qmk.fm/feature_macros)

#[EXTRA FEATURES]
#[COMMUNITY MODULES]
COMMUNITY_MODULE_LAYER_SHIFT_KEYS_ENABLE	?= no
COMMUNITY_MODULE_KEYCODE_STRING_ENABLE		?= no
COMMUNITY_MODULE_ORBITAL_MOUSE_ENABLE		?= no
COMMUNITY_MODULE_SENTENCE_CASE_ENABLE		?= no
COMMUNITY_MODULE_SELECT_WORD_ENABLE			?= no
COMMUNITY_MODULE_SPECULATIVE_HOLD_ENABLE	?= no

COMMUNITY_MODULE_RU_EN_ENABLE				?= no

SRC += src/tick_kleiner.c
SRC += src/features/tap_hold.c
SRC += src/utils/global_state.c
SRC += src/features/keycode_string.c


ifeq ($(strip $(AUTOCORRECT_ENABLE)), yes)
	OPT_DEFS += -DAUTOCORRECT_ENABLE
	SRC += src/features/autocorrect.c
endif
ifeq ($(strip $(CAPS_WORD_ENABLE)), yes)
	OPT_DEFS += -DCAPS_WORD_ENABLE
	SRC += src/features/caps_word.c
endif
ifeq ($(strip $(REPEAT_KEY_ENABLE)), yes)
	OPT_DEFS += -DREPEAT_KEY_ENABLE
	SRC += src/features/repeat_key.c
endif

#[EXTRA FEATURES]
#[COMMUNITY MODULES]
ifeq ($(strip $(COMMUNITY_MODULE_ORBITAL_MOUSE_ENABLE)), yes)
	MOUSE_ENABLE = yes
endif
