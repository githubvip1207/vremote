// Copyright 2016 The go-vgo Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// https://github.com/go-vgo/robotgo/blob/master/LICENSE
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

#include "../base/types.h"
// #include "keycode.h"
// #include "keypress.h"
#include "keypress_c.h"
#include "keycode_c.h"


int keyboardDelay = 10;

struct KeyNames{
	const char* name;
	MMKeyCode   key;
}key_names[] = {
	{ "backspace",      K_BACKSPACE },
	{ "delete",         K_DELETE },
	{ "enter",          K_RETURN },
	{ "tab",            K_TAB },
	{ "escape",         K_ESCAPE },
	{ "up",             K_UP },
	{ "down",           K_DOWN },
	{ "right",          K_RIGHT },
	{ "left",           K_LEFT },
	{ "home",           K_HOME },
	{ "end",            K_END },
	{ "pageup",         K_PAGEUP },
	{ "pagedown",       K_PAGEDOWN },
	{ "f1",             K_F1 },
	{ "f2",             K_F2 },
	{ "f3",             K_F3 },
	{ "f4",             K_F4 },
	{ "f5",             K_F5 },
	{ "f6",             K_F6 },
	{ "f7",             K_F7 },
	{ "f8",             K_F8 },
	{ "f9",             K_F9 },
	{ "f10",            K_F10 },
	{ "f11",            K_F11 },
	{ "f12",            K_F12 },
	{ "f13",            K_F13 },
	{ "f14",            K_F14 },
	{ "f15",            K_F15 },
	{ "f16",            K_F16 },
	{ "f17",            K_F17 },
	{ "f18",            K_F18 },
	{ "f19",            K_F19 },
	{ "f20",            K_F20 },
	{ "f21",            K_F21 },
	{ "f22",            K_F22 },
	{ "f23",            K_F23 },
	{ "f24",            K_F24 },
	{ "command",        K_META },
	{ "alt",            K_ALT },
	{ "control",        K_CONTROL },
	{ "shift",          K_SHIFT },
	{ "right_shift",    K_RIGHTSHIFT },
	{ "space",          K_SPACE },
	{ "printscreen",    K_PRINTSCREEN },
	{ "insert",         K_INSERT },
	{ "menu",           K_MENU },

	{ "audio_mute",     K_AUDIO_VOLUME_MUTE },
	{ "audio_vol_down", K_AUDIO_VOLUME_DOWN },
	{ "audio_vol_up",   K_AUDIO_VOLUME_UP },
	{ "audio_play",     K_AUDIO_PLAY },
	{ "audio_stop",     K_AUDIO_STOP },
	{ "audio_pause",    K_AUDIO_PAUSE },
	{ "audio_prev",     K_AUDIO_PREV },
	{ "audio_next",     K_AUDIO_NEXT },
	{ "audio_rewind",   K_AUDIO_REWIND },
	{ "audio_forward",  K_AUDIO_FORWARD },
	{ "audio_repeat",   K_AUDIO_REPEAT },
	{ "audio_random",   K_AUDIO_RANDOM },

	{ "numpad_0",		K_NUMPAD_0 },
	{ "numpad_1",		K_NUMPAD_1 },
	{ "numpad_2",		K_NUMPAD_2 },
	{ "numpad_3",		K_NUMPAD_3 },
	{ "numpad_4",		K_NUMPAD_4 },
	{ "numpad_5",		K_NUMPAD_5 },
	{ "numpad_6",		K_NUMPAD_6 },
	{ "numpad_7",		K_NUMPAD_7 },
	{ "numpad_8",		K_NUMPAD_8 },
	{ "numpad_9",		K_NUMPAD_9 },

	{ "lights_mon_up",    K_LIGHTS_MON_UP },
	{ "lights_mon_down",  K_LIGHTS_MON_DOWN },
	{ "lights_kbd_toggle",K_LIGHTS_KBD_TOGGLE },
	{ "lights_kbd_up",    K_LIGHTS_KBD_UP },
	{ "lights_kbd_down",  K_LIGHTS_KBD_DOWN },

	{ NULL,               K_NOT_A_KEY } /* end marker */
};

int CheckKeyCodes(char* k, MMKeyCode *key)
{
	if (!key) return -1;

	if (strlen(k) == 1)
	{
		*key = keyCodeForChar(*k);
		return 0;
	}

	*key = K_NOT_A_KEY;

	struct KeyNames* kn = key_names;
	while (kn->name)
	{
		if (strcmp(k, kn->name) == 0)
		{
			*key = kn->key;
			break;
		}
		kn++;
	}

	if (*key == K_NOT_A_KEY)
	{
		return -2;
	}

	return 0;
}

