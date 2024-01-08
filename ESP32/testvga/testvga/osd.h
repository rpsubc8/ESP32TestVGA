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

void PruebaModoTexto(void);

//void PutMandrilGris(void);
//void PonPixelMandril(unsigned short int x,unsigned short int y,unsigned char color);
void PutImagenTruco8(unsigned short int ancho, unsigned short int alto, const unsigned char *pData);
void SDLVGAFullClear(void);
void WaitKeypress(void);

#endif

