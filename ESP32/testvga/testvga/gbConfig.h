#ifndef _GB_CONFIG_H
 #define _GB_CONFIG_H

 //Not use double numbers (IEEE not fpu) calculate vga frequency
 //#define use_lib_fix_double_precision

 //Debug i2s bitluni vga frequency
 //#define use_lib_debug_i2s

 //Use terminal keyboard, putty
 #define use_lib_keyboard_uart
 
 //timeout read millis
 #define use_lib_keyboard_uart_timeout 0

 //log keyboard remote uart 
 //#define use_lib_log_keyboard_uart

 //Fixes error keyboards not initializing (solution dcrespo3D)
 #define FIX_PERIBOARD_NOT_INITING
 
 //Using modified bitluni library in C tiny fast
 #define use_lib_tinybitluni_fast 




 //Video mode selection
 //Select one mode and deselect other modes
 //360x200 720x400 31.3 Khz 70.3 Hz freq:28322000 pixel_clock:14161000
 #define use_lib_vga360x200x70hz_bitluni
 //320x240 640x480 31.4 Khz 60 Hz freq:25175000 pixel_clock:12587500
 //#define use_lib_vga320x240x60hz_bitluni
 //320x200 720x400 31.4 Khz 70.0 Hz freq:25175000 pixel_clock:12587500
 //#define use_lib_vga320x200x70hz_bitluni
 //320x200 70Hz freq:12587500 funciona
 //#define use_lib_vga320x200x70hz_fabgl
 //QVGA 320x240 60Hz freq:12600000 funciona
 //#define use_lib_vga320x240x60hz_fabgl


 #ifdef use_lib_vga360x200x70hz_bitluni
  #define use_lib_vga360x200  
 #else
  #ifdef use_lib_vga320x240x60hz_bitluni
   #define use_lib_vga320x240   
  #else
   #ifdef use_lib_vga320x200x70hz_bitluni
    #define use_lib_vga320x200
   #else
    #ifdef use_lib_vga320x200x70hz_fabgl
     #define use_lib_vga320x200
     #ifndef use_lib_fix_double_precision
      #define use_lib_fix_double_precision
     #endif
    #else
     #ifdef use_lib_vga320x240x60hz_fabgl
      #define use_lib_vga320x240
      #ifndef use_lib_fix_double_precision
       #define use_lib_fix_double_precision
      #endif      
     #endif
    #endif
   #endif
  #endif
 #endif


 //Usamos DAC 3 bits RGB (BGR)
 #define use_lib_vga8colors
 
 //Log output usb serial port
 #define use_lib_log_serial 

 
 //Usamos VGA y CVBS al mismo tiempo
 #define use_lib_cvbs_bitluni

 //No usamos backbuffer ahorramos RAM
 #define use_lib_cvbs_bitluni_not_backbuffer

 //mode PAL, NTSC for default
 #define use_lib_cvbs_pal 

 //Use DAC output 5 volts (not 3.2 volts) TTGO VGA32
 #define use_lib_cvbs_ttgo_vga32
 //Advertencia Usar alto brillo en DAC TTGO VGA32 5 volts (PreparaColorCVBS)
 //El valor supera 1 voltio de la norma CVBS
 //#define use_lib_cvbs_ttgo_vga32_bright  


 //Poll keyboard remote uart
 #define gb_current_ms_poll_keyboard_uart 50
 //Read keyboard every x milliseconds
 #define gb_ms_keyboard 10 
 
 
#endif
