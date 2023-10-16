#include <grrlib.h>

#include "shiggy_1_png.h"
#include "shiggy_2_png.h"
#include "shiggy_3_png.h"
#include "shiggy_4_png.h"
#include "shiggy_5_png.h"
#include "shiggy_6_png.h"
#include "shiggy_7_png.h"
#include "shiggy_8_png.h"
#include "shiggy_9_png.h"
#include "shiggy_10_png.h"
#include "light_png.h"
#include "dark_png.h"

GRRLIB_texImg *frames[10];

GRRLIB_texImg *themeButtonIndex[2];

void init_textures() {
    // TODO: just replace this with something else, this is god awful
    frames[0] = GRRLIB_LoadTexture(shiggy_1_png);
    frames[1] = GRRLIB_LoadTexture(shiggy_2_png);
    frames[2] = GRRLIB_LoadTexture(shiggy_3_png);
    frames[3] = GRRLIB_LoadTexture(shiggy_4_png);
    frames[4] = GRRLIB_LoadTexture(shiggy_5_png);
    frames[5] = GRRLIB_LoadTexture(shiggy_6_png);
    frames[6] = GRRLIB_LoadTexture(shiggy_7_png);
    frames[7] = GRRLIB_LoadTexture(shiggy_8_png);
    frames[8] = GRRLIB_LoadTexture(shiggy_9_png);
    frames[9] = GRRLIB_LoadTexture(shiggy_10_png);
    themeButtonIndex[0] = GRRLIB_LoadTexture(light_png);
    themeButtonIndex[1] = GRRLIB_LoadTexture(dark_png);
}

void free_textures() {
    int frame_count = sizeof(frames) / sizeof(GRRLIB_texImg * );

    for (int i = 0; i < frame_count; i++) {
        GRRLIB_FreeTexture(frames[i]);
    }

        for (int i = 0; i < 1; i++) {
        GRRLIB_FreeTexture(themeButtonIndex[i]);
    }
}
