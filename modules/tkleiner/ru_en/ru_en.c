#include "ru_en.h"
#include <stdbool.h>
#include "action_util.h"
#include "community_modules.h"
#include "keymap_russian.h"

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
#endif // NO_ACTION_ONESHOT
#ifdef LAYER_LOCK_ENABLE
        case QK_LLCK:
#endif // LAYER_LOCK_ENABLE
            return true;

#ifndef NO_ACTION_TAPPING
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            if (record->tap.count == 0) {
                return true;
            }
            break;
#endif // NO_ACTION_TAPPING
    }
    return false;
}

static ruen_language_t base_language    = RUEN_DEFAULT_LANGUAGE;
static ruen_language_t current_language = RUEN_DEFAULT_LANGUAGE;

static bool aux_english_active = false;
static bool ru_en_word_active  = false;

typedef struct {
    bool            active;
    ruen_language_t previous_language;
    ruen_language_t target_language;
#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
    bool layer_was_enabled;
#endif
} ruen_language_hold_t;

static ruen_language_hold_t language_hold = {
    .previous_language = RUEN_DEFAULT_LANGUAGE,
    .target_language   = RUEN_DEFAULT_LANGUAGE,
};

static ruen_language_t opposite_language(ruen_language_t language) {
    switch (language) {
        case RUEN_RU:
            return RUEN_EN;
        case RUEN_EN:
            return RUEN_RU;
    }
    return RUEN_DEFAULT_LANGUAGE;
}

static ruen_language_t intended_language(void) {
    return language_hold.active ? language_hold.target_language : base_language;
}

static ruen_language_t effective_language(void) {
    return aux_english_active ? RUEN_EN : intended_language();
}

static void tap_code16_without_active_mods(uint16_t keycode) {
    const uint8_t saved_mods      = get_mods();
    const uint8_t saved_weak_mods = get_weak_mods();
#ifndef NO_ACTION_ONESHOT
    const uint8_t saved_oneshot_mods = get_oneshot_mods();
#endif

    set_mods(0);
    clear_weak_mods();
#ifndef NO_ACTION_ONESHOT
    clear_oneshot_mods();
#endif
    send_keyboard_report();

    tap_code16(keycode);

    set_mods(saved_mods);
    set_weak_mods(saved_weak_mods);
#ifndef NO_ACTION_ONESHOT
    set_oneshot_mods(saved_oneshot_mods);
#endif
    send_keyboard_report();
}

#if !defined(RUEN_RU_LANGUAGE_KEY) || !defined(RUEN_EN_LANGUAGE_KEY)
static void tap_toggle_language_key(void) {
    const uint8_t saved_mods      = get_mods();
    const uint8_t saved_weak_mods = get_weak_mods();
#    ifndef NO_ACTION_ONESHOT
    const uint8_t saved_oneshot_mods = get_oneshot_mods();
#    endif

    set_mods(0);
    clear_weak_mods();
#    ifndef NO_ACTION_ONESHOT
    clear_oneshot_mods();
#    endif
    send_keyboard_report();

    register_code(RUEN_TOGGLE_LANGUAGE_MOD_KEY);
    register_code(RUEN_TOGGLE_LANGUAGE_KEY);
    unregister_code(RUEN_TOGGLE_LANGUAGE_KEY);
    unregister_code(RUEN_TOGGLE_LANGUAGE_MOD_KEY);

    set_mods(saved_mods);
    set_weak_mods(saved_weak_mods);
#    ifndef NO_ACTION_ONESHOT
    set_oneshot_mods(saved_oneshot_mods);
#    endif
    send_keyboard_report();
}
#endif

static void send_language_key(ruen_language_t language) {
    if (current_language == language) return;

    switch (language) {
        case RUEN_RU:
#ifdef RUEN_RU_LANGUAGE_KEY
            tap_code16_without_active_mods(RUEN_RU_LANGUAGE_KEY);
#else
            tap_toggle_language_key();
#endif
            current_language = RUEN_RU;
            break;
        case RUEN_EN:
#ifdef RUEN_EN_LANGUAGE_KEY
            tap_code16_without_active_mods(RUEN_EN_LANGUAGE_KEY);
#else
            tap_toggle_language_key();
#endif
            current_language = RUEN_EN;
            break;
    }
}

static void sync_effective_language(void) {
    send_language_key(effective_language());
}

#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
static bool language_layer_is_on(ruen_language_t language) {
    switch (language) {
#    ifdef RUEN_RU_LANGUAGE_LAYER
        case RUEN_RU:
            return layer_state_is(RUEN_RU_LANGUAGE_LAYER);
#    endif //* RUEN_RU_LANGUAGE_LAYER
#    ifdef RUEN_EN_LANGUAGE_LAYER
        case RUEN_EN:
            return layer_state_is(RUEN_EN_LANGUAGE_LAYER);
#    endif //* RUEN_EN_LANGUAGE_LAYER
    }
    return false;
}

