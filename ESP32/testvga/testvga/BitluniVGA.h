#ifndef _BITLUNI_VGA_H
 #define _BITLUNI_VGA_H
//	Author: bitluni 2019
//	License: 
//	Creative Commons Attribution ShareAlike 4.0
//	https://creativecommons.org/licenses/by-sa/4.0/
//	
//	For further details check out: 
//		https://youtube.com/bitlunislab
//		https://github.com/bitluni
//		http://bitluni.net

//JJ #pragma once

//#include "BitluniI2S.h"
#include "gbGlobals.h"
#include "gb_sdl_font6x8.h"
#include <Arduino.h>

//BitluniI2S.h include BEGIN
#include "esp_heap_caps.h"
#include "soc/soc.h"
#include "soc/gpio_sig_map.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "soc/io_mux_reg.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "rom/lldesc.h"
//BitluniI2S.h include END


//JJ #include "Mode.h"
//JJ #include "PinConfig.h"


//Modos de video globales para modos de interrupcion
#define vgaMode_novideo 0
#define vgaMode_TextModeSimple 1
#define vgaMode_TextModeSimpleRetro 2
#define vgaMode_TextMode3bpp 3
#define vgaMode_TextMode3bppRetro 4
#define vgaMode_Bitmap1bpp 5







//BitluniI2S.h include BEGIN
//BitluniDMABufferDescriptor.h BEGIN
class DMABufferDescriptor : protected lldesc_t
{
  public:
	static void *allocateBuffer(int bytes, bool clear = true, unsigned long clearValue = 0)
	{
		bytes = (bytes + 3) & 0xfffffffc;
		void *b = heap_caps_malloc(bytes, MALLOC_CAP_DMA);
//JJ		if (!b) //Si no puede que reviente, a quien le importa
//JJ		{
//JJ			//JJ DEBUG_PRINTLN("Failed to alloc dma buffer");
//JJ		 Serial.printf("Fail\r\n");
//JJ		}
		if (clear)
			for (int i = 0; i < bytes / 4; i++)
				((unsigned long *)b)[i] = clearValue;
		return b;
	}

 
//  //No lo necesito
// 	static void **allocateDMABufferArray(int count, int bytes, bool clear = true, unsigned long clearValue = 0)
// 	{
// 		void **arr = (void **)malloc(count * sizeof(void *));
// //JJ		if(!arr) //Si no puede que reviente, a quien le importa
// //JJ		{
// //JJ		 //JJ ERROR("Not enough DMA memory");
// //JJ		 Serial.println("DMA\r\n");
// //JJ		 delay(3000); 
// //JJ		 throw 0;
// //JJ		}
// 		for (int i = 0; i < count; i++)
// 		{
// 			arr[i] = DMABufferDescriptor::allocateBuffer(bytes, true, clearValue);
// //JJ			if(!arr[i]) //Si no puede que reviente, a quien le importa
// //JJ			{
// //JJ		     //JJ ERROR("Not enough DMA memory");
// //JJ			 Serial.println("DMA\r\n");
// //JJ			 delay(3000);
// //JJ			 throw 0;
// //JJ			}
// 		}
// 		return arr;
// 	}
	

	void setBuffer(void *buffer, int bytes)
	{
		length = bytes;
		size = length;
		buf = (uint8_t *)buffer;
	}

	void *buffer() const
	{
		return (void *)buf;
	}

	void init()
	{
		length = 0;
		size = 0;
		owner = 1;
		sosf = 0;
		buf = (uint8_t *)0;
		offset = 0;
		empty = 0;
		eof = 1;
		qe.stqe_next = 0;
	}

	static DMABufferDescriptor *allocateDescriptors(int count)
	{
		DMABufferDescriptor *b = (DMABufferDescriptor *)heap_caps_malloc(sizeof(DMABufferDescriptor) * count, MALLOC_CAP_DMA);
//JJ		if (!b) //Si no puede que reviente, a quien le importa
//JJ		{
//JJ		 //DEBUG_PRINTLN("Failed to alloc DMABufferDescriptors");
//JJ		 Serial.print("Failed\r\n");
//JJ		}
		for (int i = 0; i < count; i++)
			b[i].init();
		return b;
	}

	static DMABufferDescriptor *allocateDescriptor(int bytes, bool allocBuffer = true, bool clear = true, unsigned long clearValue = 0)
	{
		bytes = (bytes + 3) & 0xfffffffc;
		DMABufferDescriptor *b = (DMABufferDescriptor *)heap_caps_malloc(sizeof(DMABufferDescriptor), MALLOC_CAP_DMA);
//JJ		if (!b) //Si no puede que reviente, a quien le importa
//JJ		{
//JJ		 //JJ DEBUG_PRINTLN("Failed to alloc DMABufferDescriptor");
//JJ		 Serial.print("Failed\r\n");
//JJ		}
		b->init();
		if (allocateBuffer)
			b->setBuffer(allocateBuffer(bytes, clear, clearValue), bytes);
		return b;
	}

