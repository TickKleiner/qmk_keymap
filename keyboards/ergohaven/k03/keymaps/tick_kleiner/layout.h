#pragma once
// clang-format off
#define LAYOUT_K03( \
    k05, k04, k03, k02, k01, k00,            k50, k51, k52, k53, k54, k55,  \
    k15, k14, k13, k12, k11, k10,            k60, k61, k62, k63, k64, k65,  \
    k25, k24, k23, k22, k21, k20,            k70, k71, k72, k73, k74, k75,  \
    k35, k34, k33, k32, k31, k30, k40,  k90, k80, k81, k82, k83, k84, k85,  \
              k45, k44, k43, k42, k41,  k91, k92, k93, k94, k95) {          \
    { k00, k01, k02, k03, k04, k05 },                                       \
    { k10, k11, k12, k13, k14, k15 },                                       \
    { k20, k21, k22, k23, k24, k25 },                                       \
    { k30, k31, k32, k33, k34, k35 },                                       \
    { k40, k41, k42, k43, k44, k45 },                                       \
    { k50, k51, k52, k53, k54, k55 },                                       \
    { k60, k61, k62, k63, k64, k65 },                                       \
    { k70, k71, k72, k73, k74, k75 },                                       \
    { k80, k81, k82, k83, k84, k85 },                                       \
    { k90, k91, k92, k93, k94, k95 }                                        \
}

#define LAYOUT_LR( \
  L05, L04, L03, L02, L01, L00,                                             \
  L15, L14, L13, L12, L11, L10,                                             \
  L25, L24, L23, L22, L21, L20,                                             \
  L35, L34, L33, L32, L31, L30, L40,                                        \
            L45, L44, L43, L42, L41,                                        \
                                                                            \
       R50, R51, R52, R53, R54, R55,                                        \
       R60, R61, R62, R63, R64, R65,                                        \
       R70, R71, R72, R73, R74, R75,                                        \
  R90, R80, R81, R82, R83, R84, R85,                                        \
  R91, R92, R93, R94, R95)                                                  \
  LAYOUT_K03(                                                               \
    L05, L04, L03, L02, L01, L00,            R50, R51, R52, R53, R54, R55,  \
    L15, L14, L13, L12, L11, L10,            R60, R61, R62, R63, R64, R65,  \
    L25, L24, L23, L22, L21, L20,            R70, R71, R72, R73, R74, R75,  \
    L35, L34, L33, L32, L31, L30, L40,  R90, R80, R81, R82, R83, R84, R85,  \
              L45, L44, L43, L42, L41,  R91, R92, R93, R94, R95)
// clang-format on
// Matrix positions of the left home row keys.
#define LEFT_HOME_ROW       2
#define LEFT_BOTTOM_ROW     LEFT_HOME_ROW + 1
#define LEFT_UPPER_ROW      LEFT_HOME_ROW - 1
#define RIGHT_HOME_ROW      7
#define RIGHT_BOTTOM_ROW    RIGHT_HOME_ROW + 1
#define RIGHT_UPPER_ROW     RIGHT_HOME_ROW - 1
#define INNER_COL   0
#define INDEX_COL   1
#define MIDDLE_COL  2
#define RING_COL    3
#define PINKY_COL   4
#define OUTER_COL   5
