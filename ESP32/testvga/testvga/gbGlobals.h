#ifndef _GB_GLOBALS_H
 #define _GB_GLOBALS_H
 #include "gbConfig.h"


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

 extern volatile unsigned char keymap[256];
 extern volatile unsigned char oldKeymap[256];

 extern unsigned char gb_current_ms_poll_sound; //milisegundos muestreo
 extern unsigned char gb_current_ms_poll_keyboard; //milisegundos muestreo teclado

 extern unsigned char gb_show_osd_main_menu;


 extern unsigned int gb_keyboard_time_cur;
 extern unsigned int gb_keyboard_time_last;


 extern unsigned char gb_const_colorNormal[8];

 extern const unsigned char pin_config[];

 extern const unsigned char * gb_sdl_font;

 extern const int *gb_ptrVideo_cur;

 extern unsigned short int gb_width;
 extern unsigned short int gb_height; 

 extern unsigned char gb_id_sel_video_mode;

#endif


