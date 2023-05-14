#include "gbConfig.h"
#include "gbGlobals.h"

#include "osd.h"
#include "PS2Kbd.h"
#include "keys.h"
#include "hardware.h"
//#include "gb_sdl_font8x8.h"
//#include "gb_sdl_font6x8.h"
#include "vga_6bit.h"

//unsigned char gb_show_osd_main_menu=0;

#ifdef use_lib_vga8colors
 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 7 //Blanco
 //#define ID_COLOR_MAGENTA 6 //Magenta
 //#define ID_COLOR_MAGENTA 3 //Magenta
 #define ID_COLOR_MAGENTA 4 //Azul Seleccion
#else
 #define ID_COLOR_BLACK 0 //Negro
 #define ID_COLOR_WHITE 7 //Blanco
 //#define ID_COLOR_MAGENTA 6 //Magenta
 //#define ID_COLOR_MAGENTA 3 //Magenta
 #define ID_COLOR_MAGENTA 4 //Azul Seleccion
#endif 

const char *gb_cadTitle= "TEST ESP32 VGA by ackerman";

#define max_gb_osd_screen_values 5
const char * gb_osd_screen_values[max_gb_osd_screen_values]={
 "0",
 "1",
 "2", 
 "3", 
 "4"
};



#define max_gb_main_menu 13
const char * gb_main_menu[max_gb_main_menu]={
 "360x200x70hz bitluni", 
 "320x240x60hz bitluni",
 "320x240x60hz fabgl",
 "QVGA 320x240x60hz fabgl",
 "320x200x70hz bitluni",
 "320x200x70hz fabgl", 
 "360x400x70.4hz bitluni", 
 "400x300x56.2hz bitluni",
 "320x350x70hz bitluni",
 "320x400x70hz bitluni", 
 "640x400x70hz bitluni",
 "Reset",
 "Return"
};


#define max_gb_reset_menu 2
const char * gb_reset_menu[max_gb_reset_menu]={
 "Soft",
 "Hard"
};


#define max_gb_value_binary_menu 2
const char * gb_value_binary_menu[max_gb_value_binary_menu]={
 "0 (OFF)",
 "1 (ON)"
};





//********************************************************************
void SDLClear()
{
 unsigned int a32= gb_const_colorNormal[0];
 unsigned int gb_topeY= 200;
 unsigned int gb_topeX_div4= 80;
 a32= a32|(a32<<8)|(a32<<16)|(a32<<24);
 for (int y=0; y<gb_topeY; y++){
  for (int x=0; x<gb_topeX_div4; x++){
   gb_buffer_vga32[y][x]= a32;
  }
 }
}

//***********************************************************
//void jj_fast_putpixel(int x,int y,unsigned char c)
//{
// gb_buffer_vga[y][x^2]= gb_const_colorNormal[c];
//}

//*************************************************************************************
void SDLprintCharOSD(char car,int x,int y,unsigned char color,unsigned char backcolor)
{ 
// unsigned char aux = gb_sdl_font_6x8[(car-64)];
 unsigned int auxId = car << 3; //*8
 unsigned char aux;
 //unsigned char auxBit,auxColor;
 unsigned char auxColor;
 for (unsigned char j=0;j<8;j++)
 {
  aux = gb_sdl_font[auxId + j];
  for (unsigned int i=0;i<8;i++)
  {
   auxColor= ((aux>>i) & 0x01);
   //SDLputpixel(surface,x+(6-i),y+j,(auxColor==1)?color:backcolor);
   //jj_fast_putpixel(x+(6-i),y+j,(auxColor==1)?color:backcolor);
   gb_buffer_vga[(y+j)][(x+(6-i))^2]= gb_const_colorNormal[((auxColor==1)?color:backcolor)];
  }
 }
}

void SDLprintText(const char *cad,int x, int y, unsigned char color,unsigned char backcolor)
{
//SDL_Surface *surface,
// gb_sdl_font_6x8
 unsigned int auxLen= strlen(cad);
 if (auxLen>50)
  auxLen=50;
 for (unsigned int i=0;i<auxLen;i++)
 {
  SDLprintCharOSD(cad[i],x,y,color,backcolor);
  x+=7;
 }
}

//****************************
//void SDLEsperaTeclado()
//{
// //SDL_WaitEvent(gb_osd_sdl_event);
//}

#ifdef use_lib_vga360x200
 #define gb_pos_x_menu 120
#else
 #define gb_pos_x_menu 80
#endif 
#define gb_pos_y_menu 50
#define gb_osd_max_rows 10

