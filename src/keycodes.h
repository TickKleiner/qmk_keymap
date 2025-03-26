#pragma once
#include "quantum.h"
#include "keymap_russian.h"

typedef enum {
    LNG_START,
    LNG_1      = LNG_START,
    LNG_2,
    LNG_END    = LNG_2,
} language_t;

enum layers {
    EN,
    RU,
    SYM,
    NAV,
    NUM,
    WIN,
    FUN,
    EXT,
    MAG,
    GAM,
    FAC,
};

// The "magic" key is the Alternate Repeat Key.
#define MAGIC QK_AREP

//[LAYERS]
#define TO_SYM  TO(SYM)
#define TO_NAV  TO(NAV)
#define TO_NUM  TO(NUM)
#define TO_WIN  TO(WIN)
#define TO_FUN  TO(FUN)
#define TO_EXT  TO(EXT)
#define TO_GAM  TO(GAM)
#define TG_GAM  TG(GAM)
#define TO_FAC  TO(FAC)
#define TG_FAC  TG(FAC)
#define DF_EN   DF(EN)
#define DF_RU   DF(RU)
#define TG_MAG  TG(MAG)
#define OSL_MAG OSL(MAG)

//[HRM_EN]
#define ALT_S   LALT_T(KC_S)
#define SYM_T   LT(SYM, KC_T)
#define SFT_R   LSFT_T(KC_R)
#define NAV_D   LT(NAV, KC_D)
#define GUI_X   LGUI_T(KC_X)
#define CTL_G   LCTL_T(KC_G)

#define NUM_N   LT(NUM, KC_N)
#define SFT_E   RSFT_T(KC_E)
#define SYM_A   LT(SYM, KC_A)
#define ALT_I   LALT_T(KC_I)
#define CTL_H   RCTL_T(KC_H)
#define WIN_DOT LT(WIN, KC_DOT)
#define GUI_QUO LGUI_T(KC_QUOTE)

//[HRM_RU]
#define ALT_EF  LALT_T(RU_EF)
#define SYM_YRU LT(SYM, RU_YERU)
#define SFT_VE  LSFT_T(RU_VE)
#define NAV_A   LT(NAV, RU_A)
#define GUI_YA  LGUI_T(RU_YA)
#define CTL_EM  LCTL_T(RU_EM)

#define NUM_O   LT(NUM, RU_O)
#define SFT_EL  RSFT_T(RU_EL)
#define SYM_DE  LT(SYM, RU_DE)
#define ALT_ZHE LALT_T(RU_ZHE)
#define CTL_SFT RCTL_T(RU_SOFT)
#define WIN_YU  LT(WIN, RU_YU)
#define GUI_E   LGUI_T(RU_E)

//[HRM_BOTH]
#define EXT_COL LT(EXT, KC_SCLN)

//[HRM_SYM]
#define NAV_EQL LT(NAV, KC_EQL)
#define NAV_SLS LT(NAV, KC_SLSH)

//[EDITOR]
#define CUT     C(KC_X)
#define COPY    C(KC_C)
#define PASTE   C(KC_V)
#define SEL_ALL C(KC_A)
#define SAVE    C(KC_S)
#define UNDO    C(KC_Z)
#define REP_SEL C(KC_H)

//[BROWSER]
#define NEW_BTB C(KC_T)

//[WINDOWS]
#define EXP_TAB G(KC_TAB)
#define BWD_TAB S(A(KC_TAB))
#define FWD_TAB A(KC_TAB)
#define CLOSE   A(KC_F4)
#define APP_SCH A(KC_SPC)
#define OPTIONS G(KC_A)
#define MENU    G(KC_X)
#define TKMGR   C(S(KC_ESC))
#define EXPLR   G(KC_E)
#define OPTS    G(KC_I)
#define LOCK    G(KC_L)
#define PRV_TAB C(KC_PGUP)
#define NXT_TAB C(KC_PGDN)
#define WIN_LFT G(S(KC_LEFT))
#define WIN_RGT G(S(KC_RGHT))
#define FST_MS  G(S(KC_D))
#define PRV_DKP  G(C(KC_LEFT))
#define NXT_DKP  G(C(KC_RGHT))
#define NEW_DKP  G(C(KC_D))
#define CLS_DKP  G(C(KC_F4))


//[AUDIO]
#define MUTEMIC KC_F20

enum custom_keycodes {
    KC_USER_START = SAFE_RANGE,
    UPDIR,
    TMUXESC,    /*Enter copy  mode in Tmux.*/
    SRCHSEL,    /*Searches the current selection in a new tab.*/
    SELLINE,    /*Line selection*/
    SELWBAK,    /*Backward word selection*/
    SELWFWD,    /*Forward word selection*/
    LT_WORD,
    M_THE,      /* *magic*=' '      */
    M_ION,      /* *magic*='i'      */
    M_MENT,     /* *magic*='m'      */
    M_QUEN,     /* *magic*='q'      */
    M_TMENT,    /* *magic*='t'      */
    M_UPDIR,    /* *magic*='.'      */
    M_INCLUDE,  /* *magic*='#'      */
    M_EQEQ,     /* *magic*='=='     */
    M_NBSP,     /* *magic*='nbsp;'  */
    M_MKGRVS,   /* *magic*='`'      */
    M_DOCSTR,   /* *magic*='"'      */
    M_NOOP,
    KC_USER_END,
};