static void language_layer_on(ruen_language_t language) {
    switch (language) {
#    ifdef RUEN_RU_LANGUAGE_LAYER
        case RUEN_RU:
            layer_on(RUEN_RU_LANGUAGE_LAYER);
            return;
#    endif //* RUEN_RU_LANGUAGE_LAYER
#    ifdef RUEN_EN_LANGUAGE_LAYER
        case RUEN_EN:
            layer_on(RUEN_EN_LANGUAGE_LAYER);
            return;
#    endif //* RUEN_EN_LANGUAGE_LAYER
    }
}

static void language_layer_off(ruen_language_t language) {
    switch (language) {
#    ifdef RUEN_RU_LANGUAGE_LAYER
        case RUEN_RU:
            layer_off(RUEN_RU_LANGUAGE_LAYER);
            return;
#    endif //* RUEN_RU_LANGUAGE_LAYER
#    ifdef RUEN_EN_LANGUAGE_LAYER
        case RUEN_EN:
            layer_off(RUEN_EN_LANGUAGE_LAYER);
            return;
#    endif //* RUEN_EN_LANGUAGE_LAYER
    }
}

static void set_default_language_layer(ruen_language_t language) {
    switch (language) {
#    ifdef RUEN_RU_LANGUAGE_LAYER
        case RUEN_RU:
            if (get_highest_layer(default_layer_state) != RUEN_RU_LANGUAGE_LAYER) {
                default_layer_set(1UL << RUEN_RU_LANGUAGE_LAYER);
            }
            return;
#    endif //* RUEN_RU_LANGUAGE_LAYER
#    ifdef RUEN_EN_LANGUAGE_LAYER
        case RUEN_EN:
            if (get_highest_layer(default_layer_state) != RUEN_EN_LANGUAGE_LAYER) {
                default_layer_set(1UL << RUEN_EN_LANGUAGE_LAYER);
            }
            return;
#    endif //* RUEN_EN_LANGUAGE_LAYER
    }
}

static layer_state_t language_layer_mask(void) {
    layer_state_t mask = 0;
#    ifdef RUEN_RU_LANGUAGE_LAYER
    mask |= (layer_state_t)1 << RUEN_RU_LANGUAGE_LAYER;
#    endif //* RUEN_RU_LANGUAGE_LAYER
#    ifdef RUEN_EN_LANGUAGE_LAYER
    mask |= (layer_state_t)1 << RUEN_EN_LANGUAGE_LAYER;
#    endif //* RUEN_EN_LANGUAGE_LAYER
    return mask;
}

static bool has_aux_english_layer(layer_state_t state) {
    return (state & ~language_layer_mask()) != 0;
}
#endif //* defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)

static void toggle_language(void) {
    base_language = opposite_language(base_language);
#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
    set_default_language_layer(base_language);
#endif
    sync_effective_language();
}

static void set_language(ruen_language_t language) {
    base_language = language;
#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
    set_default_language_layer(language);
#endif
    sync_effective_language();
}

static void set_language_register(ruen_language_t language) {
    if (language_hold.active) return;

    language_hold.active            = true;
    language_hold.previous_language = intended_language();
    language_hold.target_language   = language;

#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
    language_hold.layer_was_enabled = language_layer_is_on(language);
    if (!language_hold.layer_was_enabled) {
        language_layer_on(language);
    }
#endif
    sync_effective_language();
}

static void set_language_unregister(void) {
    if (!language_hold.active) return;

    const ruen_language_t previous_language = language_hold.previous_language;
    const ruen_language_t target_language   = language_hold.target_language;
#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
    if (!language_hold.layer_was_enabled) {
        language_layer_off(target_language);
    }
    language_hold.layer_was_enabled = false;
#endif
    language_hold.active            = false;
    language_hold.previous_language = previous_language;
    language_hold.target_language   = previous_language;
    sync_effective_language();
}

static void ru_en_word_on(void) {
    if (!ru_en_word_active && !language_hold.active) {
        set_language_register(opposite_language(intended_language()));
        ru_en_word_active = true;
    }
}

static void ru_en_word_off(void) {
    if (ru_en_word_active) {
        set_language_unregister();
        ru_en_word_active = false;
    }
}

// Ручной tap/hold для SET_RU/SET_EN: кастомные кейкоды не получают tap.count от tapping-движка.
static struct {
    uint16_t keycode;     // SET_RU/SET_EN, удерживаемый сейчас, или KC_NO
    uint16_t press_time;  // record->event.time нажатия
    bool     interrupted; // была ли нажата другая клавиша во время удержания
} language_key = {.keycode = KC_NO};

static void language_key_press(uint16_t keycode, ruen_language_t target, keyrecord_t *record) {
    if (language_key.keycode != KC_NO) return; // второй SET_* во время удержания игнорируем
    if (ru_en_word_active) ru_en_word_off();   // явная смена языка завершает режим слова
    language_key.keycode     = keycode;
    language_key.press_time  = record->event.time;
    language_key.interrupted = false;
    set_language_register(target); // моментальный эффект сразу при нажатии
}

