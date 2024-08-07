#include <grrlib.h>
#include "shiggy0_png.h"
#include "shiggy1_png.h"
#include "shiggy2_png.h"
#include "shiggy3_png.h"
#include "shiggy4_png.h"
#include "aro_png.h"
#include "ace_png.h"
#include "aroace_png.h"
#include "agndr_png.h"
#include "trans_png.h"
#include "lgbtq_png.h"
#include "gay_png.h"
#include "les_png.h"
#include "one_bpp_png.h"
#include "two_bpp_png.h"
#include "vennie_png.h"
#include "bi_png.h"
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

#define DARK_BG      0x1E1E2EFF
#define DARK_FG      0xCDD6F4FF
#define WHITE 0xFFFFFFFF
#define SKIN_AMOUNT 18

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
    {6,  SOUND_DEFAULT,    64,  {"Aromantic"}},
    {7,  SOUND_DEFAULT,    64,    {"Asexual"}},
    {8,  SOUND_DEFAULT,    64,    {"Aro Ace"}},
    {9,  SOUND_DEFAULT,    64,    {"Agender"}},
    {10, SOUND_DEFAULT,    64,      {"Trans"}},
    {11, SOUND_DEFAULT,    64,     {"LGBTQ+"}},
    {12, SOUND_DEFAULT,    64,    {"Lesbian"}},
    {13, SOUND_DEFAULT,    64,        {"Gay"}},
    {14, SOUND_DEFAULT,    64,   {"Bisexual"}},
    {15, SOUND_DEFAULT,    64,       {"1bpp"}},
    {16, SOUND_DEFAULT,    64,       {"2bpp"}},
    {17, SOUND_DEFAULT,    64,     {"Vennie"}}
};

void init_textures() {
    shiggyTex[0] = GRRLIB_LoadTexture(shiggy0_png);
    shiggyTex[1] = GRRLIB_LoadTexture(shiggy1_png);
    shiggyTex[2] = GRRLIB_LoadTexture(shiggy2_png);
    shiggyTex[3] = GRRLIB_CreateEmptyTexture(shiggyTex[0]->w, shiggyTex[0]->h);
    shiggyTex[4] = GRRLIB_LoadTexture(shiggy3_png);
    shiggyTex[5] = GRRLIB_LoadTexture(shiggy4_png);
    shiggyTex[6] = GRRLIB_LoadTexture(aro_png);
    shiggyTex[7] = GRRLIB_LoadTexture(ace_png);
    shiggyTex[8] = GRRLIB_LoadTexture(aroace_png);
    shiggyTex[9] = GRRLIB_LoadTexture(agndr_png);
    shiggyTex[10] = GRRLIB_LoadTexture(trans_png);
    shiggyTex[11] = GRRLIB_LoadTexture(lgbtq_png);
    shiggyTex[12] = GRRLIB_LoadTexture(les_png);
    shiggyTex[13] = GRRLIB_LoadTexture(gay_png);
    shiggyTex[14] = GRRLIB_LoadTexture(bi_png);
    shiggyTex[15] = GRRLIB_LoadTexture(one_bpp_png);
    shiggyTex[16] = GRRLIB_LoadTexture(two_bpp_png);
    shiggyTex[17] = GRRLIB_LoadTexture(vennie_png);
    GRRLIB_BMFX_FlipH(shiggyTex[0], shiggyTex[3]);
    GRRLIB_FlushTex(shiggyTex[3]);
    font = GRRLIB_LoadTexture(font_png);
    GRRLIB_InitTileSet(font, 9, 16, 32);
}

void free_textures() {
    for (int i = 0; i <= SKIN_AMOUNT-1; i++) {
        GRRLIB_FreeTexture(shiggyTex[i]);
    }
}

static void VoiceCallBack(AESNDPB *pb, u32 state)
{
	if (state == VOICE_STATE_STOPPED)
		AESND_FreeVoice(pb);
}