	void next(DMABufferDescriptor &next)
	{
		qe.stqe_next = &next;
	}

	int sampleCount() const
	{
		//JJ return length / 4;
		return length>>2;
	}

	void destroy()
	{
		if (buf)
		{
			free((void *)buf);
			buf = 0;
		}
		free(this);
	}
};
//BitluniDMABufferDescriptor.h END




class I2S
{
  public:
	int i2sIndex;
	intr_handle_t interruptHandle;
	int dmaBufferDescriptorCount;
	int dmaBufferDescriptorActive;
	DMABufferDescriptor *dmaBufferDescriptors;
	volatile bool stopSignal;

	/// hardware index [0, 1]
	I2S(const int i2sIndex = 0);
	void reset();

	void stop();

	void i2sStop();
	void startTX();
	void startRX();

	void resetDMA();
	void resetFIFO();
	//JJ bool initParallelOutputMode(const int *pinMap, long APLLFreq = 1000000, const int bitCount = 8, int wordSelect = -1, int baseClock = -1);
	//bool initParallelOutputMode(const short int *pinMap, long APLLFreq = 1000000, const unsigned char bitCount = 8, int wordSelect = -1, short int baseClock = -1);
	bool initParallelOutputMode(const unsigned char *pinMap, long APLLFreq = 1000000, const unsigned char bitCount = 8, int wordSelect = -1, short int baseClock = -1);
	bool initSerialOutputMode(int dataPin, const int bitCount = 8, int wordSelect = -1, int baseClock = -1);
	//bool initParallelInputMode(const int *pinMap, long sampleRate = 1000000, const int bitCount = 8, int wordSelect = -1, int baseClock = -1);
	virtual DMABufferDescriptor *firstDescriptorAddress() const;

	void allocateDMABuffers(int count, int bytes);
	void deleteDMABuffers();
	virtual void getClockSetting(long *sampleRate, int *n, int *a, int *b, int *div);

	void (*interruptStaticChild)(void *arg) = 0;
	void FreeInterrupt(void); //Libero recursos(coexistir normal con interrupt)
	void SetVideoInterrupt(unsigned char auxState); //coexistir normal con interrupt

  protected:
	virtual bool useInterrupt();
	void setAPLLClock(long sampleRate, int bitCount);
	void setClock(long sampleRate, int bitCount, bool useAPLL = true);
	
  private:
	static void IRAM_ATTR interruptStatic(void *arg);
};
//BitluniI2S.h include END




//BEGIN mode.h
class Mode
{//JJ const Mode VGA::MODE640x350(16, 96, 48, 640, 37, 2, 60, 350, 1, 25175000, 0, 1);
  public:
	unsigned char hFront; //JJ int hFront;
	unsigned char hSync; //JJ int hSync;
	unsigned char hBack; //JJ int hBack;
	unsigned short int hRes; //JJ int hRes;
	unsigned char vFront; //JJ int vFront;
	unsigned char vSync; //JJ int vSync;
	unsigned char vBack; //JJ int vBack;
	unsigned short int vRes; //JJ int vRes;
	unsigned char vDiv; //JJ int vDiv;
	unsigned long pixelClock;
	unsigned char hSyncPolarity; //JJ int hSyncPolarity;
	unsigned char vSyncPolarity; //JJ int vSyncPolarity;
	//JJ float aspect; //No lo necesito
	//JJ short int activeLineCount; //JJ int activeLineCount; //No lo necesito
	Mode(
		const unsigned char hFront = 0, //JJ const int hFront = 0,
		const unsigned char hSync = 0, //JJ const int hSync = 0,
		const unsigned char hBack = 0, //JJ const int hBack = 0,
		const unsigned short int hRes = 0, //JJconst int hRes = 0,
		const unsigned char vFront = 0, //JJ const int vFront = 0,
		const unsigned char vSync = 0, //JJ const int vSync = 0,
		const unsigned char vBack = 0, //JJ const int vBack = 0,
		const unsigned short int vRes = 0, //JJ const int vRes = 0,
		const unsigned char vDiv = 1, //JJ const int vDiv = 1,
		const unsigned long pixelClock = 0, //JJ const unsigned long pixelClock = 0,
		const unsigned char hSyncPolarity = 1, //JJ const int hSyncPolarity = 1,
		const unsigned char vSyncPolarity = 1 //JJ const int vSyncPolarity = 1,
		//JJconst float aspect = 1.f //No lo necesito
		)
		: hFront(hFront),
		  hSync(hSync),
		  hBack(hBack),
		  hRes(hRes),
		  vFront(vFront),
		  vSync(vSync),
		  vBack(vBack),
		  vRes(vRes),
		  vDiv(vDiv),
		  pixelClock(pixelClock),
		  hSyncPolarity(hSyncPolarity),
		  vSyncPolarity(vSyncPolarity)
		  //JJ ,
		  //JJ aspect(aspect), //No lo necesito
		  //JJ activeLineCount(vRes / vDiv) //No lo necesito
	{
	}

