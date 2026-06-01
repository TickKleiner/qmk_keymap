#include "global_state.h"

global_state_t global_state = {};
const global_state_t *GLOBAL_STATE = &global_state;

bool pre_process_record_set_state_user(uint16_t keycode, keyrecord_t *record) {
    global_state.mods = get_mods();
    global_state.weak_mods = get_weak_mods();
#ifndef NO_ACTION_ONESHOT
    global_state.oneshot_mods = get_oneshot_mods();
#endif  //* NO_ACTION_ONESHOT
    global_state.all_mods = (global_state.mods | global_state.weak_mods
#ifndef NO_ACTION_ONESHOT
        | global_state.oneshot_mods
#endif  //* NO_ACTION_ONESHOT
    );
    global_state.layer = read_source_layers_cache(record->event.key);
    global_state.home_held[record->event.key.row][record->event.key.col] = record->event.pressed;
    return true;
}
