#include <grrlib.h>
#include "shiggy0_png.h"
#include "shiggy1_png.h"
#include "shiggy2_png.h"
#include "shiggy3_png.h"
#include "shiggy4_png.h"
#include "bz_pif_it.h"
#include "font_png.h"
#include "concrete_raw.h"
#include "concrete_lowquality_raw.h"
#include "concrete_reverse_raw.h"
#include "concrete_glitch_raw.h"
#include "yayayayay_raw.h"
#include "explod_raw.h"

#define TIMER_STATE_GAMEOVER -4
#define TIMER_STATE_TIMEOVER -1

#define SOUND_DEFAULT    0
#define SOUND_GLITCH     1
#define SOUND_EXPLOD     2
#define SOUND_LOWQUALITY 3
#define SOUND_REVERSE    4
#define SOUND_TROLOLO    5

#define DARK_BG      0x282828FF
#define DARK_FG      0xD4BE98FF
#define GRRLIB_WHITE 0xFFFFFFFF
#define SKIN_AMOUNT 6

GRRLIB_texImg *shiggyTex[6];
GRRLIB_texImg *font;

int hyperYCoords[10] = {
    0,
    10,
    19,
    26,
    31,
    32,
    31,
    26,
    19,
    10
};

struct ShiggySkin {
    unsigned int texNum, sndNum;
    char *name[10];
};

struct ShiggyData {
    double x, y, angle;
    int shigCount;
    unsigned int hyperTimer;
    struct ShiggySkin Skin;
};

struct ShiggySkin shiggySkins[6] = {
    {0, SOUND_DEFAULT,        {"Shiggy"}},
    {1, SOUND_GLITCH,      {"Corrupted"}},
    {2, SOUND_EXPLOD,     {"Shigsplode"}},
    {5, SOUND_LOWQUALITY,     {"Shoggy"}},
    {3, SOUND_REVERSE,        {"yggihS"}},
    {4, SOUND_TROLOLO,       {"Shiggey"}}
};

void init_textures() {
    shiggyTex[0] = GRRLIB_LoadTexture(shiggy0_png);
    shiggyTex[1] = GRRLIB_LoadTexture(shiggy1_png);
    shiggyTex[2] = GRRLIB_LoadTexture(shiggy2_png);
    shiggyTex[3] = GRRLIB_CreateEmptyTexture(shiggyTex[0]->w, shiggyTex[0]->h);
    shiggyTex[4] = GRRLIB_LoadTexture(shiggy3_png);
    shiggyTex[5] = GRRLIB_LoadTexture(shiggy4_png);
    GRRLIB_BMFX_FlipH(shiggyTex[0], shiggyTex[3]);
    GRRLIB_FlushTex(shiggyTex[3]);
    font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(font, 8, 16, 32);
}

void modify_textures() {

}

void free_textures() {
    for (int i = 0; i <= 4; i++) {
        GRRLIB_FreeTexture(shiggyTex[i]);
    }
}

static void VoiceCallBack(AESNDPB *pb, u32 state)
{
	if (state == VOICE_STATE_STOPPED)
		AESND_FreeVoice(pb);
}