	//JJ int maxXRes() const
	//JJ {
	//JJ 	return (int(hRes * 19673499. / pixelClock) & 0xfffffffe);
	//JJ }

	int linesPerField() const
	{
		return vFront + vSync + vBack + vRes;
	}

	int pixelsPerLine() const
	{
		return hFront + hSync + hBack + hRes;
	}

	Mode custom(int xres, int yres, int fixedYDivider = 0) const
	{
		xres = (xres + 3) & 0xfffffffc;
		float f = float(xres) / hRes;
		int hs = int(hSync * f + 3) & 0xfffffffc;
		int hb = int((hSync + hBack - hs / f) * f + 3) & 0xfffffffc;
		int hr = xres;
		int hf = int((pixelsPerLine() - (hs + hb + hr) / f) * f + 3) & 0xfffffffc;
		
		int vd = fixedYDivider ? fixedYDivider : (vRes / yres);
		int vr = yres * vd;
		int vf = vFront + vRes / 2 - vr / 2;
		int vb = vBack + vRes / 2 - (vr - vr / 2);
		long pc = long(pixelClock * f);
		Serial.printf("Modo custom video\r\n");
		Serial.printf("hf:%d hs:%d hb:%d hr:%d\r\n",hf, hs, hb, hr);
		Serial.printf("vf:%d vSync:%d vb:%d vr:%d\r\n", vf, vSync, vb, vr);
		Serial.printf("vd:%d pc:%ld\r\n", vd, pc);
		Serial.printf("hSyncPolarity:%d vSyncPolarity:%d\r\n",hSyncPolarity, vSyncPolarity);
		return Mode(hf, hs, hb, hr, vf, vSync, vb, vr, vd, pc, hSyncPolarity, vSyncPolarity);
	}

	void SetValues(const unsigned int *p)
	{
	 hFront= p[0];
	 hSync= p[1];
	 hBack= p[2];
	 hRes= p[3];
	 vFront= p[4];
	 vSync= p[5];
	 vBack= p[6];
	 vRes= p[7];
	 vDiv= p[8];
	 pixelClock= p[9];
	 hSyncPolarity= p[10];
	 vSyncPolarity= p[11];
	 Serial.printf("Mode SetValues\r\n");
	}
	
};
//END Mode.h






class VGA : public I2S
{
  public:
	VGA(const int i2sIndex = 0);
	void setLineBufferCount(int lineBufferCount);
	//JJ bool init(const Mode &mode, const int *pinMap, const int bitCount, const int clockPin = -1);
	//bool init(const Mode &mode, const short int *pinMap, const unsigned char bitCount, const short int clockPin = -1);
	bool init(const Mode &mode, const unsigned char *pinMap, const unsigned char bitCount, const short int clockPin = -1);
	//JJ virtual bool init(const Mode &mode, const PinConfig &pinConfig) = 0;
    

//JJ	static const Mode MODE320x480;
//JJ	static const Mode MODE320x240;
//JJ	static const Mode MODE320x120;
//JJ	static const Mode MODE320x400;
//JJ	static const Mode MODE320x200;
//JJ	static const Mode MODE360x400;
//JJ	static const Mode MODE360x200;
//JJ	static const Mode MODE360x350;
//JJ	static const Mode MODE360x175;

//JJ	static const Mode MODE320x350;
//JJ	static const Mode MODE320x175;

//JJ	static const Mode MODE400x300;
	static Mode MODEcurrent; //Modo estatico variable (ahorro RAM)
//JJ	static const Mode MODE400x150;
//JJ	static const Mode MODE400x100;
//JJ	static const Mode MODE200x150;

//JJ	static const Mode MODE500x480;
//JJ	static const Mode MODE500x240;

//JJ	static const Mode MODE1280x1024;
//JJ	static const Mode MODE1280x960;
//JJ	static const Mode MODE1280x800;
//JJ	static const Mode MODE1024x768;
//JJ	static const Mode MODE800x600;
//JJ	static const Mode MODE720x400;
//JJ	static const Mode MODE720x350;
//JJ	static const Mode MODE640x480;
//JJ	static const Mode MODE640x400;
//JJ	static const Mode MODE640x350; //Cada tipo Mode consume 56 bytes RAM

	//JJ static const PinConfig VGAv01;
	//JJ static const PinConfig VGABlackEdition;
	//JJ static const PinConfig VGAWhiteEdition;
	//JJ static const PinConfig PicoVGA;


	Mode mode;

