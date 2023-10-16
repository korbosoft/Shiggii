#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <asndlib.h>
#include <mp3player.h>
#include <math.h>

#include "main.h"

#include "shiggy_loop_mp3.h"
#include "font_ttf.h"

#define DARK_BG 0x282828FF
#define LIGHT_BG 0xF5EDCAFF
#define DARK_FG 0xD4BE98FF
#define LIGHT_FG 0x644735FF
#define GRRLIB_WHITE 0xFFFFFFFF

int frameIndex = 0;

int main(int argc, char **argv) {
    GRRLIB_Init();
    GRRLIB_SetAntiAliasing(false);

    WPAD_Init();
    WPAD_SetDataFormat(0, WPAD_FMT_BTNS_ACC_IR);
    PAD_Init();

    ASND_Init();
    MP3Player_Init();

    init_textures();

    int frameCount = sizeof(frames) / sizeof(GRRLIB_texImg * );
    int shigCount = 0;
    char str[38];
    int theme = 1;
    double sx = 0, sy = 0, sa = 0;
    WPADData *wd;
    GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);

    while (1) {
        if (!MP3Player_IsPlaying()) {
            MP3Player_PlayBuffer(shiggy_loop_mp3, shiggy_loop_mp3_size, NULL);
        }

        WPAD_ScanPads();
        PAD_ScanPads();
        wd = WPAD_Data(0);
        if (wd->ir.valid) {
            sx = wd->ir.x;
            sy = wd->ir.y;
            sa = wd->ir.angle;
        }
        if (abs(PAD_StickX(0)) > 25.4) sx += PAD_StickX(0)/25.4;
        if (abs((double)PAD_StickY(0)) > 25.4) sy -= PAD_StickY(0)/25.4;
        if (abs(PAD_TriggerL(0)/255) > 0.95) --sa;
        if (abs(PAD_TriggerR(0)/255) > 0.95) ++sa;

        if (theme) {
            GRRLIB_FillScreen(DARK_BG);
        } else {
            GRRLIB_FillScreen(LIGHT_BG);
        }

        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
            break;
        } else if ((WPAD_ButtonsDown(0) != 0) || (PAD_ButtonsDown(0) != 0)) {
            if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_A) || (PAD_ButtonsDown(0) & PAD_BUTTON_A)) {
                if (GRRLIB_PtInRect(536, 384, 64, 64, sx, sy)) {
                    ++theme;
                    theme %= 2;
                }
            }
            if (PAD_ButtonsDown(0) & PAD_TRIGGER_Z) sx = sy = sa = 0;
            ++frameIndex;
            frameIndex %= frameCount;
            if (!frameIndex) ++shigCount;
        }


        GRRLIB_texImg *frame = frames[frameIndex];
        GRRLIB_SetMidHandle(frame, true);
        GRRLIB_texImg *themeButtonTex = themeButtonIndex[theme];

        GRRLIB_DrawImg(sx, sy, frame, sa, .25, .25, GRRLIB_WHITE);
        sprintf(str, "%d shigs", shigCount);
        if (theme) {
            GRRLIB_DrawImg(536, 384, themeButtonTex, 0, 1, 1, DARK_FG);
            GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, DARK_FG);
        } else {
            GRRLIB_DrawImg(536, 384, themeButtonTex, 0, 1, 1, LIGHT_FG);
            GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, LIGHT_FG);
        }
        GRRLIB_Render();
    }
    free_textures();
    GRRLIB_Exit();
    return 0;
}
