#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <aesndlib.h>
#include <grrmod.h>
#include <math.h>

#include "main.h"

int HWButton = -1;
struct ShiggyData shiggy;
int frameCount = 10;
int frameIndex = 0;
signed int timerSeconds = -3;
int theme = 1;
AESNDPB* shigCompleteVoice;
int timer = 0;
unsigned int skinNum = 0;

/**
 * handler for controls so my code is less shit lmao
 */
void buttonHandler() {
    if ((WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) || (PAD_ButtonsHeld(0) & PAD_BUTTON_A)) {
        if (shiggy.hyperTimer == 0)
            shiggy.hyperTimer = 9;
    }
    if ((WPAD_ButtonsDown(0)) || (PAD_ButtonsDown(0))) {
    if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_PLUS) || (PAD_ButtonsDown(0) & PAD_BUTTON_START)) {
        if ((WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) || (PAD_ButtonsHeld(0) & PAD_BUTTON_Y)) {
            ++skinNum;
            skinNum %= SKIN_AMOUNT;
            shiggy.Skin = shiggySkins[skinNum];
            return;
        }
        shiggy.shigCount = 0;
        timerSeconds = 59;
        timer = 0;
    }
    if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_MINUS) || (PAD_ButtonsDown(0) & PAD_BUTTON_B)) {
        if ((WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) || (PAD_ButtonsHeld(0) & PAD_BUTTON_Y)) {
            --skinNum;
            if ((signed)skinNum < 0)
                skinNum = SKIN_AMOUNT-1;
            shiggy.Skin = shiggySkins[skinNum];
            return;
        }
        shiggy.shigCount = 0;
        timerSeconds = -4;
        timer = 0;
    }
    if ((WPAD_ButtonsDown(0) & WPAD_BUTTON_2) || (PAD_ButtonsDown(0) & PAD_TRIGGER_Z)) shiggy.x = shiggy.y = shiggy.angle = 0;
    ++frameIndex;
    frameIndex %= frameCount;
    if (!frameIndex & ((timerSeconds >= 0) || (timerSeconds <= -5))) {
        shigCompleteVoice = AESND_AllocateVoice(VoiceCallBack);
        if (shigCompleteVoice) switch (shiggy.Skin.sndNum) {
                case SOUND_GLITCH:
                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_glitch_raw, concrete_glitch_raw_size, 48000, 0, 0);
                    break;
                case SOUND_EXPLOD:
                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, explod_raw, explod_raw_size, 48000, 0, 0);
                    break;
                case SOUND_LOWQUALITY:
                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_lowquality_raw, concrete_lowquality_raw_size, 48000, 0, 0);
                    break;
                case SOUND_REVERSE:
                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_reverse_raw, concrete_reverse_raw_size, 48000, 0, 0);
                    break;
                case SOUND_TROLOLO:
                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, yayayayay_raw, yayayayay_raw_size, 48000, 0, 0);
                    break;
                default:
                    AESND_PlayVoice(shigCompleteVoice, VOICE_STEREO16, concrete_raw, concrete_raw_size, 48000, 0, 0);
                    break;
        }
        ++shiggy.shigCount;
    }
    }
}

/**
 * Callback for the reset button on the Wii.
 */
void WiiResetPressed() {
	HWButton = SYS_RETURNTOMENU;
}

/**
 * Callback for the power button on the Wii.
 */
void WiiPowerPressed() {
	HWButton = SYS_POWEROFF;
}

/**
 * Callback for the power button on the Wiimote.
 * @param[in] chan The Wiimote that pressed the button
 */
void WiimotePowerPressed(s32 chan) {
	if (!chan) HWButton = SYS_POWEROFF;
}

/**
 * Entry point.
 * @param[in] argc The number of arguments invoked with the program
 * @param[in] argv The array containing the arguments
 * @return 0 on clean exit, an error code otherwise
 */