	virtual int bytesPerSample() const = 0;

  protected:
	
	virtual void initSyncBits() = 0;
	virtual long syncBits(bool h, bool v) = 0;
 
	int lineBufferCount;
	//int vsyncPin; //No lo necesito
	//int hsyncPin;
	int currentLine;
	long vsyncBit;
	long hsyncBit;
	long vsyncBitI;
	long hsyncBitI;

	int totalLines;
	volatile bool vSyncPassed;

	void *vSyncInactiveBuffer;
	void *vSyncActiveBuffer;
	void *inactiveBuffer;
	void *blankActiveBuffer;

	void allocateLineBuffers(const int lines);
	virtual void allocateLineBuffers();
	//JJ virtual void allocateLineBuffers(void **frameBuffer); //No lo necesito
	virtual void propagateResolution(const int xres, const int yres) = 0;

  protected:
	virtual void interrupt();
	virtual void vSync();
	//virtual void interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits);
	virtual void interruptPixelLine(unsigned short int y, unsigned long *pixels, unsigned long syncBits);
};


/*
//BitluniVGA3BitI.h BEGIN
//#include "BitluniVGA.h"
#include "BitluniGraphicsR1G1B1A1.h"

class VGA3BitI : public VGA, public GraphicsR1G1B1A1
{  
  public:    
	VGA3BitI()	//8 bit based modes only work with I2S1
		: VGA(1)
	{
		interruptStaticChild = &VGA3BitI::interrupt;
	}

//JJ	bool init(const Mode &mode, const int RPin, const int GPin, const int BPin, const int hsyncPin, const int vsyncPin, const int clockPin = -1)
//JJ	{
//JJ		int pinMap[8] = {
//JJ			RPin,
//JJ			GPin,
//JJ			BPin,
//JJ			-1, -1, -1,
//JJ			hsyncPin, vsyncPin
//JJ		};
//JJ		return VGA::init(mode, pinMap, 8, clockPin);
//JJ	}

//JJ	bool init(const Mode &mode, const PinConfig &pinConfig)
//JJ	{
//JJ		int pins[8];
//JJ		pinConfig.fill3Bit(pins);
//JJ		return VGA::init(mode, pins, 8, pinConfig.clock);
//JJ	}

	bool init(const Mode &mode, const short int *pinConfig)
	{
		return VGA::init(mode, pinConfig, 8, -1);
	}

	virtual void initSyncBits()
	{
		hsyncBitI = mode.hSyncPolarity ? 0x40 : 0;
		vsyncBitI = mode.vSyncPolarity ? 0x80 : 0;
		hsyncBit = hsyncBitI ^ 0x40;
		vsyncBit = vsyncBitI ^ 0x80;
	}

	virtual long syncBits(bool hSync, bool vSync)
	{
		return ((hSync ? hsyncBit : hsyncBitI) | (vSync ? vsyncBit : vsyncBitI)) * 0x1010101;
	}

	virtual int bytesPerSample() const
	{
		return 1;
	}

	//JJ virtual float pixelAspect() const
	//JJ {
	//JJ 	return 1;
	//JJ }

	virtual void propagateResolution(const int xres, const int yres)
	{
		setResolution(xres, yres);
	}

	virtual void show(bool vSync = false)
	{
		if (!frameBufferCount)
			return;
		if (vSync)
		{
			vSyncPassed = false;
			while (!vSyncPassed)
				delay(0);
		}
		Graphics::show(vSync);
	}	

  protected:
	bool useInterrupt()
	{ 
		return true; 
	};

	static void interrupt(void *arg);

	//JJ static void interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits, void *arg);
	static void interruptPixelLine(unsigned short int y, unsigned long *pixels, unsigned long syncBits, void *arg);
};

//BitluniVGA3BitI.h END
*/



























//BitluniVGA3BitI.h BEGIN
//BitluniGraphics.h BEGIN
template<typename Color>
//JJ class Graphics: public ImageDrawer //No necesito nada de imagenes
class Graphics
{
  public:
	//JJ int cursorX, cursorY, cursorBaseX;
	//JJ long frontColor, backColor;
	//JJ Font *font; //no necesito fuentes
	unsigned char frameBufferCount; //JJ int frameBufferCount;
	unsigned char currentFrameBuffer; //JJ int currentFrameBuffer;
	Color **frameBuffers[3];	//No se necesita 3, solo es 1,frontBuffer y backBuffer es el mismo frameBuffers
	Color **frontBuffer;
	Color **backBuffer;
	//JJ bool autoScroll;

	short int xres; //JJ int xres;
	short int yres; //JJ int yres;

