# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

QMK **external userspace** (fork of qmk/qmk_userspace) for a single keyboard: Ergohaven K03 (RP2040 split), keymap `keyboards/ergohaven/k03/keymaps/tick_kleiner`.

## Build & verify

- Verify any change compiles: `qmk compile -kb ergohaven/k03/rev3 -km tick_kleiner` (the firmware checkout exposes only `ergohaven/k03/rev3`; plain `ergohaven/k03` is not a valid target)
- Requires one-time setup (`qmk config user.qmk_home=<path-to-qmk_firmware>` and `qmk config user.overlay_dir="$(realpath .)"`); the root Makefile errors if `user.qmk_home` is unset.
- `modules/getreuer` is a git submodule — if its sources are missing, run `git submodule update --init`.
- **Never flash.** Firmware is flashed manually by drag-dropping the `.uf2` onto the RP2040 drive from Windows. Do not run `qmk flash`.

## Architecture

- `src/` is the shared userspace library: the keymap's `rules.mk` includes `src/rules.mk`, its `config.h` includes `src/config.h`. Main logic lives in `src/tick_kleiner.c`.
- `modules/tkleiner/` — first-party QMK community modules; `modules/getreuer/` — third-party (submodule).
- Enabling a module takes **two synced edits**: add it to `"modules"` in the keymap's `keymap.json` AND set `COMMUNITY_MODULE_<NAME>_ENABLE = yes` in the keymap's `rules.mk` (defaults are `?= no` in `src/rules.mk`). Module-dependent code is guarded by `#ifdef COMMUNITY_MODULE_<NAME>_ENABLE`.
- Module contract: `qmk_module.json`, main `.c`/`.h`, and `introspection.c`/`.h` exposing weak `_count()`/`_get()` accessors over a keymap-defined array (e.g. `layer_shift_keys[]` in `keymap.c`).
- Flow Tap (both QMK core Flow Tap and the community Tap Flow module) is a forbidden architectural dependency. Its rhythm-dependent eager-tap decisions are incompatible with the user's uneven typing style; do not enable or propose it again. Keep Speculative Hold disabled as well. The stable baseline is Chordal Hold + Permissive Hold with a permanent 6KRO report path (`NKRO_ENABLE = no`) on every layer, including GAM.

## Refactoring in progress

Goal: move as much logic as possible out of `src/tick_kleiner.c` into modules under `modules/tkleiner/` to decompose and simplify maintenance of the keymap:

- `layer_shift_keys` (replaces `getreuer/custom_shift_keys`) — per-layer shift overrides; needed because the RU layer requires different shift mappings than EN. **Done.**
- `ru_en` — layout-switching module driving the EN/RU layers and sending the OS layout-switch chords Ctrl+1/Ctrl+2 (Windows). **Done.** SET_RU/SET_EN: tap = permanent switch, hold = momentary language (manual tap/hold via `record->event.time`, term `RUEN_TAPPING_TERM`).
- `custom_keys` — planned generic rule-based remapping engine; currently a header-only scaffold (`custom_keys.c` is empty, not wired into `keymap.json`/`rules.mk` yet).

## Code style

- `.clang-format` (Google-based, 4-space indent, `ColumnLimit: 1000` — no line wrapping, aligned consecutive assignments/declarations). `SortIncludes: false` — do not reorder includes.
- `*.mk` and `Makefile` use tabs (see `.editorconfig`).
- Custom keycodes are SCREAMING_CASE; module APIs are prefixed with the module name (`ruen_`, `layer_shift_keys_`).