static void language_key_release(uint16_t keycode, ruen_language_t target, keyrecord_t *record) {
    if (language_key.keycode != keycode) return;
    const bool tapped = !language_key.interrupted && TIMER_DIFF_16(record->event.time, language_key.press_time) < RUEN_TAPPING_TERM;
    // Tap: set_language до unregister — язык уже целевой, sync внутри unregister становится no-op (нет двойной отправки аккорда).
    if (tapped) set_language(target);
    set_language_unregister();
    language_key.keycode = KC_NO;
}

static void process_ru_en_word(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return;
    switch (keycode) {
        case SET_RU:
        case SET_EN:
        case R_E_WRD:
        case TG_LNG:
            return;
    }
    // 1. Игнорируем удержание для Mod-Tap и Layer-Tap, если это не тап
    if (IS_QK_MOD_TAP(keycode) && record->tap.count == 0) return;
    if (IS_QK_LAYER_TAP(keycode) && record->tap.count == 0) return;
    // 2. Извлекаем чистый кейкод, если были применены базовые модификаторы типа Shift
    // (Например, чтобы Shift + 1 (восклицательный знак) или Shift + буквы обрабатывались корректно)
    uint16_t              clean_keycode = QK_MODS_GET_BASIC_KEYCODE(get_tap_keycode(keycode));
    const ruen_language_t word_language = language_hold.active ? language_hold.target_language : base_language;
    switch (word_language) {
        case RUEN_RU:
            switch (clean_keycode) {
                // Буквы и цифры удерживают слой всегда
                case RU_EF ... RU_YA:
                case RU_HA ... RU_HARD: // 'Х', 'Ъ' в русском
                case RU_ZHE ... RU_YU:  // 'Ж', 'Э', 'Ё', 'Б', 'Ю' в русском
                case RU_1 ... RU_0:
                case RU_MINS:
                case KC_BSPC:
                case KC_DEL:
                    return;
            }
            break;
        case RUEN_EN:
            switch (clean_keycode) {
                // Буквы и цифры удерживают слой всегда
                case KC_A ... KC_Z:
                case KC_1 ... KC_0:
                // Внутрисловные символы и удаление
                case KC_MINS:
                case KC_GRAVE:
                case KC_QUOT: // Апостроф (важен для английского, например: don't)
                case KC_BSPC: // Удаление символа при опечатке
                case KC_DEL:
                    return;
            }
            break;
        default:
            break;
    }
    // Все остальные клавиши (Пробел, Enter, стрелочки) вернут false и выключат режим слова
    ru_en_word_off();
    return;
}

ruen_language_t get_current_language(void) {
    return current_language;
}

#if defined(RUEN_RU_LANGUAGE_LAYER) || defined(RUEN_EN_LANGUAGE_LAYER)
layer_state_t layer_state_set_ru_en(layer_state_t state) {
    state              = layer_state_set_ru_en_kb(state);
    aux_english_active = has_aux_english_layer(state);
    sync_effective_language();
    return state;
}
#endif

void keyboard_post_init_ru_en(void) {
    keyboard_post_init_ru_en_kb();
    set_language(RUEN_DEFAULT_LANGUAGE);
}

// Сканирование word-режима остаётся в pre_process: layer_shift_keys стоит раньше ru_en
// и возвращает false при ремапе — до process_record_ru_en такие клавиши не дошли бы.
// Известное ограничение: record->tap.count здесь всегда 0, поэтому тапы MT/LT не сканируются.
bool pre_process_record_ru_en(uint16_t keycode, keyrecord_t *record) {
    if (!pre_process_record_ru_en_kb(keycode, record)) return false;
    if (is_modifier_or_layer_key(keycode, record)) return true;
    if (ru_en_word_active && record->event.pressed) process_ru_en_word(keycode, record);
    return true;
}

bool process_record_ru_en(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_ru_en_kb(keycode, record)) return false;
    if (record->event.pressed && language_key.keycode != KC_NO && keycode != language_key.keycode) language_key.interrupted = true;
    if (is_modifier_or_layer_key(keycode, record)) return true;
    switch (keycode) {
        case SET_RU:
            if (record->event.pressed)
                language_key_press(keycode, RUEN_RU, record);
            else
                language_key_release(keycode, RUEN_RU, record);
            return false;
        case SET_EN:
            if (record->event.pressed)
                language_key_press(keycode, RUEN_EN, record);
            else
                language_key_release(keycode, RUEN_EN, record);
            return false;
        case R_E_WRD:
            if (record->event.pressed) {
                if (ru_en_word_active == false)
                    ru_en_word_on();
                else
                    ru_en_word_off();
            }
            return false;
        case TG_LNG:
            if (record->event.pressed) {
                toggle_language();
            }
            return false;
    }
    return true;
}