void LineHorizontal(unsigned short int y,unsigned short int w,unsigned char c)
{
 for (unsigned short int x=0;x<w;x++)
 {
  gb_buffer_vga[y][x^2]= gb_const_colorNormal[c];
 }
}

void LineVertical(unsigned short int x,unsigned short int h,unsigned char c)
{
 for (unsigned short int y=0;y<h;y++)
 {
  gb_buffer_vga[y][x^2]= gb_const_colorNormal[c];
 }
}

void ShowInfoVideoMode()
{
 unsigned short int col=2;
 unsigned short int row=18;
 char cadDest0[40];
 char cadDest1[40];
 char cadDest2[40];
 char cadDest3[40];
 char cadDest4[40];
 unsigned int p0,p1,p2,p3;

 LineHorizontal(0,gb_width,0x07);
 LineHorizontal((gb_height-1),gb_width,0x07);

 LineVertical(0,(gb_height-1),0x07);
 LineVertical((gb_width-1),(gb_height-1),0x07);

 SDLprintText((char *)gb_main_menu[gb_id_sel_video_mode],(col<<3),(row<<3),ID_COLOR_WHITE,ID_COLOR_BLACK); 

 row++;
 sprintf(cadDest0,"hf:%d hs:%d hb:%d hp:%d",gb_ptrVideo_cur[0],gb_ptrVideo_cur[1],gb_ptrVideo_cur[2],gb_ptrVideo_cur[3]);
 SDLprintText(cadDest0,(col<<3),(row<<3),ID_COLOR_WHITE,ID_COLOR_BLACK); 
 
 row++;
 sprintf(cadDest1,"vf:%d vs:%d vb:%d vp:%d",gb_ptrVideo_cur[4],gb_ptrVideo_cur[5],gb_ptrVideo_cur[6],gb_ptrVideo_cur[7]);
 SDLprintText(cadDest1,(col<<3),(row<<3),ID_COLOR_WHITE,ID_COLOR_BLACK); 

 row++;
 sprintf(cadDest2,"vd:%d pc:%d hp:%d vp:%d",gb_ptrVideo_cur[8],gb_ptrVideo_cur[9],gb_ptrVideo_cur[10],gb_ptrVideo_cur[11]);
 SDLprintText(cadDest2,(col<<3),(row<<3),ID_COLOR_WHITE,ID_COLOR_BLACK);

 row++;
 p0= vga_get_pll_cte_p0();
 p1= vga_get_pll_cte_p1();
 p2= vga_get_pll_cte_p2();
 p3= vga_get_pll_cte_p3();
 sprintf(cadDest3,"p0:%08X p1:%08X",p0,p1);
 SDLprintText(cadDest3,(col<<3),(row<<3),ID_COLOR_WHITE,ID_COLOR_BLACK); 
 row++; 
 sprintf(cadDest4,"p2:%08X p3:%08X",p2,p3);
 SDLprintText(cadDest4,(col<<3),(row<<3),ID_COLOR_WHITE,ID_COLOR_BLACK);  

 #ifdef use_lib_log_serial
  Serial.printf("Info Video\r\n%s\r\n",cadDest0);
  Serial.printf("name:%s\r\n",gb_main_menu[gb_id_sel_video_mode]);
  Serial.printf("%s\r\n",cadDest1);
  Serial.printf("%s\r\n",cadDest2);
  Serial.printf("%s\r\n",cadDest3);
  Serial.printf("%s\r\n",cadDest4);
 #endif 
}

void OSDMenuRowsDisplayScroll(const char **ptrValue,unsigned char currentId,unsigned char aMax)
{//Dibuja varias lineas
 //jjvga vga.setTextColor(WHITE,BLACK);
 char cadDest[40];
 unsigned char auxLen;
 unsigned int xOri= (gb_width - (unsigned int)(26<<3))>>1;
 //Borro linea
 //for (int i=0;i<gb_osd_max_rows;i++)
 //{
 // //jjvga vga.setCursor(gb_pos_x_menu, gb_pos_y_menu+8+(i<<3));
 // //jjvga vga.print("                    ");
 // SDLprintText("                    ",gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),ID_COLOR_BLACK,ID_COLOR_BLACK);
 //}
 
 for (int i=0;i<gb_osd_max_rows;i++)
 {
  if (currentId < aMax)
  {
   auxLen= strlen(ptrValue[currentId]);  
   sprintf(cadDest,"%s",ptrValue[currentId]);
   memset(&cadDest[auxLen],32,(30-auxLen));   
  }
  else
  {
   memset(cadDest,32,30);   
  }
  cadDest[26]='\0';
   
  //SDLprintText(ptrValue[currentId],gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_WHITE:ID_COLOR_WHITE),((i==0)?ID_COLOR_MAGENTA:ID_COLOR_BLACK));
  //SDLprintText(cadDest,gb_pos_x_menu,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_WHITE:ID_COLOR_WHITE),((i==0)?ID_COLOR_MAGENTA:ID_COLOR_BLACK));
  SDLprintText(cadDest,xOri,gb_pos_y_menu+8+(i<<3),((i==0)?ID_COLOR_WHITE:ID_COLOR_WHITE),((i==0)?ID_COLOR_MAGENTA:ID_COLOR_BLACK));

  currentId++;
 }     
}



