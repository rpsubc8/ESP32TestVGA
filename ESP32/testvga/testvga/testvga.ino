//author: ackerman
//Test VGA Bitluni (Ricardo Massaro minimal library vga_6bit)
//Project compatible Web Editor and ArduinoDroid (6.3.1)
#include "gbConfig.h"
#include "hardware.h"
#include "gbGlobals.h"
#include "PS2Kbd.h"
#include <Arduino.h>
//#include "testvga.h" ////Ya no se necesita
#include "osd.h"
//#include "PS2KeyCode.h" //Ya no se necesita
#include "vga_6bit.h"
//#include "gb_sdl_font8x8.h"
#include "gb_sdl_font6x8.h"
#include "BitluniVGA.h" //Para modos interrupt
#include "testvga.h"


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


 //const int hsyncPin=23;
 //const int vsyncPin=15;
 //const short int pin_config_bitluni[]= {
 // RED_PIN_3B,
 // GRE_PIN_3B,  
 // BLU_PIN_3B,
 // -1,
 // -1,
 // -1,
 // PIN_HSYNC,
 // PIN_VSYNC      
 //}; 

 unsigned char gb_vga_1bpp=0;
 unsigned char gb_vga_text=0;
 unsigned char gb_vga_videomode= vgaMode_novideo;
 unsigned long precalcula_baseh_bit;
 unsigned long precalcula_base_bit;
 unsigned long precalcula_baseh_bitI;
 unsigned long precalcula_base_bitI;
 unsigned char precalcula_optimiceModehSync2;
 unsigned short int precalcula_optimiceModehRes2;
 unsigned short int precalcula_optimiceSumHfront;

#endif

//TaskHandle_t gb_task_fix_pll;
//volatile unsigned char gb_call_task_fix_pll_end=0;

//void Task_fix_pll_code(void * parameter);

const unsigned char * gb_sdl_font= gb_sdl_font_6x8;

unsigned int gb_keyboard_time_cur=0;
unsigned int gb_keyboard_time_last=0;
unsigned char gb_current_ms_poll_keyboard=20; //milisegundos muestreo teclado

unsigned char gb_const_colorNormal[8];

unsigned char gb_sync_bits;
unsigned char **gb_buffer_vga;
unsigned int **gb_buffer_vga32;

const unsigned int *gb_ptrVideo_cur;

unsigned short int gb_width;
unsigned short int gb_height;

unsigned char gb_show_osd_main_menu=1;

unsigned char gb_id_sel_video_mode=0;
unsigned char gb_id_sel_video_mode_prev=0;
unsigned char gb_dibuja=1; //Solucion rapida.Hay que quitar

#ifdef use_lib_cvbs_bitluni
 unsigned char **gb_buffer_cvbs;
 unsigned int **gb_buffer_cvbs32;
 unsigned char gb_color_cvbs[8];
#endif 

volatile unsigned char gb_cvbs_mode=0;
volatile unsigned char gb_cvbs_shutdown=1;


VGA3BitI vga; //Modo interrupt



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


//void Task_fix_pll_code(void * parameter)
//{    
// //unsigned int ini,fin,aux;
// gb_call_task_fix_pll_end= 0;
// //ini= micros();
// vga_init(pin_config,VgaMode_vga_mode_360x200,false,0,0,0,0,0);
// //fin=micros();
// //aux= fin-ini;
// //Serial.printf("Time vga_init %lu\r\n",aux);
// //delay(3000);
// gb_call_task_fix_pll_end= 1;
//}



void setup()
{ 
 #if defined(use_lib_log_serial) || defined(use_lib_keyboard_uart)
  Serial.begin(115200);         
  Serial.printf("HEAP BEGIN %d\r\n", ESP.getFreeHeap()); 
 #endif

 #ifdef use_lib_keyboard_uart
  Serial.setTimeout(use_lib_keyboard_uart_timeout);
 #endif

 gb_cvbs_mode=0;
 gb_cvbs_shutdown=1; 

 //gb_call_task_fix_pll_end=0;
 //xTaskCreatePinnedToCore(Task_fix_pll_code,"FixPLL",2048,NULL,0,&gb_task_fix_pll,0);
 //unsigned int time_ini_fix_pll= millis();
 //while ((gb_call_task_fix_pll_end==0)&&((millis()-time_ini_fix_pll)<2000)) { } 
 //if ((millis()-time_ini_fix_pll)>=2000)
 //{
 // Serial.printf("Timeout\r\n");
 //}
 //vTaskDelete(gb_task_fix_pll);

 //PruebaModoTexto(); 
 //vga.MODEcurrent.SetValues(VgaMode_vga_mode_400x300); //Ahorrar memoria
 //vga.init(vga.MODEcurrent, pin_config);
 //delay(100);

 gb_colorBW[0]= 0x00;
 gb_colorBW[1]= 0x07;
 memcpy(gb_sdl_font_6x8_ram,gb_sdl_font_6x8,sizeof(gb_sdl_font_6x8_ram)); 
 

 vga_init(pin_config,VgaMode_vga_mode_360x200,false,0,0,0,0,0,0);
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