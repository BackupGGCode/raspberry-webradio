///////////////////////////////////////////////////////////////
//
//  Volume Control
//
//  12.09.2012 - Michael Schwarz
//
///////////////////////////////////////////////////////////////  

#include "screen_volume.h"

typedef enum {
    AUDIO_VOLUME_SET,
    AUDIO_VOLUME_GET,
} audio_volume_action;

int audio_volume(audio_volume_action action, long* outvol) {
    int ret;
    snd_mixer_t* handle;
    snd_mixer_elem_t* elem;
    snd_mixer_selem_id_t* sid;

    static const char* mix_name = "Master";
    static const char* card = "default";
    static int mix_index = 0;

    snd_mixer_selem_id_alloca(&sid);

    //sets simple-mixer index and name
    snd_mixer_selem_id_set_index(sid, mix_index);
    snd_mixer_selem_id_set_name(sid, mix_name);

        if ((snd_mixer_open(&handle, 0)) < 0)
        return -1;
    if ((snd_mixer_attach(handle, card)) < 0) {
        snd_mixer_close(handle);
        return -2;
    }
    if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
        snd_mixer_close(handle);
        return -3;
    }
    ret = snd_mixer_load(handle);
    if (ret < 0) {
        snd_mixer_close(handle);
        return -4;
    }
    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        snd_mixer_close(handle);
        return -5;
    }

    long minv, maxv;

    snd_mixer_selem_get_playback_volume_range (elem, &minv, &maxv);

    if(action == AUDIO_VOLUME_GET) {
        if(snd_mixer_selem_get_playback_volume(elem, 0, outvol) < 0) {
            snd_mixer_close(handle);
            return -6;
        }

        /* make the value bound to 100 */
        *outvol -= minv;
        maxv -= minv;
        minv = 0;
        *outvol = 100 * (*outvol) / maxv; // make the value bound from 0 to 100
    }
    else if(action == AUDIO_VOLUME_SET) {
        if(*outvol < 0 || *outvol > 100) // out of bounds
            return -7;
        *outvol = (*outvol * (maxv - minv) / (100-1)) + minv;

        if(snd_mixer_selem_set_playback_volume(elem, 0, *outvol) < 0) {
            snd_mixer_close(handle);
            return -8;
        }
        if(snd_mixer_selem_set_playback_volume(elem, 1, *outvol) < 0) {
            snd_mixer_close(handle);
            return -9;
        }
    }

    snd_mixer_close(handle);
    return 0;
}


long vol, setvol;

// ---------------------------------------------------------------------------
void init_Volume() {
  audio_volume(AUDIO_VOLUME_GET, &vol);
  printf("Master volume is %li\n", vol);
}


// ---------------------------------------------------------------------------
void draw_Volume() {
        Screen_DrawBorder(_lng(VOLUME_TITLE));

	int8_t rotary = IO_GetRotaryValue();
	if(rotary != 0) {
	 vol += rotary * 5;
	 if(vol > 100) vol = 100;
	 if(vol < 0) vol = 0;
	 
	 setvol = vol;
	 audio_volume(AUDIO_VOLUME_SET, &setvol);
	}
	int dvol = vol * 64 / 100;
	
	GLCDD_Rect r;
	r.x = 8;
	r.y = 16;
	r.w = SCREEN_W;
	r.h = -1;
	
	//GLCDD_Printf(fnt_dejavu_9, &r, "%li%%", vol);
	
	r.x = 10;
	r.y = 32;
	GLCDD_Print(fnt_dejavu_9, &r, "0%");
	r.x = SCREEN_W - 1 - 32;
        GLCDD_Print(fnt_dejavu_9, &r, "100%");
        GLCDD_Line(26, 36, SCREEN_W - 1 - 32 - 4, 36);
        GLCDD_Line(26, 37, SCREEN_W - 1 - 32 - 4, 37);
        
        GLCDD_ClearEx(26 + dvol - 1, 33, 26 + dvol - 1 + 3, 33 + 8);
        GLCDD_Rectangle(26 + dvol - 1, 33, 4, 8, 0);
        GLCDD_ClearPixel(26 + dvol - 1, 33);
        GLCDD_ClearPixel(26 + dvol - 1 + 3, 33);
        GLCDD_ClearPixel(26 + dvol - 1, 40);
        GLCDD_ClearPixel(26 + dvol - 1 + 3, 40);
        GLCDD_SetPixel(26 + dvol + 1, 35);
        GLCDD_SetPixel(26 + dvol + 1, 37);
        GLCDD_SetPixel(26 + dvol + 1, 39);
	
}