    void FreeGraphicsRAM()
	{//Libero RAM del bitmap grafico Texto o grafico 1 bit
     unsigned int totalY= yres;
	 if (gb_vga_text==1)
	 {
      int yresmod= (yres%8);
	  totalY= (yresmod>0)? (yres/8)+1 : (yres/8);	  
	 }
	 else
	 {	
	  if (gb_vga_1bpp==1)
	  {
	   totalY= yres;
	  }
	 }

	 Serial.printf("BitluniVGA free voy a borrar:%d scanlines Contbuffer:%d\r\n",totalY,frameBufferCount);

	 for (unsigned char j=0;j<frameBufferCount;j++)
	 {
	  if (frameBuffers[j] != NULL)
      {//Solo deberia liberar el primero
       for (unsigned int i = 0; i < totalY; i++)
       {
        free(frameBuffers[j][i]);
        frameBuffers[j][i]= NULL;
       }  
       free(frameBuffers[j]);
       frameBuffers[j]= NULL;
      }
	 }
	 Serial.printf("BitluniVGA free %d scanlines\r\n",totalY);
	}
	void dotFast(short int x, short int y, Color color); //virtual void dotFast(int x, int y, Color color) = 0;
	void dot(short int x, short int y, Color color); //JJ virtual void dot(int x, int y, Color color) = 0;
	void dotAdd(short int x, short int y, Color color); //JJ virtual void dotAdd(int x, int y, Color color) = 0;
	void dotMix(short int x, short int y, Color color); //JJ virtual void dotMix(int x, int y, Color color) = 0;
	virtual Color get(int x, int y) = 0;
	virtual Color** allocateFrameBuffer() = 0;
	virtual Color** allocateFrameBuffer(int xres, int yres, Color value)
	{
		Color** frame = (Color **)malloc(yres * sizeof(Color *));
		if(!frame)
		{
		 //JJ ERROR("Not enough memory for frame buffer");				
		 Serial.printf("mem\r\n");
		 delay(3000);
		 throw 0;
		}

        Serial.printf("allocateFrameBuffer xres:%d yres:%d size:%d\r\n",xres,yres,(yres*xres*sizeof(Color)));

		for (int y = 0; y < yres; y++)
		{
			frame[y] = (Color *)malloc(xres * sizeof(Color));
			if(!frame[y])
			{
			 //JJ ERROR("Not enough memory for frame buffer");
		     Serial.print("mem\r\n");
		     delay(3000);
		     throw 0;			 
			}
			for (int x = 0; x < xres; x++)
				frame[y][x] = value;
		}

		//Serial.printf("frameBuffers count:%d size:%d frontBuffer size:%d backBuffer size:%d\r\n",frameBufferCount,sizeof(frameBuffers),sizeof(frontBuffer),sizeof(backBuffer));						

		return frame;
	}
	virtual Color RGBA(int r, int g, int b, int a = 255) const = 0;
	virtual int R(Color c) const = 0;
	virtual int G(Color c) const = 0;
	virtual int B(Color c) const = 0;
	virtual int A(Color c) const = 0;
	Color RGB(unsigned long rgb) const 
	{
		return RGBA(rgb & 255, (rgb >> 8) & 255, (rgb >> 16) & 255);
	}
	Color RGBA(unsigned long rgba) const 
	{
		return RGBA(rgba & 255, (rgba >> 8) & 255, (rgba >> 16) & 255, rgba >> 24);
	}
	Color RGB(int r, int g, int b) const 
	{
		return RGBA(r, g, b);
	}

	void setFrameBufferCount(unsigned char i)
	{
	 	frameBufferCount = i > 3 ? 3 : i;
	}

	virtual void show(bool vSync = false)
	{
		if(!frameBufferCount)
		 	return;
		currentFrameBuffer = (currentFrameBuffer + 1) % frameBufferCount;
		frontBuffer = frameBuffers[currentFrameBuffer];
		backBuffer = frameBuffers[(currentFrameBuffer + frameBufferCount - 1) % frameBufferCount];
	}

	//JJ Graphics(int xres = 0, int yres = 0)
	Graphics(short int xres = 0, short int yres = 0)
	{
		this->xres = xres;
		this->yres = yres;
		//JJ font = 0; //No necesito fuentes
		//JJ cursorX = cursorY = cursorBaseX = 0;
		//JJ frontColor = -1;
		//JJ backColor = 0;
		frameBufferCount = 1;
		//JJ for(int i = 0; i < 3; i++)
		for(unsigned char i = 0; i < 3; i++)
		{
		  frameBuffers[i] = 0;
		}		

		frontBuffer = 0;
		backBuffer = 0;
		//JJ autoScroll = true;		
	}

	virtual bool allocateFrameBuffers()
	{
		if(yres <= 0 || xres <= 0)
			return false;
		for(int i = 0; i < frameBufferCount; i++)
			frameBuffers[i] = allocateFrameBuffer();		

		currentFrameBuffer = 0;
		show();
		return true;
	}

	virtual void setResolution(int xres, int yres)
	{
		this->xres = xres;
		this->yres = yres;
		allocateFrameBuffers();
	}

