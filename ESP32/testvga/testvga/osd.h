#ifndef OSDFILE_H
 #define OSDFILE_H

#include "gbConfig.h"


void do_tinyOSD(void);
void SDLActivarOSDMainMenu();
void SDLOSDClear(void);
//void SDLEsperaTeclado(void);
void SDLClear(void);
//void jj_fast_putpixel(int x,int y,unsigned char c);

void PrepareColorsBitluniVGA(void);
void ShowInfoVideoMode(void);




unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax);

#ifdef use_lib_keyboard_uart
 void keyboard_uart_poll(void);
#endif 

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor);
void InitModoCVBS(unsigned char opcion, unsigned char modoPALNTSC,double Vcc);

#endif

