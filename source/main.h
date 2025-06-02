#include <grrlib.h>

#include "resource.h"

#define TIMER_STATE_GAMEOVER -4
#define TIMER_STATE_TIMEOVER -1

#define SKIN_AMOUNT 11

GRRLIB_texImg *shiggyTex[SKIN_AMOUNT];
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
    unsigned int texNum, sndNum, texRes;
    char name[11];
};

struct ShiggyData {
    double x, y, angle;
    int shigCount;
    unsigned int hyperTimer;
    struct ShiggySkin Skin;
};

struct ShiggySkin shiggySkins[SKIN_AMOUNT] = {
    {0,  SOUND_DEFAULT,    64,     {"Shiggy"}},
    {1,  SOUND_GLITCH,     64,  {"Corrupted"}},
    {2,  SOUND_EXPLOD,     64, {"Shigsplode"}},
    {3,  SOUND_REVERSE,    64,     {"yggihS"}},
    {4,  SOUND_TROLOLO,    64,    {"Shiggey"}},
    {5,  SOUND_LOWQUALITY, 64,     {"Shoggy"}},
    {6,  SOUND_DEFAULT,    64,       {"Woke"}},
    {7,  SOUND_DEFAULT,    64,       {"1bpp"}},
    {8,  SOUND_DEFAULT,    64,       {"2bpp"}},
    {9,  SOUND_DEFAULT,    64,     {"Vennie"}},
    {10, SOUND_MIRROR,     64,    {"Crawley"}}
};

void init_textures() {
    shiggyTex[0] = GRRLIB_LoadTexture(shiggy0_png);
    shiggyTex[1] = GRRLIB_LoadTexture(shiggy1_png);
    shiggyTex[2] = GRRLIB_LoadTexture(shiggy2_png);
    shiggyTex[3] = GRRLIB_CreateEmptyTexture(shiggyTex[0]->w, shiggyTex[0]->h);
    shiggyTex[4] = GRRLIB_LoadTexture(shiggy3_png);
    shiggyTex[5] = GRRLIB_LoadTexture(shiggy4_png);
    shiggyTex[6] = GRRLIB_LoadTexture(lgbtq_png);
    shiggyTex[7] = GRRLIB_LoadTexture(one_bpp_png);
    shiggyTex[8] = GRRLIB_LoadTexture(two_bpp_png);
    shiggyTex[9] = GRRLIB_LoadTexture(vennie_png);
    shiggyTex[10] = GRRLIB_LoadTexture(crawley_png);
    GRRLIB_BMFX_FlipH(shiggyTex[0], shiggyTex[3]);
    GRRLIB_FlushTex(shiggyTex[3]);
    font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(font, 9, 16, 32);
}

void free_textures() {
    for (int i = 0; i <= SKIN_AMOUNT - 1; i++) {
        GRRLIB_FreeTexture(shiggyTex[i]);
    }
}

static void VoiceCallBack(AESNDPB *pb, u32 state)
{
	if (state == VOICE_STATE_STOPPED)
		AESND_FreeVoice(pb);
}