int CheckKeyFlags(char* f, MMKeyFlags* flags)
{
	if (!flags) return -1;

	if (strcmp(f, "alt") == 0)
	{
		*flags = MOD_ALT;
	}
	else if(strcmp(f, "command") == 0)
	{
		*flags = MOD_META;
	}
	else if(strcmp(f, "control") == 0)
	{
		*flags = MOD_CONTROL;
	}
	else if(strcmp(f, "shift") == 0)
	{
		*flags = MOD_SHIFT;
	}
	else if(strcmp(f, "none") == 0)
	{
		*flags = (MMKeyFlags) MOD_NONE;
	}
	else
	{
		return -2;
	}

	return 0;
}

int GetFlagsFromValue(char* value[], MMKeyFlags* flags,int num){
	if (!flags) {return -1;}
		int i;
		for ( i= 0; i <num; i++){
			MMKeyFlags f = MOD_NONE;
			const int rv = CheckKeyFlags(value[i], &f);
			if (rv) return rv;

			*flags = (MMKeyFlags)(*flags | f);
		}
	return 0;
	// return CheckKeyFlags(fstr, flags);
}

// If it's not an array, it should be a single string value.
char* aKey_Tap(char *k, char* keyarr[], int num){
	MMKeyFlags flags = MOD_NONE;
	// MMKeyFlags flags = 0;
	MMKeyCode key;

			switch(GetFlagsFromValue(keyarr, &flags, num)){
			// switch (CheckKeyFlags(akey,&flags)){
				case -1:
					return "Null pointer in key flag.";
					break;
				case -2:
					return "Invalid key flag specified.";
					break;
			}

	switch(CheckKeyCodes(k, &key)){
			case -1:
				return "Null pointer in key code.";
				break;
			case -2:
				return "Invalid key code specified.";
				break;
			default:
				tapKeyCode(key, flags);
				microsleep(keyboardDelay);
		}

	return "0";
}

char* aKeyTap(char *k, char *akey, char *akeyt){
	MMKeyFlags flags = (MMKeyFlags) MOD_NONE;
	// MMKeyFlags flags = 0;
	MMKeyCode key;

	// char *k;
	// k = *kstr;
	if (strcmp(akey, "null") != 0){
		if (strcmp(akeyt, "null") == 0){
			switch (CheckKeyFlags(akey, &flags)){
				case -1:
					return "Null pointer in key flag.";
					break;
				case -2:
					return "Invalid key flag specified.";
					break;
			}
		}else{
			char* akeyarr[2] = {akey, akeyt};
			switch(GetFlagsFromValue(akeyarr, &flags, 2)){
				case -1:
					return "Null pointer in key flag.";
					break;
				case -2:
					return "Invalid key flag specified.";
					break;
			}
		}
	}

	switch(CheckKeyCodes(k, &key)){
		case -1:
			return "Null pointer in key code.";
			break;
		case -2:
			return "Invalid key code specified.";
			break;
		default:
			tapKeyCode(key, flags);
			microsleep(keyboardDelay);
	}

	return "0";
}

char* aKeyToggle(char *k, char *d, char *akey, char *akeyt){
	MMKeyFlags flags = (MMKeyFlags) MOD_NONE;
	MMKeyCode key;

	bool down;
	// char *k;
	// k = *kstr;

	if (d != 0){
		// char *d;
		// d = *dstr;
		if (strcmp(d, "down") == 0){
			down = true;
		}else if (strcmp(d, "up") == 0){
			down = false;
		}else{
			return "Invalid key state specified.";
		}
	}

	if (strcmp(akey, "null") != 0){
		if (strcmp(akeyt, "null") == 0){
			switch (CheckKeyFlags(akey, &flags)){
				case -1:
					return "Null pointer in key flag.";
					break;
				case -2:
					return "Invalid key flag specified.";
					break;
			}
		}else{
			char* akeyarr[2] = {akey, akeyt};
			switch (GetFlagsFromValue(akeyarr, &flags, 2))
				{
					case -1:
						return "Null pointer in key flag.";
						break;
					case -2:
						return "Invalid key flag specified.";
						break;
				}
		}
	}

	switch(CheckKeyCodes(k, &key)){
		case -1:
			return "Null pointer in key code.";
			break;
		case -2:
			return "Invalid key code specified.";
			break;
		default:
			toggleKeyCode(key, down, flags);
			microsleep(keyboardDelay);
	}

	return "0";
}

void aTypeString(char *str){
	typeString(str);
}

void aTypeStringDelayed(char *str, size_t cpm){
	typeStringDelayed(str, cpm);
}

void aSetKeyboardDelay(size_t val){
	keyboardDelay = val;
}