#ifdef use_lib_keyboard_uart
 void keyboard_uart_poll()
 {
  if(Serial.available() > 0)
  {
   gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);

   //Serial.print("OSD I received: ");
   gb_buf_uart[gb_rlen_uart]='\0';
   #ifdef use_lib_log_keyboard_uart
    Serial.printf("UART key tot:%d\r\nASCII:%s\r\n",gb_rlen_uart,gb_buf_uart);        
    for (unsigned short int i=0;i<gb_rlen_uart;i++)
    {
     Serial.printf("%02X ",gb_buf_uart[i]);
    }
    Serial.printf("\r\n"); 
   #endif 
  }
  else
  {
   gb_rlen_uart=0;
   gb_buf_uart[0]='\0';
  }  
 }

 unsigned char checkKey_uart(unsigned char scancode)
 {
  unsigned int contBuf=0;
  unsigned char toReturn= 0;
  //if(Serial.available() > 0)
  //{
  // gb_rlen_uart = Serial.readBytes(gb_buf_uart, BUFFER_SIZE_UART);
  //
  // Serial.print("OSD I received: ");
  // gb_buf_uart[gb_rlen_uart]='\0';
  // Serial.printf("OSD Tot:%d\nASCII:%s\n",gb_rlen_uart,gb_buf_uart);     
  // Serial.printf("\n"); 
   
   //for(contBuf= 0; contBuf < gb_rlen_uart; contBuf++)
   //Serial.printf("OSD check tot:%d\n",gb_rlen_uart);
   while (contBuf < gb_rlen_uart)
   {
    //Serial.printf("%02X ",gb_buf_uart[contBuf]);
    switch (gb_buf_uart[contBuf])
    {
     case 0x1B: //Arriba 1B 5B 41
      if ((contBuf+2) < gb_rlen_uart)
      {
       contBuf++;
       if (gb_buf_uart[contBuf] == 0x5B)
       {
        contBuf++;
        switch (gb_buf_uart[contBuf])
        {
         case 0x41: toReturn = (scancode == KEY_CURSOR_UP) ? 1 : 0; break; //arriba 1B 5B 41
         case 0x42: toReturn = (scancode == KEY_CURSOR_DOWN) ? 1 : 0; break; //abajo 1B 5B 42
         case 0x43: toReturn = (scancode == KEY_CURSOR_RIGHT) ? 1 : 0; break; //derecha 1B 5B 43
         case 0x44: toReturn = (scancode == KEY_CURSOR_LEFT) ? 1 : 0; break; //izquierda 1B 5B 44        
        }
       }
      }
      else
      {       
       toReturn = (scancode == KEY_ESC) ? 1 : 0; //ESC              
      }
      break;

     case 0x0D: case 0x0A: //0D 0A ENTER
      //if ((contBuf+1) < gb_rlen_uart)
      //{
      // contBuf++;
      // if (gb_buf_uart[contBuf] == 0x0A)
      // {
      //  toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      //  //contBuf++;
      // }
      //}
       toReturn = (scancode == KEY_ENTER) ? 1 : 0; //ENTER
      break;

    }
    contBuf++;
   }
  //}
  //Serial.printf("\n");
  return toReturn;
 } 
#endif 

