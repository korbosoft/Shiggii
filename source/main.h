#include <grrlib.h>

#include "shiggy_png.h"

#include "light_png.h"
#include "dark_png.h"

GRRLIB_texImg *themeButtonIndex[2];
GRRLIB_texImg *shiggyTex;

void init_textures() {
    themeButtonIndex[0] = GRRLIB_LoadTexture(light_png);
    themeButtonIndex[1] = GRRLIB_LoadTexture(dark_png);
    shiggyTex = GRRLIB_LoadTexture(shiggy_png);
}

void free_textures() {
    for (int i = 0; i < 1; i++) {
        GRRLIB_FreeTexture(themeButtonIndex[i]);
    }
}
