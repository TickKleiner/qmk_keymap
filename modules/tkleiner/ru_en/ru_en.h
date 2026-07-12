#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RUEN_RU,
    RUEN_EN,
} ruen_language_t;

#ifndef RUEN_DEFAULT_LANGUAGE
#    pragma message "RUEN_DEFAULT_LANGUAGE: not set"
#    define RUEN_DEFAULT_LANGUAGE RUEN_RU
#endif //* EN_LANGUAGE_LAYER

#ifndef RUEN_RU_LANGUAGE_LAYER
#    pragma message "RUEN_RU_LANGUAGE_LAYER: not set"
#endif //* RUEN_RU_LANGUAGE_LAYER
#ifndef RUEN_RU_LANGUAGE_KEY
#    pragma message "RUEN_RU_LANGUAGE_KEY: not set"
#endif //* RUEN_RU_LANGUAGE_KEY

#ifndef RUEN_EN_LANGUAGE_LAYER
#    pragma message "RUEN_EN_LANGUAGE_LAYER: not set"
#endif //* RUEN_EN_LANGUAGE_LAYER
#ifndef RUEN_EN_LANGUAGE_KEY
#    pragma message "RUEN_EN_LANGUAGE_KEY: not set"
#endif //* RUEN_EN_LANGUAGE_KEY

#ifndef RUEN_TOGGLE_LANGUAGE_MOD_KEY
#    pragma message "RUEN_TOGGLE_LANGUAGE_MOD_KEY: not_set"
#    define RUEN_TOGGLE_LANGUAGE_MOD_KEY KC_LALT
#endif //* RUEN_TOGGLE_LANGUAGE_MOD_KEY
#ifndef RUEN_TOGGLE_LANGUAGE_KEY
#    pragma message "RUEN_TOGGLE_LANGUAGE_KEY: not_set"
#    define RUEN_TOGGLE_LANGUAGE_KEY KC_LSFT
#endif //* RUEN_TOGGLE_LANGUAGE_KEY

ruen_language_t get_current_language(void);

#ifdef __cplusplus
}
#endif
