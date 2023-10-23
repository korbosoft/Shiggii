#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <aesndlib.h>
#include <grrmod.h>
#include <math.h>

#include "main.h"

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
    signed int timerSeconds = -3;
    WPADData *wd;
    GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);
    AESNDPB* shigCompleteVoice;
    struct shiggyData shiggy;
    shiggy.x = shiggy.y = shiggy.angle = shiggy.skin = 0;
    int texture; // aaaaAAAAAHHH
    while (1) {
        timer++;
        if (timer >= 60) {
            timer = 0;
            --timerSeconds;
            if ((timerSeconds < -1) & !(timerSeconds < -2)) timerSeconds = -1;
            if ((timerSeconds < -3) & !(timerSeconds <= -4)) timerSeconds = -3;
        }
        WPAD_ScanPads();
        PAD_ScanPads();
        wd = WPAD_Data(0);
        if (wd->ir.valid) {
            shiggy.x = wd->ir.x;
            shiggy.y = wd->ir.y;
            shiggy.angle = wd->ir.angle;
        }
        if (abs(PAD_StickX(0)) > 25) shiggy.x += PAD_StickX(0)/20;
        if (abs(PAD_StickY(0)) > 25) shiggy.y -= PAD_StickY(0)/20;
        if (PAD_ButtonsHeld(0) & PAD_TRIGGER_L) --shiggy.angle;
        if (PAD_ButtonsHeld(0) & PAD_TRIGGER_R) ++shiggy.angle;

        if (theme) {
            GRRLIB_FillScreen(DARK_BG);
        } else {
            GRRLIB_FillScreen(LIGHT_BG);
        }

        if (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) {
            break;
        } else if ((WPAD_ButtonsDown(0) != 0) || (PAD_ButtonsDown(0) != 0)) {
            if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_A) || (PAD_ButtonsDown(0) & PAD_BUTTON_A)) {
                if (GRRLIB_PtInRect(536, 384, 64, 64, shiggy.x, shiggy.y)) {
                    ++theme;
                    theme %= 2;
                }
            }
        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_PLUS) || (PAD_ButtonsDown(0) & PAD_BUTTON_START)) {
            shigCount = 0;
            timerSeconds = 59;
            timer = 0;
        }
            if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_2) || (PAD_ButtonsDown(0) & PAD_TRIGGER_Z)) shiggy.x = shiggy.y = shiggy.angle = 0;
            ++frameIndex;
            frameIndex %= frameCount;
            if (!frameIndex & (timerSeconds > -1)) {
                shigCompleteVoice = AESND_AllocateVoice(VoiceCallBack);
                if (shigCompleteVoice) {
                    switch (shiggy.skin) {
                        case 0:
                            AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_raw, concrete_raw_size, 48000, 0, 0);
                            break;
                        case 1:
                            AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_glitch_raw, concrete_glitch_raw_size, 48000, 0, 0);
                            break;
                        case 2:
                            AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, explod_raw, explod_raw_size, 48000, 0, 0);
                            break;
                        case 3:
                            AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_lowquality_raw, concrete_lowquality_raw_size, 48000, 0, 0);
                            break;
                        case 4:
                            AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_reversed_raw, concrete_reversed_raw_size, 48000, 0, 0);
                            break;
                    }
                }
                ++shigCount;
            }
        }
        if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_1) || (PAD_ButtonsDown(0) & PAD_BUTTON_X)) {
            GRRLIB_ScrShot("sd:/shiggy.png");
        }

        GRRLIB_texImg *themeButtonTex = themeButtonIndex[theme];
        GRRLIB_SetAntiAliasing(false);
        switch (shiggy.skin) {
            case 0:
                texture = 0;
                break;
            case 1:
                texture = 1;
                break;
            case 2:
                texture = 2;
                break;
            case 3:
                texture = 0;
                break;
            case 4:
                texture = 3;
                break;
}
        GRRLIB_DrawPart(shiggy.x+320, shiggy.y, 0+(64*frameIndex), 0, 64, 64, shiggyTex[texture], shiggy.angle, 2, 2, GRRLIB_WHITE);
        GRRLIB_SetMidHandle(shiggyTex[texture], true);
        if (timerSeconds > -1) {
            double deciseconds = 10-((double)timer/60*10);
            sprintf(str, "%u shigs, %u.%us left", shigCount, timerSeconds, (int)deciseconds);
        } else if (timerSeconds == -1) {
            sprintf(str, "%u shigs in 60s", shigCount);
        } else sprintf(str, "+/START: Start Timer, -/B: Infinite");
        GRRLIB_SetAntiAliasing(shiggy.skin != 4);
        if (theme) {
            GRRLIB_DrawImg(536, 384, themeButtonTex, 0, 1, 1, DARK_FG);
            GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, DARK_FG);
        } else {
            GRRLIB_DrawImg(536, 384, themeButtonTex, 0, 1, 1, LIGHT_FG);
            GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, LIGHT_FG);
        }
        GRRLIB_Render();
        // for (int i = 1; i < 20; i++) VIDEO_WaitVsync();
    }
    WPAD_Shutdown();
    free_textures();
    GRRMOD_End();
    GRRLIB_FreeTTF(font);
    GRRLIB_Exit();
    exit(0);
    return 0;
}