	virtual void clear(Color color = 0)
	{
		for (int y = 0; y < yres; y++)
			for (int x = 0; x < xres; x++)
				dotFast(x, y, color);     
	}	

    //virtual Color R2G2B2A2ToColor(unsigned char c)
	//{
	//	int r = ((int(c) & 3) * 255 + 1) / 3;
	//	int g = (((int(c) >> 2) & 3) * 255 + 1) / 3;
	//	int b = (((int(c) >> 4) & 3) * 255 + 1) / 3;
	//	int a = (((int(c) >> 6) & 3) * 255 + 1) / 3;
	//	return RGBA(r, g, b, a);
	//}
};
//BitluniGraphics.h END






//BitluniGraphicsR1G1B1A1.h BEGIN
class GraphicsR1G1B1A1: public Graphics<unsigned char>
{
	public:
	typedef unsigned char Color;
	GraphicsR1G1B1A1()
	{
		//JJ frontColor = 0xf;
	}

	virtual int R(Color c) const
	{
		return (c & 1) * 255;
	}
	virtual int G(Color c) const
	{
		return (c & 2) ? 255 : 0;
	}
	virtual int B(Color c) const
	{
		return (c & 4) ? 255 : 0;
	}
	virtual int A(Color c) const
	{
		return (c & 8) ? 255 : 0;
	}

	virtual Color RGBA(int r, int g, int b, int a = 255) const
	{
		return ((r >> 7) & 1) | ((g >> 6) & 2) | ((b >> 5) & 4) | ((a >> 4) & 8);
	}

	//JJ virtual void dotFast(int x, int y, Color color)
	void dotFast(short int x, short int y, Color color)
	{
		if(x & 1)
			backBuffer[y][x >> 1] =  (backBuffer[y][x >> 1] & 0xf) | (color << 4);
		else
			backBuffer[y][x >> 1] = (backBuffer[y][x >> 1] & 0xf0) | (color & 0xf);
	}

	//JJ virtual void dot(int x, int y, Color color)
	void dot(short int x, short int y, Color color)
	{
		if ((unsigned int)x < xres && (unsigned int)y < yres)
		{
			if(x & 1)
				backBuffer[y][x >> 1] = (backBuffer[y][x >> 1] & 0xf) | (color << 4);
			else
				backBuffer[y][x >> 1] = (backBuffer[y][x >> 1] & 0xf0) | (color & 0xf);
		}
	}

    //JJ virtual void dot_text(int x, int y, char aCar)
	void dot_text(short int x, short int y, char aCar, unsigned char color,unsigned char backcolor)
	{	 	 
	 //400x300  50x37
	 if ((unsigned int)x < (xres>>3) && (unsigned int)y < (yres>>3))
	 {
	  switch (gb_vga_videomode)
	  {
       case vgaMode_TextModeSimple: case vgaMode_TextModeSimpleRetro:
	    backBuffer[y][x]= (unsigned char)aCar;
		break;
	   case vgaMode_TextMode3bpp: case vgaMode_TextMode3bppRetro:
	    backBuffer[y][(x<<1)]= (unsigned char)aCar;
		backBuffer[y][(x<<1)+1]= (unsigned char)((color & 0x07) | ((backcolor & 0x07)<<4));
	    break;
	  }
	 }
	}

    void dot_text_cad(short int x, short int y, char *cad)
	{	 	 
	 //400x300  50x37	 
	 if ((unsigned int)x < (xres>>3) && (unsigned int)y < (yres>>3))
	 {	  
	  unsigned char lon= strlen(cad);
      for (unsigned char i=0;i<lon;i++)
	  {
       dot_text(x+i,y,cad[i],7,0);
	  }
	 }
	}

    void lineVertical_jj_1bpp(unsigned short int x, unsigned short int alto)
	{
	 if (gb_vga_videomode == vgaMode_Bitmap1bpp)
	 {
      for(unsigned short int y=1;y<alto;y++)
	  {
	   if (x==0)
	   {
        backBuffer[y][0]= 0x01;
	   }
	   else
	   {
	    backBuffer[y][(x>>3)]= 0x80;
	   }
	  }
	 }		
	}

    void lineHorizontal_jj_1bpp(short int y, unsigned short int ancho, unsigned char aColor)
	{	
	 unsigned short int tope= ancho>>3;
	 if (gb_vga_videomode == vgaMode_Bitmap1bpp)
	 {
      for(unsigned short int i=0;i<tope;i++) 
	  {
	   backBuffer[y][i]= (aColor==1)?0xFF:0;
	  }
	 }
	}

