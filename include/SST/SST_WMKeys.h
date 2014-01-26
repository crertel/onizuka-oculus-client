/*
	SST_WMKeys.h
	Author: Patrick Baggett <ptbaggett@762studios.com>
	Created: 5/30/2012

	Purpose: 

	Key code definitions for libsst-wm

	License:

	This program is free software. It comes without any warranty, to
	the extent permitted by applicable law. You can redistribute it
	and/or modify it under the terms of the Do What The Fuck You Want
	To Public License, Version 2, as published by Sam Hocevar. See
	http://sam.zoy.org/wtfpl/COPYING for more details.

*/

#pragma once

#ifndef _SST_WMKEY_H
#define _SST_WMKEY_H

typedef enum SST_WMKey
{
	SSTWMKEY_NONE,

	/* Whitespace and character modifier keys */
	SSTWMKEY_BACKSPACE,
	SSTWMKEY_TAB,
	SSTWMKEY_RETURN,
	SSTWMKEY_ESCAPE,
	SSTWMKEY_SPACE,
	SSTWMKEY_CAPSLOCK,


	/* Tilde + number keys */
	SSTWMKEY_TILDE,
	SSTWMKEY_0,
	SSTWMKEY_1,
	SSTWMKEY_2,
	SSTWMKEY_3,
	SSTWMKEY_4,
	SSTWMKEY_5,
	SSTWMKEY_6,
	SSTWMKEY_7,
	SSTWMKEY_8,
	SSTWMKEY_9,


	/* A - Z */
	SSTWMKEY_A,
	SSTWMKEY_B,
	SSTWMKEY_C,
	SSTWMKEY_D,
	SSTWMKEY_E,
	SSTWMKEY_F,
	SSTWMKEY_G,
	SSTWMKEY_H,
	SSTWMKEY_I,
	SSTWMKEY_J,
	SSTWMKEY_K,
	SSTWMKEY_L,
	SSTWMKEY_M,
	SSTWMKEY_N,
	SSTWMKEY_O,
	SSTWMKEY_P,
	SSTWMKEY_Q,
	SSTWMKEY_R,
	SSTWMKEY_S,
	SSTWMKEY_T,
	SSTWMKEY_U,
	SSTWMKEY_V,
	SSTWMKEY_W,
	SSTWMKEY_X,
	SSTWMKEY_Y,
	SSTWMKEY_Z,

	/* Symbol/punctuation characters */
	SSTWMKEY_COMMA,
	SSTWMKEY_PERIOD,
	SSTWMKEY_FORWARDSLASH,
	SSTWMKEY_SEMICOLON,
	SSTWMKEY_QUOTES,
	SSTWMKEY_OPENBRACKET,
	SSTWMKEY_CLOSEBRACKET,
	SSTWMKEY_BACKSLASH,
	SSTWMKEY_UNDERBAR,
	SSTWMKEY_EQUALS,


	/* Keypad character */
	SSTWMKEY_NUMLOCK,
	SSTWMKEY_KEYPAD_0,
	SSTWMKEY_KEYPAD_1,
	SSTWMKEY_KEYPAD_2,
	SSTWMKEY_KEYPAD_3,
	SSTWMKEY_KEYPAD_4,
	SSTWMKEY_KEYPAD_5,
	SSTWMKEY_KEYPAD_6,
	SSTWMKEY_KEYPAD_7,
	SSTWMKEY_KEYPAD_8,
	SSTWMKEY_KEYPAD_9,
	SSTWMKEY_KEYPAD_PERIOD,
	SSTWMKEY_KEYPAD_DIVIDE,
	SSTWMKEY_KEYPAD_MULTIPLY,
	SSTWMKEY_KEYPAD_MINUS,
	SSTWMKEY_KEYPAD_PLUS,
	SSTWMKEY_KEYPAD_ENTER,
	SSTWMKEY_KEYPAD_EQUALS,

	/* Arrow keys */
	SSTWMKEY_ARROW_UP,
	SSTWMKEY_ARROW_DOWN,
	SSTWMKEY_ARROW_LEFT,
	SSTWMKEY_ARROW_RIGHT,

	/* The page up/down block */
	SSTWMKEY_INSERT,
	SSTWMKEY_HOME,
	SSTWMKEY_END,
	SSTWMKEY_PAGEUP,
	SSTWMKEY_PAGEDOWN,
	SSTWMKEY_DELETE,

	/* Function Keys */
	SSTWMKEY_F1,
	SSTWMKEY_F2,
	SSTWMKEY_F3,
	SSTWMKEY_F4,
	SSTWMKEY_F5,
	SSTWMKEY_F6,
	SSTWMKEY_F7,
	SSTWMKEY_F8,
	SSTWMKEY_F9,
	SSTWMKEY_F10,
	SSTWMKEY_F11,
	SSTWMKEY_F12,
	SSTWMKEY_F13,
	SSTWMKEY_F14,
	SSTWMKEY_F15,

	/* Left/right keys */
	SSTWMKEY_RIGHTSHIFT,
	SSTWMKEY_LEFTSHIFT,
	SSTWMKEY_RIGHTCONTROL,
	SSTWMKEY_LEFTCONTROL,
	SSTWMKEY_RIGHTALT,
	SSTWMKEY_LEFTALT,
	SSTWMKEY_RIGHTSUPER, /* "windows" key */
	SSTWMKEY_LEFTSUPER,	/* "windows" key */

	SSTWMKEY_PRINTSCREEN,
	SSTWMKEY_SCROLLLOCK,
	SSTWMKEY_PAUSE,

	SSTWMKEY_MAX
} SST_WMKey;

#endif

