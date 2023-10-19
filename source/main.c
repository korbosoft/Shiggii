#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <aesndlib.h>
#include <grrmod.h>
#include <math.h>

#include "main.h"


#define DARK_BG 0x282828FF
#define LIGHT_BG 0xF5EDCAFF
#define DARK_FG 0xD4BE98FF
#define LIGHT_FG 0x644735FF
#define GRRLIB_WHITE 0xFFFFFFFF

int frameIndex = 0;

int main(int argc, char **argv) {
    GRRLIB_Init();

    WPAD_Init();
    WPAD_SetDataFormat(0, WPAD_FMT_BTNS_ACC_IR);
    PAD_Init();

    AESND_Init();
    GRRMOD_Init(true);
    GRRMOD_SetMOD(bz_pif_it, bz_pif_it_size);
    GRRMOD_Start();
    init_textures();
    int timer = 0;
    int frameCount = 10;
    unsigned int shigCount = 0;
    char str[38];
    int theme = 1;
    double sx = 0, sy = 0, sa = 0;
    signed int timerSeconds = -3;
    WPADData *wd;
    GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);
    AESNDPB* shigCompleteVoice;

    while (1) {
        timer++;
        if (timer >= 60) {
            timer = 0;
            --timerSeconds;
            if ((timerSeconds < -1) & !(timerSeconds < -2)) timerSeconds = -1;
            if (timerSeconds < -3) timerSeconds = -3;
        }
        WPAD_ScanPads();
        PAD_ScanPads();
        wd = WPAD_Data(0);
        if (wd->ir.valid) {
            sx = wd->ir.x;
            sy = wd->ir.y;
            sa = wd->ir.angle;
        }
        if (abs(PAD_StickX(0)) > 25) sx += PAD_StickX(0)/20;
        if (abs(PAD_StickY(0)) > 25) sy -= PAD_StickY(0)/20;
        if (PAD_ButtonsHeld(0) & PAD_TRIGGER_L) --sa;
        if (PAD_ButtonsHeld(0) & PAD_TRIGGER_R) ++sa;

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
        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_PLUS) || (PAD_ButtonsDown(0) & PAD_BUTTON_START)) {
            shigCount = 0;
            timerSeconds = 59;
            timer = 0;
        }
            if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_2) || (PAD_ButtonsDown(0) & PAD_TRIGGER_Z)) sx = sy = sa = 0;
            ++frameIndex;
            frameIndex %= frameCount;
            if (!frameIndex & (timerSeconds > -1)) {
                shigCompleteVoice = AESND_AllocateVoice(VoiceCallBack);
                if (shigCompleteVoice) {

                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, &concrete_raw, concrete_raw_size, 48000, 0, 0);
                }
                ++shigCount;
            }
        }
        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_1) || (PAD_ButtonsDown(0) & PAD_BUTTON_X)) {
            GRRLIB_ScrShot("sd:/shiggy.png");
        }

        GRRLIB_texImg *themeButtonTex = themeButtonIndex[theme];

        GRRLIB_SetAntiAliasing(false);
        GRRLIB_DrawPart(sx+320, sy, 0+(64*frameIndex), 0, 64, 64, shiggyTex, sa, 1, 1, GRRLIB_WHITE);
        GRRLIB_SetMidHandle(shiggyTex, true);
        if (timerSeconds > -1) {
            double deciSeconds = 10-((double)timer/60*10);
            sprintf(str, "%u shigs, %u.%us left", shigCount, timerSeconds, (unsigned int)deciSeconds);
        } else if (timerSeconds == -1) {
            sprintf(str, "%u shigs in 60s", shigCount);
        } else sprintf(str, "Press +/START to start timer");
        GRRLIB_SetAntiAliasing(true);
        if (theme) {
            GRRLIB_DrawImg(536, 384, themeButtonTex, 0, 1, 1, DARK_FG);
            GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, DARK_FG);
        } else {
            GRRLIB_DrawImg(536, 384, themeButtonTex, 0, 1, 1, LIGHT_FG);
            GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, LIGHT_FG);
        }
        GRRLIB_Render();
        // for (int i = 1; i < 20; i++) VIDEO_WaitVSync();
    }
    WPAD_Shutdown();
    free_textures();
    GRRMOD_End();
    GRRLIB_FreeTTF(font);
    GRRLIB_Exit();
    exit(0);
    return 0;
}