    void dot_char1bpp(unsigned short int x, unsigned short int y, unsigned char idCar, unsigned char aColor, unsigned char aBackcolor)
	{
     unsigned int auxId = ((unsigned int)idCar) << 3;
     unsigned char aux,auxFin;
     unsigned char auxColor;
	 unsigned char a0,a1,a2,a3,a4,a5,a6,a7;
	 if (gb_vga_videomode == vgaMode_Bitmap1bpp)
	 {
	  for (unsigned int j=0;j<8;j++)
	  {
	   aux = gb_sdl_font[auxId + j];
	   a7= (((aux>>7)&0x01) == 1) ? aColor:aBackcolor;
	   a6= (((aux>>6)&0x01) == 1) ? aColor:aBackcolor;
	   a5= (((aux>>5)&0x01) == 1) ? aColor:aBackcolor;
	   a4= (((aux>>4)&0x01) == 1) ? aColor:aBackcolor;
	   a3= (((aux>>3)&0x01) == 1) ? aColor:aBackcolor;
	   a2= (((aux>>2)&0x01) == 1) ? aColor:aBackcolor;
	   a1= (((aux>>1)&0x01) == 1) ? aColor:aBackcolor;
	   a0= ((aux&0x01) == 1) ? aColor:aBackcolor;
	   auxFin= a7 | (a6<<1) | (a5<<2) | (a4<<3) | (a3<<4) | (a2<<5) | (a1<<6) | (a0<<7);
	   backBuffer[y+j][(x>>3)]= auxFin;
	  }
	 }
	 //10001000
	}

	//JJ virtual void dot_jj(int x, int y, Color color)
	void dot_jj(short int x, short int y, Color color)
	{
	 unsigned char mod;
	 unsigned int cor;
	 if ((unsigned int)x < xres && (unsigned int)y < yres)
	 {	  			
	  cor= x>>3; //cor= x/8;
	  mod= (x&0x07); //mod= (x%8);

      if (color==0)
	  {
	   backBuffer[y][cor]= backBuffer[y][cor] & (~(1<<mod));
	  }
	  else
	  {
       backBuffer[y][cor]= backBuffer[y][cor] | (1<<mod);
	  }

	  //if(x & 1)
	  // backBuffer[y][x >> 1] = (backBuffer[y][x >> 1] & 0xf) | (color << 4);
	  //else
	  // backBuffer[y][x >> 1] = (backBuffer[y][x >> 1] & 0xf0) | (color & 0xf);
	 }
	}

	//JJ virtual void dotAdd(int x, int y, Color color)
	void dotAdd(short int x, short int y, Color color)
	{
		if ((unsigned int)x < xres && (unsigned int)y < yres)
		{
			if(x & 1)
				backBuffer[y][x >> 1] = backBuffer[y][x >> 1] | (color << 4);
			else
				backBuffer[y][x >> 1] = backBuffer[y][x >> 1] | (color & 0xf);
		}
	}
	
	//JJ virtual void dotMix(int x, int y, Color color)
	void dotMix(short int x, short int y, Color color)
	{
		if ((unsigned int)x < xres && (unsigned int)y < yres && (color & 8) != 0)
		{
			if(x & 1)
				backBuffer[y][x >> 1] = (backBuffer[y][x >> 1] & 0xf) | (color << 4);
			else
				backBuffer[y][x >> 1] =  (backBuffer[y][x >> 1] & 0xf0) | (color & 0xf);
		}	
	}
	
	virtual Color get(int x, int y)
	{
		if ((unsigned int)x < xres && (unsigned int)y < yres)
		{
			if(x & 1)
				return backBuffer[y][x >> 1] = backBuffer[y][x >> 1] >> 4;
			else
				return backBuffer[y][x >> 1] = backBuffer[y][x >> 1] & 0xf;
		}
		return 0;
	}

	virtual void clear(Color color = 0)
	{
     if (gb_vga_text==1)
	 {
	  //for (unsigned short int y = 0; y < this->yres/8; y++)
	  for (unsigned short int y = 0; y < (this->yres>>3); y++)
	  {
	   //for (unsigned short int x = 0; x < this->xres / 8; x++)
	   for (unsigned short int x = 0; x < (this->xres>>3); x++)
	   {
	    this->backBuffer[y][x] = 0; //Lo borro todo no dejo color (chapuza rapida)
	   }
	  }		 
	  return;	 
	 }

	 if (gb_vga_1bpp==1)
	 {
	  for (unsigned short int y = 0; y < this->yres; y++)
	  {
	   //for (unsigned short int x = 0; x < this->xres / 8; x++)
	   for (unsigned short int x = 0; x < (this->xres>>3); x++)
	   {
	    this->backBuffer[y][x] = 0; //Lo borro todo no dejo color (chapuza rapida)
	   }
	  }	 
	 }
	 //else
	 //{
	 // for (int y = 0; y < this->yres; y++)
	 // {
	 //  //for (int x = 0; x < this->xres / 2; x++)
	 //  for (int x = 0; x < (this->xres>>1); x++)
	 //  {
	 //   this->backBuffer[y][x] = color | (color << 4);
	 //  }
	 // }
	 //}
	}

