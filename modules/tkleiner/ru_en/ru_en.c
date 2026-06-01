#include "ru_en.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

static inline bool is_modifier_or_layer_key(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MODIFIER_KEYCODE_RANGE:
        case QK_MOMENTARY ... QK_MOMENTARY_MAX:
        case QK_LAYER_MOD ... QK_LAYER_MOD_MAX:
        case QK_LAYER_TAP_TOGGLE ... QK_LAYER_TAP_TOGGLE_MAX:
        case QK_TO ... QK_TO_MAX:
        case QK_TOGGLE_LAYER ... QK_TOGGLE_LAYER_MAX:
#ifndef NO_ACTION_ONESHOT
        case QK_ONE_SHOT_LAYER ... QK_ONE_SHOT_LAYER_MAX:
        case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
#endif  // NO_ACTION_ONESHOT
#ifdef LAYER_LOCK_ENABLE
        case QK_LLCK:
#endif  // LAYER_LOCK_ENABLE
            return true;

#ifndef NO_ACTION_TAPPING
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            if (record->tap.count == 0) {
                return true;
            }
            break;
#endif  // NO_ACTION_TAPPING
    }
    return false;
}

static bool language_hold = false;
static bool ru_en_word_active = false;
static ruen_language_t current_language = RUEN_DEFAULT_LANGUAGE;
#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
static bool layer_hold = false;
#endif

void toggle_language(void) {
    register_code(RUEN_TOGGLE_LANGUAGE_MOD_KEY);
    register_code(RUEN_TOGGLE_LANGUAGE_KEY);
    unregister_code(RUEN_TOGGLE_LANGUAGE_KEY);
    unregister_code(RUEN_TOGGLE_LANGUAGE_MOD_KEY);
    switch (current_language) {
        case RUEN_RU:
            current_language = RUEN_EN;
            break;
        case RUEN_EN:
            current_language = RUEN_RU;
            break;
    }
}
void toggle_language_register(void) {
    register_code(RUEN_TOGGLE_LANGUAGE_MOD_KEY);
    register_code(RUEN_TOGGLE_LANGUAGE_KEY);
}
void toggle_language_unregister(void) {
    unregister_code(RUEN_TOGGLE_LANGUAGE_KEY);
    unregister_code(RUEN_TOGGLE_LANGUAGE_MOD_KEY);
    switch (current_language) {
        case RUEN_RU:
            current_language = RUEN_EN;
            break;
        case RUEN_EN:
            current_language = RUEN_RU;
            break;
    }
}

void set_language(ruen_language_t language) {
    switch (language) {
        case RUEN_RU:
            if (current_language != RUEN_RU)
            {
#ifdef RUEN_RU_LANGUAGE_KEY
                tap_code16(RUEN_RU_LANGUAGE_KEY);
                current_language = RUEN_RU;
#else
                toggle_language();
#endif
            }
#ifdef RUEN_RU_LANGUAGE_LAYER
            if (get_highest_layer(default_layer_state) != RUEN_RU_LANGUAGE_LAYER) {
                default_layer_set(1UL << RUEN_RU_LANGUAGE_LAYER);
            }
#endif
            return;
        case RUEN_EN:
            if (current_language != RUEN_EN) {
#ifdef RUEN_EN_LANGUAGE_KEY
                tap_code16(RUEN_EN_LANGUAGE_KEY);
                current_language = RUEN_EN;
#else
                toggle_language();
#endif
            }
#ifdef RUEN_EN_LANGUAGE_LAYER
            if (get_highest_layer(default_layer_state) != RUEN_EN_LANGUAGE_LAYER) {
                default_layer_set(1UL << RUEN_EN_LANGUAGE_LAYER);
            }
#endif
            return;
    }
}
void set_language_register(ruen_language_t language) {
    switch (language) {
        case RUEN_RU:
            if (current_language != RUEN_RU)
            {
#ifdef RUEN_RU_LANGUAGE_KEY
                tap_code16(RUEN_RU_LANGUAGE_KEY);
#else
                toggle_language();
#endif
                current_language = RUEN_RU;
                language_hold = true;
            }
#ifdef RUEN_RU_LANGUAGE_LAYER
            if (get_highest_layer(layer_state) != RUEN_RU_LANGUAGE_LAYER) {
                layer_on(RUEN_RU_LANGUAGE_LAYER);
                layer_hold = true;
            }
#endif
            return;
        case RUEN_EN:
            if (current_language != RUEN_EN)
            {
#ifdef RUEN_EN_LANGUAGE_KEY
                tap_code16(RUEN_EN_LANGUAGE_KEY);
#else
                toggle_language();
#endif
                current_language = RUEN_EN;
                language_hold = true;
            }
#ifdef RUEN_EN_LANGUAGE_LAYER
            if (get_highest_layer(layer_state) != RUEN_EN_LANGUAGE_LAYER) {
                layer_on(RUEN_EN_LANGUAGE_LAYER);
                layer_hold = true;
            }
#endif
            return;
    }
}
void set_language_unregister(void) {
    switch (current_language) {
        case RUEN_RU:
            if (language_hold == true) {
#ifdef RUEN_EN_LANGUAGE_KEY
                tap_code16(RUEN_EN_LANGUAGE_KEY);
#else
                toggle_language();
#endif
                current_language = RUEN_EN;
                language_hold = false;
            }
#ifdef RUEN_RU_LANGUAGE_LAYER
            if (layer_hold == true) {
                layer_off(RUEN_RU_LANGUAGE_LAYER);
                layer_hold = false;
            }
#endif
            return;
        case RUEN_EN:
            if (language_hold == true) {
#ifdef RUEN_RU_LANGUAGE_KEY
                tap_code16(RUEN_RU_LANGUAGE_KEY);
#else
                toggle_language();
#endif
                current_language = RUEN_RU;
                language_hold = false;
            }
#ifdef RUEN_EN_LANGUAGE_LAYER
            if (layer_hold == true) {
                layer_off(RUEN_EN_LANGUAGE_LAYER);
                layer_hold = false;
            }
#endif
            return;
    }
}