//Maximo 256 elementos
unsigned char ShowTinyMenu(const char *cadTitle,const char **ptrValue,unsigned char aMax,short int aSel)
{
 unsigned char aReturn=0;
 unsigned char salir=0;  
 unsigned int xOri=0;
 char cadTitulo[40];
 unsigned char longitud=0;
  
 #ifdef use_lib_keyboard_uart
  unsigned int curTime_keyboard_uart;
  unsigned int curTime_keyboard_before_uart;  
 #endif 
 unsigned int curTime_keyboard;
 unsigned int curTime_keyboard_before;
 
 #ifdef use_lib_keyboard_uart
  curTime_keyboard_uart = curTime_keyboard_before_uart= millis();
 #endif
 
 curTime_keyboard= curTime_keyboard_before= millis();

 //WaitVsync();
 SDLClear();
 //WaitVsync();
 xOri= (gb_width - (unsigned int)(strlen(gb_cadTitle)<<3))>>1;
 SDLprintText(gb_cadTitle,xOri,(gb_pos_y_menu-16),ID_COLOR_WHITE,ID_COLOR_BLACK); 
 //SDLprintText(gb_cadTitle,(gb_pos_x_menu-(32)),(gb_pos_y_menu-16),ID_COLOR_WHITE,ID_COLOR_BLACK); 
 //for (int i=0;i<14;i++)
 //{ 
 // SDLprintCharOSD(' ',gb_pos_x_menu+(i<<3),gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 //} 
   
 memset(cadTitulo,' ',40);
 sprintf(cadTitulo,"%s",cadTitle);
 longitud= strlen(cadTitulo);
 cadTitulo[longitud]=' ';
 cadTitulo[26]='\0';
 xOri= (gb_width - (unsigned int)(strlen(cadTitulo)<<3))>>1;
 SDLprintText(cadTitulo,xOri,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 //SDLprintText(cadTitle,gb_pos_x_menu,gb_pos_y_menu,ID_COLOR_BLACK,ID_COLOR_WHITE);
 aReturn = (aSel!=-1)?aSel:0;
 OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);

 ShowInfoVideoMode();
 
 while (salir == 0)
 {             
  //case SDLK_UP:
  curTime_keyboard = millis();    
  if ((curTime_keyboard - curTime_keyboard_before) >= gb_current_ms_poll_keyboard)
  {   
   curTime_keyboard_before= curTime_keyboard;

   #ifdef use_lib_keyboard_uart
    curTime_keyboard_uart= curTime_keyboard;
    if ((curTime_keyboard_uart - curTime_keyboard_before_uart) >= gb_current_ms_poll_keyboard_uart)
    {
     curTime_keyboard_before_uart = curTime_keyboard_uart;
     keyboard_uart_poll();
    
     if (checkKey_uart(KEY_CURSOR_LEFT)==1)
     {
      if (aReturn>10) aReturn-=10;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_CURSOR_RIGHT)==1)
     {
      if (aReturn<(aMax-10)) aReturn+=10;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
     }  
     if (checkKey_uart(KEY_CURSOR_UP)==1)
     {
      if (aReturn>0) aReturn--;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_CURSOR_DOWN)==1)
     {
      if (aReturn < (aMax-1)) aReturn++;
      OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
     }
     if (checkKey_uart(KEY_ENTER)==1)
     {
      salir= 1;
     }
     if (checkKey_uart(KEY_ESC))
     {
      salir=1; aReturn= 255;    
     }
    }
   #endif

   if (checkAndCleanKey(KEY_CURSOR_LEFT))
   {
    if (aReturn>10) aReturn-=10;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
   }
   if (checkAndCleanKey(KEY_CURSOR_RIGHT))
   {
    if (aReturn<(aMax-10)) aReturn+=10;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);       
   }  
   if (checkAndCleanKey(KEY_CURSOR_UP))
   {
    if (aReturn>0) aReturn--;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
   }
   if (checkAndCleanKey(KEY_CURSOR_DOWN))
   {
    if (aReturn < (aMax-1)) aReturn++;
    OSDMenuRowsDisplayScroll(ptrValue,aReturn,aMax);
   }
   if (checkAndCleanKey(KEY_ENTER))
   {
    salir= 1;
   }
   //case SDLK_KP_ENTER: case SDLK_RETURN: salir= 1;break;
   if (checkAndCleanKey(KEY_ESC))
   {
    salir=1; aReturn= 255;    
   }
   //case SDLK_ESCAPE: salir=1; aReturn= 255; break;
   //default: break;
  }
 }
 gb_show_osd_main_menu= 0;
 return aReturn;
}



//Menu resetear
//void ShowTinyResetMenu()
//{
// unsigned char aSelNum;
// aSelNum= ShowTinyMenu("Reset",gb_reset_menu,max_gb_reset_menu,-1);   
// if (aSelNum == 255){
//  return; 
// }
//
// if (aSelNum == 1){
//  ESP.restart();
// }
//
//}



//*******************************************
void SDLActivarOSDMainMenu()
{     
 gb_show_osd_main_menu= true;   
}