	virtual Color** allocateFrameBuffer()
	{		
     int yresmod= 0;
	 int auxYres= 0;
	 int auxXres= 0;
	 Serial.printf("BitluniVGA allocateFrameBuffer GraphicsR1G1B1A1.h\r\n");
	 Serial.printf(" gb_vga_text:%d gb_vga_1bpp:%d\r\n",gb_vga_text,gb_vga_1bpp);
	 Serial.printf(" xres:%d yres:%d\r\n",xres,yres);
	 Serial.printf(" gb_vga_videomode:%d\r\n",gb_vga_videomode);
     if (gb_vga_text==1)
	 {	 
      yresmod= (yres&0x07);
	  auxYres= (yresmod>0)? (yres>>3)+1 : (yres>>3);
	  auxXres= ((gb_vga_videomode == vgaMode_TextMode3bpp)||(gb_vga_videomode == vgaMode_TextMode3bppRetro))? (xres>>2) : (xres>>3); //2 bytes para color y atributo o 1 byte sin color
	  
	  Serial.printf(" text xres:%d yres:%d yresmod:%d auxYres:%d\r\n",(xres>>3),(yres>>3),yresmod, auxYres);
	  return Graphics<Color>::allocateFrameBuffer(auxXres, auxYres, (Color)0);
	 }

	 if (gb_vga_1bpp==1)
	 {//1 bpp 8 pixels por byte
	  Serial.printf(" 1bpp xres:%d yres:%d\r\n",(xres>>3),yres);
      return Graphics<Color>::allocateFrameBuffer((xres>>3), yres, (Color)0);
	 }
	 else
	 {//3bpp (4 bits) 2 pixels por byte
	  Serial.printf(" 4bpp xres:%d yres:%d\r\n",(xres/2),yres);
	  return Graphics<Color>::allocateFrameBuffer(xres / 2, yres, (Color)0);
	 }
	}
};
//BitluniGraphicsR1G1B1A1.h END








class VGA3BitI : public VGA, public GraphicsR1G1B1A1
{  
  public:    
	VGA3BitI()	//8 bit based modes only work with I2S1
		: VGA(1)
	{
		interruptStaticChild = &VGA3BitI::interrupt;
	}

//JJ	bool init(const Mode &mode, const int RPin, const int GPin, const int BPin, const int hsyncPin, const int vsyncPin, const int clockPin = -1)
//JJ	{
//JJ		int pinMap[8] = {
//JJ			RPin,
//JJ			GPin,
//JJ			BPin,
//JJ			-1, -1, -1,
//JJ			hsyncPin, vsyncPin
//JJ		};
//JJ		return VGA::init(mode, pinMap, 8, clockPin);
//JJ	}

//JJ	bool init(const Mode &mode, const PinConfig &pinConfig)
//JJ	{
//JJ		int pins[8];
//JJ		pinConfig.fill3Bit(pins);
//JJ		return VGA::init(mode, pins, 8, pinConfig.clock);
//JJ	}

	//bool init(const Mode &mode, const short int *pinConfig)
	bool init(const Mode &mode, const unsigned char *pinConfig)
	{
		return VGA::init(mode, pinConfig, 8, -1);
	}

	virtual void initSyncBits()
	{
		hsyncBitI = mode.hSyncPolarity ? 0x40 : 0;
		vsyncBitI = mode.vSyncPolarity ? 0x80 : 0;
		hsyncBit = hsyncBitI ^ 0x40;
		vsyncBit = vsyncBitI ^ 0x80;

		PrecalculaInterrupt();
	}

	virtual long syncBits(bool hSync, bool vSync)
	{
		return ((hSync ? hsyncBit : hsyncBitI) | (vSync ? vsyncBit : vsyncBitI)) * 0x1010101;
	}

	virtual int bytesPerSample() const
	{
		return 1;
	}

	//JJ virtual float pixelAspect() const
	//JJ {
	//JJ 	return 1;
	//JJ }

	virtual void propagateResolution(const int xres, const int yres)
	{
		setResolution(xres, yres);
	}

	virtual void show(bool vSync = false)
	{
		if (!frameBufferCount)
			return;
		if (vSync)
		{
			vSyncPassed = false;
			while (!vSyncPassed)
				delay(0);
		}
		Graphics::show(vSync);
	}	

  protected:
	bool useInterrupt()
	{ 
		return true; 
	};

	static void interrupt(void *arg);

	//JJ static void interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits, void *arg);
	//static void interruptPixelLine(unsigned short int y, unsigned long *pixels, unsigned long syncBits, void *arg);
	void PrecalculaInterrupt(void);
};
//BitluniVGA3BitI.h END






#endif