void ru_en_word_on(void) {
    if (!ru_en_word_active) {
        switch (current_language) {
            case RUEN_RU:
                set_language_register(RUEN_EN);
                ru_en_word_active = true;
                return;
            case RUEN_EN:
                set_language_register(RUEN_RU);
                ru_en_word_active = true;
                return;
        }
    }
}
void ru_en_word_off(void) {
    if (ru_en_word_active) {
        switch (current_language) {
            case RUEN_RU:
            case RUEN_EN:
                set_language_unregister();
                ru_en_word_active = false;
                return;
        }
    }
}
void process_ru_en_word(uint16_t keycode, keyrecord_t *record) {
    // 1. Игнорируем удержание для Mod-Tap и Layer-Tap, если это не тап
    if (IS_QK_MOD_TAP(keycode) && record->tap.count == 0) return;
    if (IS_QK_LAYER_TAP(keycode) && record->tap.count == 0) return;
    // 2. Извлекаем чистый кейкод, если были применены базовые модификаторы типа Shift
    // (Например, чтобы Shift + 1 (восклицательный знак) или Shift + буквы обрабатывались корректно)
    uint16_t clean_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    switch (clean_keycode) {
        // Буквы и цифры удерживают слой всегда
        case KC_A ... KC_Z:
        case KC_1 ... KC_0:
        // Внутрисловные символы и удаление
        case KC_MINS:
        case KC_UNDS:
        case KC_QUOT: // Апостроф (важен для английского, например: don't)
        case KC_BSPC: // Удаление символа при опечатке
        case KC_DEL:
            return;
        // Символы, которые ведут себя по-разному в зависимости от языка
        case KC_LBRC ... KC_RBRC: // Х, Ъ в русском
        case KC_SCLN:             // Ж в русском
        case KC_COMM ... KC_DOT:  // Б, Ю в русском
            if (current_language == RUEN_RU) {
                return; // На русском слое это буквы, они удерживают слой
            }
            // На английском слое это точки/запятые/скобки — они завершают слово
            break;
        default:
            break;
    }
    // Все остальные клавиши (Пробел, Enter, стрелочки) вернут false и выключат режим слова
    ru_en_word_off();
    return;
}

ruen_language_t get_current_language(void)
{
    return current_language;
}

void keyboard_post_init_ru_en(void) {
    keyboard_post_init_ru_en_kb();
    set_language(RUEN_DEFAULT_LANGUAGE);
}

bool pre_process_record_ru_en(uint16_t keycode, keyrecord_t* record) {
    if (!pre_process_record_ru_en_kb(keycode, record)) return false;
    if (ru_en_word_active) process_ru_en_word(keycode, record);
    return true;
}

bool process_record_ru_en(uint16_t keycode, keyrecord_t* record) {
    if (!process_record_ru_en_kb(keycode, record)) return false;
    if (is_modifier_or_layer_key(keycode, record)) return true;
    switch (keycode) {
        case SET_RU:
            if (record->event.pressed) {
                if (record->tap.count == 0) set_language(RUEN_RU);
                else set_language_register(RUEN_RU);
            } else {
                if (record->tap.count == 0) set_language_unregister();
            }
            return false;
        case SET_EN:
            if (record->event.pressed) {
                if (record->tap.count == 0) set_language(RUEN_EN);
                else set_language_register(RUEN_EN);
            } else {
                if (record->tap.count == 0) set_language_unregister();
            }
            return false;
        case R_E_WRD:
            if (record->event.pressed) {
                if (ru_en_word_active == false) ru_en_word_on();
                else ru_en_word_off();
            }
            return false;
        case TG_LNG:
            if (record->event.pressed) {
                if (record->tap.count == 0) toggle_language_register();
                else toggle_language();
            } else {
                if (record->tap.count == 0) toggle_language_unregister();
            }
            return false;
    }
    return true;
}
