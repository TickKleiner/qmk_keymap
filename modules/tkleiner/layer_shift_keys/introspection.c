#ifdef COMMUNITY_MODULE_LAYER_SHIFT_KEYS_ENABLE

uint16_t layer_shift_keys_count_raw(void) {
    return ARRAY_SIZE(layer_shift_keys);
}

__attribute__((weak)) uint16_t layer_shift_keys_count(void) {
    return layer_shift_keys_count_raw();
}

const layer_shift_key_t *layer_shift_keys_get_raw(uint16_t index) {
    if (index >= layer_shift_keys_count_raw()) {
        return NULL;
    }
    return &layer_shift_keys[index];
}

__attribute__((weak)) const layer_shift_key_t *layer_shift_keys_get(uint16_t index) {
    return layer_shift_keys_get_raw(index);
}

#endif  // COMMUNITY_MODULE_LAYER_SHIFT_KEYS_ENABLE
