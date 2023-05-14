//author: ackerman
//Test VGA Bitluni (Ricardo Massaro minimal library vga_6bit)
//Project compatible Web Editor and ArduinoDroid (6.3.1)
#include "PS2Kbd.h"
#include <Arduino.h>
#include "gbConfig.h"
#include "testvga.h"
#include "gbGlobals.h"
#include "hardware.h"
#include "osd.h"
#include "PS2KeyCode.h"
#include "vga_6bit.h"
//#include "gb_sdl_font8x8.h"
#include "gb_sdl_font6x8.h"

#ifdef use_lib_vga8colors
 //Uso solo RGB 3 bits DAC
 //static const unsigned char pin_config[] = {
 const unsigned char pin_config[]= {
  PIN_RED_HIGH,
  PIN_GREEN_HIGH,  
  PIN_BLUE_HIGH,
  255,
  255,
  255,
  PIN_HSYNC,
  PIN_VSYNC      
 };
#endif


const unsigned char * gb_sdl_font= gb_sdl_font_6x8;

unsigned int gb_keyboard_time_cur=0;
unsigned int gb_keyboard_time_last=0;
unsigned char gb_current_ms_poll_keyboard=20; //milisegundos muestreo teclado

unsigned char gb_const_colorNormal[8];

unsigned char gb_sync_bits;
unsigned char **gb_buffer_vga;
unsigned int **gb_buffer_vga32;

const int *gb_ptrVideo_cur;

unsigned short int gb_width;
unsigned short int gb_height;

unsigned char gb_show_osd_main_menu=1;

unsigned char gb_id_sel_video_mode=0;


#ifdef use_lib_keyboard_uart
 unsigned int gb_curTime_keyboard_before_uart;
 unsigned int gb_curTime_keyboard_uart;
 void do_keyboard_uart(void);
#endif 

//*******************************
void PrepareColorsBitluniVGA()
{  
 //(color & RGBAXMask) | SBits;
 for (unsigned char i=0;i<8;i++)
 {   
  gb_const_colorNormal[i]= (i & 0x07) | gb_sync_bits;
 }
}


#ifdef use_lib_keyboard_uart
 char gb_buf_uart[BUFFER_SIZE_UART];
 unsigned char gb_rlen_uart=0;

 void do_keyboard_uart()
 {
  unsigned int contBuf=0;
  if (gb_rlen_uart>0)
  {
   while (contBuf < gb_rlen_uart)
   {
    switch (gb_buf_uart[contBuf])
    {
     
    }//fin switch gb_buf_uart
    contBuf++;
   }//fin while contBuf   
  }//fin if gb_rlen_uart
 }
#endif






void setup()
{ 
 #if defined(use_lib_log_serial) || defined(use_lib_keyboard_uart)
  Serial.begin(115200);         
  Serial.printf("HEAP BEGIN %d\r\n", ESP.getFreeHeap()); 
 #endif

 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif

 vga_init(pin_config,VgaMode_vga_mode_360x200,false,0,0,0,0,0);
 gb_ptrVideo_cur= VgaMode_vga_mode_360x200;
 gb_width= 360;
 gb_height= 200;
 gb_sync_bits= vga_get_sync_bits();
 gb_buffer_vga = vga_get_framebuffer();
 gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
 PrepareColorsBitluniVGA(); //Llamar despues de tener gb_sync_bits 
 SDLClear();

 #ifdef use_lib_log_serial
  Serial.printf("VGA %d\r\n", ESP.getFreeHeap()); 
 #endif

 kb_begin(); 

 #ifdef use_lib_log_serial  
  Serial.printf("END SETUP %d\r\n", ESP.getFreeHeap()); 
 #endif

 gb_keyboard_time_cur= gb_keyboard_time_last= millis();
 #ifdef use_lib_keyboard_uart
  gb_curTime_keyboard_before_uart= gb_curTime_keyboard_uart= gb_keyboard_time_cur;
 #endif 
}

void loop()
{
 //Todo este codigo sobra al forzar gb_show_osd_main_menu
 gb_keyboard_time_cur = millis();

 if ((gb_keyboard_time_cur - gb_keyboard_time_last) >= (gb_current_ms_poll_keyboard-1))
 {
  gb_keyboard_time_last= gb_keyboard_time_cur;

  #ifdef use_lib_keyboard_uart       
   gb_curTime_keyboard_uart= gb_keyboard_time_cur;
   if ((gb_curTime_keyboard_uart - gb_curTime_keyboard_before_uart) >= gb_current_ms_poll_keyboard_uart)
   {
    gb_curTime_keyboard_before_uart= gb_curTime_keyboard_uart;
    keyboard_uart_poll();
   }
   do_keyboard_uart();
  #endif

  do_tinyOSD();  
 }
}