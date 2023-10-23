#include <grrlib.h>

#include "shiggy0_png.h"
#include "shiggy1_png.h"
#include "shiggy2_png.h"
#include "shiggy3_png.h"
#include "bz_pif_it.h"
#include "font_ttf.h"
#include "light_png.h"
#include "dark_png.h"
#include "concrete_raw.h"
#include "concrete_lowquality_raw.h"
#include "concrete_reversed_raw.h"
#include "concrete_glitch_raw.h"
#include "explod_raw.h"

#define DARK_BG 0x282828FF
#define LIGHT_BG 0xF5EDCAFF
#define DARK_FG 0xD4BE98FF
#define LIGHT_FG 0x644735FF
#define GRRLIB_WHITE 0xFFFFFFFF

double aspectRatioScale(double x) {return x/(4.f/3.f);}

GRRLIB_texImg *themeButtonIndex[2];
GRRLIB_texImg *shiggyTex[4];

struct shiggyData {
    double x, y, angle;
    int skin;
};

void init_textures() {
    themeButtonIndex[0] = GRRLIB_LoadTexture(light_png);
    themeButtonIndex[1] = GRRLIB_LoadTexture(dark_png);

    shiggyTex[0] = GRRLIB_LoadTexture(shiggy0_png);
    shiggyTex[1] = GRRLIB_LoadTexture(shiggy1_png);
    shiggyTex[2] = GRRLIB_LoadTexture(shiggy2_png);
    shiggyTex[3] = GRRLIB_LoadTexture(shiggy3_png);
}

void free_textures() {
    for (int i = 0; i < 2; i++) {
        GRRLIB_FreeTexture(themeButtonIndex[i]);
    }
    for (int i = 0; i < 3; i++) {
        GRRLIB_FreeTexture(shiggyTex[i]);
    }
}

static void VoiceCallBack(AESNDPB *pb, u32 state)
{
	if (state == VOICE_STATE_STOPPED)
		AESND_FreeVoice(pb);
}
