#pragma once

#include "quantum.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CK_ACTION_NONE = 0,
    CK_ACTION_KEYCODE,
    CK_ACTION_MODS,
    CK_ACTION_LAYER,
    CK_ACTION_CALLBACK,
} custom_key_action_type_t;

typedef struct {
    custom_key_action_type_t type;

    union {
        uint16_t keycode;
        uint8_t  mods;
        uint8_t  layer;

        struct {
            void (*on_press)(keyrecord_t *record);
            void (*on_release)(keyrecord_t *record);
        } callback;
    };
} custom_key_action_t;

#define CK_NO_ACTION \
    ((custom_key_action_t){ .type = CK_ACTION_NONE })

#define CK_KEY(kc) \
    ((custom_key_action_t){ .type = CK_ACTION_KEYCODE, .keycode = (kc) })

#define CK_MODS(mod_mask) \
    ((custom_key_action_t){ .type = CK_ACTION_MODS, .mods = (mod_mask) })

#define CK_LAYER(layer_id) \
    ((custom_key_action_t){ .type = CK_ACTION_LAYER, .layer = (layer_id) })

#define CK_CALLBACK(press_fn, release_fn) \
    ((custom_key_action_t){                     \
        .type = CK_ACTION_CALLBACK,             \
        .callback = {                            \
            .on_press = (press_fn),              \
            .on_release = (release_fn),          \
        },                                       \
    })

typedef struct {
    uint16_t trigger;

    // 0 means all layers.
    layer_state_t layer_mask;

    uint8_t required_mods;
    uint8_t negative_mods;

    uint8_t suppressed_mods;
    uint8_t added_mods;
} custom_key_condition_t;

typedef struct {
    // Variant A: immediate replacement behavior.
    custom_key_action_t press;
    custom_key_action_t release;

    // Future variant C: tap-hold behavior.
    custom_key_action_t tap;
    custom_key_action_t hold;
    custom_key_action_t tap_hold;
} custom_key_behavior_t;

typedef enum {
    CK_FLAG_NONE = 0,

    CK_FLAG_MATCH_SOURCE_LAYER       = 1u << 0,
    CK_FLAG_USE_ONESHOT_MODS         = 1u << 1,
    CK_FLAG_USE_WEAK_MODS            = 1u << 2,
    CK_FLAG_RELEASE_ON_ROLLOVER      = 1u << 3,

    // Reserved for future tap-hold state machine.
    CK_FLAG_HOLD_ON_OTHER_KEY_PRESS  = 1u << 4,
    CK_FLAG_PERMISSIVE_HOLD          = 1u << 5,
    CK_FLAG_RETRO_TAPPING            = 1u << 6,

    CK_FLAG_CONSUME_TRIGGER          = 1u << 7,
} custom_key_flags_t;

typedef struct {
    custom_key_condition_t condition;
    custom_key_behavior_t  behavior;

    uint16_t tapping_term;
    uint8_t  flags;
} custom_key_rule_t;

#define CK_REPLACE(trigger_key, layer_mask_, req_mods, neg_mods, suppress_mods_, replacement_key) \
    {                                                                                             \
        .condition = {                                                                            \
            .trigger         = (trigger_key),                                                      \
            .layer_mask      = (layer_mask_),                                                      \
            .required_mods   = (req_mods),                                                         \
            .negative_mods   = (neg_mods),                                                         \
            .suppressed_mods = (suppress_mods_),                                                   \
            .added_mods      = 0,                                                                  \
        },                                                                                        \
        .behavior = {                                                                             \
            .press    = CK_KEY(replacement_key),                                                   \
            .release  = CK_KEY(replacement_key),                                                   \
            .tap      = CK_NO_ACTION,                                                              \
            .hold     = CK_NO_ACTION,                                                              \
            .tap_hold = CK_NO_ACTION,                                                              \
        },                                                                                        \
        .tapping_term = CUSTOM_KEYS_DEFAULT_TAPPING_TERM,                                          \
        .flags = CK_FLAG_MATCH_SOURCE_LAYER | CK_FLAG_USE_ONESHOT_MODS | CK_FLAG_CONSUME_TRIGGER,  \
    }

typedef enum {
    CK_STATE_IDLE = 0,
    CK_STATE_ACTIVE,
    CK_STATE_PENDING,
    CK_STATE_HOLD_ACTIVE,
    CK_STATE_TAP_SENT,
    CK_STATE_CANCELLED,
} custom_key_state_t;

typedef struct {
    custom_key_state_t state;

    const custom_key_rule_t *rule;

    keypos_t key;
    uint16_t trigger;
    uint16_t timer;

    uint8_t source_layer;

    uint8_t mods;
    uint8_t weak_mods;
    uint8_t oneshot_mods;
    uint8_t effective_mods;

    uint8_t suppressed_mods;
    uint8_t added_mods;

    custom_key_action_t active_action;

#if defined(DEFERRED_EXEC_ENABLE)
    deferred_token timeout_token;
#endif
} custom_key_active_t;

extern const custom_key_rule_t custom_key_rules[];
extern const uint16_t custom_key_rules_count;

bool process_custom_keys(uint16_t keycode, keyrecord_t *record);

#ifdef __cplusplus
}
#endif