int main(int argc, char **argv) {
// init video
    init_textures();
    modify_textures();
    GRRLIB_Init();
// init input
    WPAD_Init();
    PAD_Init();
    WPAD_SetDataFormat(0, WPAD_FMT_BTNS_ACC_IR);
// make the power and reset buttons work the way you would expect them to
    WPAD_SetPowerButtonCallback(WiimotePowerPressed);
    SYS_SetResetCallback(WiiResetPressed);
	SYS_SetPowerCallback(WiiPowerPressed);
// init sound
    AESND_Init();
    GRRMOD_Init(true);
    GRRMOD_SetMOD(bz_pif_it, bz_pif_it_size);
    GRRMOD_Start();
// the variable chunk (oooh! ahhh!)
    char str[38];
    // int texture = 0; // aaaaAAAAAHHH
    WPADData *wd;
    GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);
    shiggy.x = shiggy.y = shiggy.angle = shiggy.shigCount = shiggy.hyperTimer = 0; // lmfao
    shiggy.Skin = shiggySkins[0];
// main loop
    while (1) {
        timer++;
        if (timer >= 60) {
            timer = 0;
            --timerSeconds;
            if ((timerSeconds < -1) & !(timerSeconds < -2))
                timerSeconds = -1;
            if ((timerSeconds < -3) & !(timerSeconds <= -5))
                timerSeconds = -3;
        }
		if (HWButton != -1)
            break;
        WPAD_ScanPads();
        PAD_ScanPads();
        wd = WPAD_Data(0);
        if (wd->ir.valid) {
            shiggy.x = wd->ir.x;
            shiggy.y = wd->ir.y;
            shiggy.angle = wd->ir.angle;
        }

        if (abs(PAD_StickX(0)) > 25)
            shiggy.x += PAD_StickX(0)/20;
        if (abs(PAD_StickY(0)) > 25)
            shiggy.y -= PAD_StickY(0)/20;
        if (PAD_ButtonsHeld(0) & PAD_TRIGGER_L)
            --shiggy.angle;
        if (PAD_ButtonsHeld(0) & PAD_TRIGGER_R)
            ++shiggy.angle;
        if (shiggy.hyperTimer) shiggy.hyperTimer--;

        GRRLIB_FillScreen(DARK_BG);

        if (WPAD_ButtonsHeld(0) || PAD_ButtonsHeld(0))
            buttonHandler();
        GRRLIB_DrawPart(shiggy.x+320, shiggy.y-(hyperYCoords[10 - shiggy.hyperTimer]), frameIndex*64, 0, 64, 64, shiggyTex[shiggy.Skin.texNum], shiggy.angle, 2, 2, GRRLIB_WHITE);
        GRRLIB_SetMidHandle(shiggyTex[shiggy.Skin.texNum], true);
        if (timerSeconds > TIMER_STATE_TIMEOVER) {
            double deciseconds = 10-((double)timer/60*10);
            if (deciseconds >= 10)
                deciseconds = 9;
            sprintf(str, "%u shigs, %u.%us left", shiggy.shigCount, timerSeconds, (int)deciseconds);
        } else if (timerSeconds == TIMER_STATE_TIMEOVER) {
            sprintf(str, "%u shigs in 60s", shiggy.shigCount);
        } else if (timerSeconds <= TIMER_STATE_GAMEOVER) {
            sprintf(str, "%u shigs", shiggy.shigCount);
        } else sprintf(str, "+/START: Start Timer, -/B: Infinite");
        GRRLIB_SetAntiAliasing(false);
        GRRLIB_PrintfTTF(320-(GRRLIB_WidthTTF(font, str, 24)/2), 32, font, str, 24, DARK_FG);
        GRRLIB_Render();
        // for (int i = 1; i < 20; i++) VIDEO_WaitVsync();
    }

    WPAD_Shutdown();
    free_textures();
    GRRMOD_End();
    GRRLIB_FreeTTF(font);
    GRRLIB_Exit();
    if (HWButton != -1)
        SYS_ResetSystem(HWButton, 0, 0);
    exit(0);
    return 0;
}
// comment
