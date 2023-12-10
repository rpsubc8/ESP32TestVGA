#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 #include "gbConfig.h"

 extern unsigned char gb_vga_1bpp;
 extern unsigned char gb_vga_text;
 extern unsigned char gb_vga_videomode;
 extern const short int pin_config_bitluni[];
 extern unsigned char gb_colorBW[2]; 
 extern unsigned char gb_sdl_font_6x8_ram[2048]; //256*8
 extern unsigned long precalcula_baseh_bit;
 extern unsigned long precalcula_base_bit;
 extern unsigned long precalcula_baseh_bitI;
 extern unsigned long precalcula_base_bitI;
 extern unsigned char precalcula_optimiceModehSync2;
 extern unsigned short int precalcula_optimiceModehRes2;
 extern unsigned short int precalcula_optimiceSumHfront;


 #ifdef use_lib_keyboard_uart
  #define BUFFER_SIZE_UART 50
  extern char gb_buf_uart[BUFFER_SIZE_UART];
  extern unsigned char gb_rlen_uart;
  extern unsigned int gb_curTime_keyboard_before_uart;
  extern unsigned int gb_curTime_keyboard_uart;
 #endif


 extern unsigned char ** gb_buffer_vga;
 extern unsigned int **gb_buffer_vga32;
 extern unsigned char gb_sync_bits;

 #ifdef use_lib_cvbs_bitluni
  extern unsigned char **gb_buffer_cvbs;
  extern unsigned int **gb_buffer_cvbs32;
  extern unsigned char gb_color_cvbs[8];
 #endif 

 extern volatile unsigned char gb_cvbs_mode;
 extern volatile unsigned char gb_cvbs_shutdown;

 //PS2Kbd BEGIN
 //extern volatile unsigned char keymap[256];
 //extern volatile unsigned char oldKeymap[256];
 //extern volatile unsigned char gb_kb_bitcount;
 //extern volatile unsigned char gb_kb_incoming;
 //extern volatile unsigned int gb_kb_prev_ms; 
 //PS2Kbd END

 extern unsigned char gb_current_ms_poll_sound; //milisegundos muestreo
 extern unsigned char gb_current_ms_poll_keyboard; //milisegundos muestreo teclado

 extern unsigned char gb_show_osd_main_menu;


 extern unsigned int gb_keyboard_time_cur;
 extern unsigned int gb_keyboard_time_last;


 extern unsigned char gb_const_colorNormal[8];

 extern const unsigned char pin_config[];

 extern const unsigned char * gb_sdl_font;

 extern const unsigned int *gb_ptrVideo_cur;

 extern unsigned short int gb_width;
 extern unsigned short int gb_height; 

 extern unsigned char gb_id_sel_video_mode;
 extern unsigned char gb_id_sel_video_mode_prev; 

 extern unsigned char gb_dibuja;

 #define CVBS_MODE_PAL 0
 #define CVBS_MODE_NTSC 1

#endif


