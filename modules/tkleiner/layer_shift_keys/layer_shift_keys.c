#include "layer_shift_keys.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

#ifndef LAYER_SHIFT_KEYS_NEGMODS
#    define LAYER_SHIFT_KEYS_NEGMODS 0
#endif

// Defined in introspection.c.
uint16_t layer_shift_keys_count(void);
const layer_shift_key_t *layer_shift_keys_get(uint16_t index);

static uint16_t registered_keycode = KC_NO;

static inline bool is_tap_hold_key(uint16_t keycode) {
    return IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode);
}

static inline uint8_t active_mods(void) {
#ifndef NO_ACTION_ONESHOT
    return get_mods() | get_weak_mods() | get_oneshot_mods();
#else
    return get_mods() | get_weak_mods();
#endif
}

static void register_without_shift(uint16_t keycode) {
    const uint8_t saved_mods      = get_mods();
    const uint8_t saved_weak_mods = get_weak_mods();
#ifndef NO_ACTION_ONESHOT
    const uint8_t saved_oneshot_mods = get_oneshot_mods();
#endif

    del_mods(MOD_MASK_SHIFT);
    del_weak_mods(MOD_MASK_SHIFT);
#ifndef NO_ACTION_ONESHOT
    del_oneshot_mods(MOD_MASK_SHIFT);
#endif

    register_code16(keycode);

    set_mods(saved_mods);
    set_weak_mods(saved_weak_mods);
#ifndef NO_ACTION_ONESHOT
    set_oneshot_mods(saved_oneshot_mods);
#endif
}

static bool try_register_key(uint16_t keycode) {
    if (keycode == KC_NO) {
        return true;
    }

    registered_keycode = keycode;
    register_without_shift(registered_keycode);
    return false;
}

bool process_record_layer_shift_keys(uint16_t keycode, keyrecord_t *record) {
    if (registered_keycode != KC_NO) {
        unregister_code16(registered_keycode);
        registered_keycode = KC_NO;
    }

    if (!record->event.pressed) {
        return true;
    }

    if (is_tap_hold_key(keycode) && record->tap.count == 0) {
        return true;
    }

    const uint8_t mods = active_mods();

#if LAYER_SHIFT_KEYS_NEGMODS != 0
    if ((mods & LAYER_SHIFT_KEYS_NEGMODS) != 0) {
        return true;
    }
#endif

    const uint8_t       layer      = read_source_layers_cache(record->event.key);
    const layer_state_t layer_mask = (layer_state_t)1 << layer;
    const bool          shifted    = (mods & MOD_MASK_SHIFT) != 0;

    for (uint16_t i = 0; i < layer_shift_keys_count(); ++i) {
        const layer_shift_key_t *entry = layer_shift_keys_get(i);
        if (entry == NULL || keycode != entry->trigger || (entry->layers & layer_mask) == 0) {
            continue;
        }

        return try_register_key(shifted ? entry->shifted_keycode : entry->unshifted_keycode);
    }

    return true;
}
