#ifndef _TECLADO_PS2KBD_H
 #define _TECLADO_PS2KBD_H

#include "gbConfig.h"
#include "hardware.h"
//#include "keys.h" //Ya no se necesita
#include <Arduino.h>


//Def keys BEGIN
#define KEY_F1 0x05
#define KEY_F2 0x06
#define KEY_F3 0x04
#define KEY_F4 0x0C
#define KEY_F5 0x03
#define KEY_F6 0x0B
#define KEY_F7 0x83
#define KEY_F8 0x0A
#define KEY_F9 0x01
#define KEY_F10 0x09
#define KEY_F11 0x78
#define KEY_F12 0x07
#define KEY_ESC 0x76
#define KEY_CURSOR_LEFT 0x6B
#define KEY_CURSOR_DOWN 0x72
#define KEY_CURSOR_RIGHT 0x74
#define KEY_CURSOR_UP 0x75
#define KEY_ALT_GR 0x11
#define KEY_ENTER 0x5A
#define KEY_HOME 0xE06C
#define KEY_END 0xE069
#define KEY_PAGE_UP 0xE07D
#define KEY_PAGE_DOWN 0xE07A
#define KEY_PAUSE 0xE11477E1F014E077
#define KEY_BACKSPACE 0x66
#define KEY_DELETE 0x71
//Def keys END



//#define KB_INT_START attachInterrupt(digitalPinToInterrupt(KEYBOARD_CLK), kb_interruptHandler, FALLING)
//#define KB_INT_STOP detachInterrupt(digitalPinToInterrupt(KEYBOARD_CLK))

//extern byte lastcode; //Ya no se necesita

void IRAM_ATTR kb_interruptHandler(void);
void kb_begin(void);
//unsigned char isKeymapChanged(void);
unsigned char checkAndCleanKey(unsigned char scancode);
//unsigned char checkKey(unsigned char scancode);

// inject key from wiimote, for not modifying OSD code
//void emulateKeyChange(unsigned char scancode, unsigned char isdown);

#endif