//Very small tiny osd
void do_tinyOSD() 
{
 unsigned char aSelNum;
 if (checkAndCleanKey(KEY_F1)) 
 {
  gb_show_osd_main_menu= 1;  
  return;
 }

 if (gb_show_osd_main_menu == 1)
 {
  unsigned char auxSetVideo=0;
  unsigned char usepllcteforce=0;
  unsigned int p0=0;
  unsigned int p1=0;
  unsigned int p2=0;
  unsigned int p3=0;  

  aSelNum = ShowTinyMenu("Select Video Mode         ",gb_main_menu,max_gb_main_menu,gb_id_sel_video_mode);  
  switch (aSelNum)
  {
   case 0:
    //360x200x70hz bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_360x200;  
    gb_width= 360;
    gb_height= 200;
    auxSetVideo=1;
    gb_id_sel_video_mode= 0;
    break;
   
   case 1:
    //320x240x60hz bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_320x240; 
    gb_width= 320;
    gb_height= 240;        
    auxSetVideo=1;
    gb_id_sel_video_mode= 1;
    break;
   case 2: 
    //Misma que bitluni,cambia el calculo PLL
    //320x240x60hz fabgl
    gb_ptrVideo_cur= VgaMode_vga_mode_320x240; //320x240x60hz fabgl
    gb_width= 320;
    gb_height= 240;    
    auxSetVideo=1;
    usepllcteforce=1;
    p0= 0x000A;
    p1= 0x0057;
    p2= 0x0007;
    p3= 0x0007;
    gb_id_sel_video_mode= 2;
    break;
   case 3:
    //QVGA 320x240x60hz fabgl
    gb_ptrVideo_cur= VgaMode_vga_mode_320x240;//QVGA 320x240x60hz fabgl
    gb_width= 320;
    gb_height= 240;        
    auxSetVideo=1;
    usepllcteforce=1;
    p0=0x000A;
    p1=0x0057;
    p2=0x0007;
    p3=0x0007;
    gb_id_sel_video_mode= 3;
    break;
   case 4:
    //320x200x70hz bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_320x200;
    gb_width= 320;
    gb_height= 200;        
    auxSetVideo=1; 
    gb_id_sel_video_mode= 4;
    break;
   case 5:
    //Mismo que bitluni pero PLL esta a calzador   
    //320x200x70hz fabgl
    gb_ptrVideo_cur= VgaMode_vga_mode_320x200; //320x200x70hz fabgl
    gb_width= 320;
    gb_height= 200;        
    auxSetVideo=1;    
    usepllcteforce=1;
    p0= 0x00AE;
    p1= 0x00CF;
    p2= 0x0004;
    p3= 0x0005;
    gb_id_sel_video_mode= 5;
    break;
   case 6: 
    //360x400 bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_360x400;
    gb_width= 360;
    gb_height= 400;
    auxSetVideo=1;
    gb_id_sel_video_mode= 6;   
    break;
   case 7: 
    //400x300 bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_400x300;
    gb_width= 400;
    gb_height= 300;
    auxSetVideo=1;
    gb_id_sel_video_mode= 7;
    break;    
   case 8: 
    //320x350 bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_320x350;
    gb_width= 320;
    gb_height= 350;
    auxSetVideo=1;
    gb_id_sel_video_mode= 8;
    break;    
   case 9: 
    //320x400 bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_320x400;
    gb_width= 320;
    gb_height= 400;
    auxSetVideo=1;
    gb_id_sel_video_mode= 9;
    break; 
   case 10: 
    //640x400 bitluni
    gb_ptrVideo_cur= VgaMode_vga_mode_640x400;
    gb_width= 640;
    gb_height= 400;
    auxSetVideo=1;
    gb_id_sel_video_mode= 10;
    break;
   case 11:
    //ShowTinyResetMenu(); 
    ESP.restart();
    break;
   default: break;
  }

  if (auxSetVideo == 1)
  {
   SetVideoInterrupt(0);
   delay(100);
   //int auxY= vga_get_y_res();
   //Serial.printf("Free %d scanlines\r\n",auxY);
   //for (int i = 0; i < auxY; i++)
   //{
   // free(gb_buffer_vga[i]);
   //}
   vga_free();

   delay(100);
   vga_init(pin_config,gb_ptrVideo_cur,false,usepllcteforce,p0,p1,p2,p3);
   SetVideoInterrupt(1);

   gb_sync_bits= vga_get_sync_bits();
   gb_buffer_vga = vga_get_framebuffer();
   gb_buffer_vga32=(unsigned int **)gb_buffer_vga;
   PrepareColorsBitluniVGA(); //Llamar despues de tener gb_sync_bits 
      
   SDLClear();
   #ifdef use_lib_log_serial  
    Serial.printf("Set Video %d\r\n",aSelNum);     
    Serial.printf("RAM free %d\r\n", ESP.getFreeHeap()); 
   #endif   
  }
 }

 gb_show_osd_main_menu=1; //fuerzo hay que revisar

}
