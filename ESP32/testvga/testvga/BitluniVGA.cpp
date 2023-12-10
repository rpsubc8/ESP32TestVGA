//	Author: bitluni 2019
//	License: 
//	Creative Commons Attribution ShareAlike 4.0
//	https://creativecommons.org/licenses/by-sa/4.0/
//	
//	For further details check out: 
//		https://youtube.com/bitlunislab
//		https://github.com/bitluni
//		http://bitluni.net

#include "BitluniVGA.h"

//BitluniI2S.cpp include BEGIN
#include <soc/rtc.h>
#include <driver/rtc_io.h>
//BitluniI2S.cpp include END

#include <Arduino.h>
#include "hardware.h"

unsigned char gb_colorBW[2]; //={0x00,0x07};
unsigned char gb_sdl_font_6x8_ram[2048]; //256*8

//hfront hsync hback pixels vfront vsync vback lines divy pixelclock hpolaritynegative vpolaritynegative
//JJ const Mode VGA::MODE320x480(8, 48, 24, 320, 11, 2, 31, 480, 1, 12587500, 1, 1);
//JJ const Mode VGA::MODE320x240(8, 48, 24, 320, 11, 2, 31, 480, 2, 12587500, 1, 1);
//JJ const Mode VGA::MODE320x400(8, 48, 24, 320, 12, 2, 35, 400, 1, 12587500, 1, 0);
//JJ const Mode VGA::MODE320x200(8, 48, 24, 320, 12, 2, 35, 400, 2, 12587500, 1, 0);
//JJ const Mode VGA::MODE360x400(8, 54, 28, 360, 11, 2, 32, 400, 1, 14161000, 1, 0);
//JJ const Mode VGA::MODE360x200(8, 54, 28, 360, 11, 2, 32, 400, 2, 14161000, 1, 0);
//JJ const Mode VGA::MODE360x350(8, 54, 28, 360, 11, 2, 32, 350, 1, 14161000, 1, 1);
//JJ const Mode VGA::MODE360x175 (8, 54, 28, 360, 11, 2, 32, 350, 2, 14161000, 1, 1);

//JJ const Mode VGA::MODE320x350 (8, 48, 24, 320, 37, 2, 60, 350, 1, 12587500, 0, 1);
//JJ const Mode VGA::MODE320x175(8, 48, 24, 320, 37, 2, 60, 350, 2, 12587500, 0, 1);


//JJ const Mode VGA::MODE400x300(12, 36, 64, 400, 1, 2, 22, 600, 2, 18000000, 0, 0);
Mode VGA::MODEcurrent(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); //Ahorro RAM un uncio modo dinamico

//JJ const Mode VGA::MODE400x150(12, 36, 64, 400, 1, 2, 22, 600, 4, 18000000, 0, 0);
//JJ const Mode VGA::MODE400x100(12, 36, 64, 400, 1, 2, 22, 600, 6, 18000000, 0, 0);
//JJ const Mode VGA::MODE200x150(6, 18, 32, 200, 1, 2, 22, 600, 4, 9000000, 0, 0);
//JJ //const Mode VGA::MODE200x150(10, 32, 22, 200, 1, 4, 23, 600, 4, 10000000, 0, 0);	//60Hz version

//500 pixels horizontal it's based on 640x480
//JJ const Mode VGA::MODE500x480(12, 76, 38, 500, 11, 2, 31, 480, 1, 19667968, 1, 1);
//JJ const Mode VGA::MODE500x240(12, 76, 38, 500, 11, 2, 31, 480, 2, 19667968, 1, 1);

//base modes for custom mode calculations
//JJ const Mode VGA::MODE1280x1024(48, 112, 248, 1280, 1, 3, 38, 1024, 1, 108000000, 0, 0);
//JJ const Mode VGA::MODE1280x960(80, 136, 216, 1280, 1, 3, 30, 960, 1, 101200000, 1, 0);
//JJ const Mode VGA::MODE1280x800(64, 136, 200, 1280, 1, 3, 24, 800, 1, 83640000, 1, 0);
//fabgl SVGA_1280x600_60Hz "\"1280x600@60Hz\" 61.5 1280 1336 1464 1648 600 601 604 622 -HSync -VSync"
//fabgl SVGA_1280x720_60Hz "\"1280x720@60Hz\" 74.48 1280 1468 1604 1664 720 721 724 746 +hsync +vsync"
//fabgl SVGA_1280x720_60HzAlt1 "\"1280x720@60HzAlt1\" 73.78 1280 1312 1592 1624 720 735 742 757"
//fabgl SVGA_1280x768_50Hz "\"1280x768@50Hz\" 64.050004 1280 1312 1552 1584 768 784 791 807 -HSync -VSync"


//JJ const Mode VGA::MODE1024x768(24, 136, 160, 1024, 3, 6, 29, 768, 1, 65000000, 1, 1); //Original Falla no se ve Es un XGA 1024x768@60 Hz (pixel clock 65.0 MHz)
//fabgl SVGA_1024x768_60Hz "\"1024x768@60Hz\" 65 1024 1048 1184 1344 768 771 777 806 -HSync -VSync"
//fabgl SVGA_1024x768_70Hz "\"1024x768@70Hz\" 75 1024 1048 1184 1328 768 771 777 806 -HSync -VSync"
//fabgl SVGA_1024x768_75Hz "\"1024x768@75Hz\" 78.80 1024 1040 1136 1312 768 769 772 800 +HSync +VSync"
//const Mode VGA::MODE1024x768(24, 136, 144, 1024, 3, 6, 29, 768, 1, 75000000, 1, 1); //Falla no se ve VESA Signal 1024 x 768 @ 70 Hz timing
//const Mode VGA::MODE1024x768(16, 96, 176, 1024, 1, 3, 28, 768, 1, 78800000, 0, 0); //Falla VESA Signal 1024 x 768 @ 75 Hz timing


//fabgl VGA_640x200_60HzD "\"640x200@60HzD\" 25.175 640 656 752 800 200 226 227 262 -HSync -VSync doublescan"
//fabgl VGA_640x200_70Hz "\"640x200@70Hz\" 25.175 640 656 752 800 200 206 207 224 -HSync -VSync DoubleScan"
//fabgl VGA_640x200_70HzRetro "\"640x200@70Hz\" 25.175 640 663 759 808 200 208 211 226 -HSync -VSync DoubleScan MultiScanBlank"
//fabgl VGA_640x240_60Hz "\"640x240@60Hz\" 25.175 640 656 752 800 240 245 247 262 -HSync -VSync DoubleScan"

//fabgl VGA_640x240_60Hz "\"640x240@60Hz\" 25.175 640 656 752 800 240 245 247 262 -HSync -VSync DoubleScan"
//fabgl NTSC_640x240_60hz "\"NTSC 640x240 (60Hz)\" 12.312 640 662 719 784 240 244 247 262 -hsync -vsync"



//JJ const Mode VGA::MODE800x600(24, 72, 128, 800, 1, 2, 22, 600, 1, 36000000, 0, 0);
//JJ const Mode VGA::MODE720x400(16, 108, 56, 720, 11, 2, 32, 400, 1, 28322000, 1, 0);
//JJ const Mode VGA::MODE720x350(16, 108, 56, 720, 11, 2, 32, 350, 1, 28322000, 1, 1);
//JJ const Mode VGA::MODE640x480(16, 96, 48, 640, 11, 2, 31, 480, 1, 25175000, 1, 1);
//fabgl VGA_640x480_60Hz "\"640x480@60Hz\" 25.175 640 656 752 800 480 490 492 525 -HSync -VSync"
//fabgl VGA_640x480_60HzAlt1 "\"640x480@60HzAlt1\" 27.5 640 672 768 864 480 482 488 530 -HSync -VSync"
//fabgl VGA_640x480_60HzD "\"640x480@60HzD\" 54.00 640 688 744 900 480 480 482 500 +HSync +VSync DoubleScan"
//fabgl VGA_640x480_73Hz "\"640x480@73Hz\" 31.5 640 664 704 832 480 489 491 520 -HSync -VSync"
//fabgl VESA_640x480_75Hz "\"640x480@75Hz\" 31.5 640 656 720 840 480 481 484 500 -HSync -VSync"


//JJ const Mode VGA::MODE640x400(16, 96, 48, 640, 12, 2, 35, 400, 1, 25175000, 1, 0);
//fabgl VGA_640x400_70Hz "\"640x400@70Hz\" 25.175 640 656 752 800 400 412 414 449 -HSync -VSync"
//fabgl VGA_640x400_60Hz "\"640x400@60Hz-mod\" 25.175 640 656 752 800 400 452 454 525 -HSync -VSync"

//JJ const Mode VGA::MODE640x350(16, 96, 48, 640, 37, 2, 60, 350, 1, 25175000, 0, 1);

//JJ const PinConfig VGA::VGAv01(2, 4, 12, 13, 14,  15, 16, 17, 18, 19,  21, 22, 23, 27,  32, 33,  -1);
//JJ const PinConfig VGA::VGABlackEdition(2, 4, 12, 13, 14,  15, 16, 17, 18, 19,  21, 22, 23, 27,  32, 33,  -1);
//JJ const PinConfig VGA::VGAWhiteEdition(5, 14, 13, 15, 2,  19, 18, 17, 4, 16,  27, 22, 12, 21,  32, 33, -1);
//JJ const PinConfig VGA::PicoVGA(-1, -1, -1, 18, 5,  -1, -1, -1, 14, 4,  -1, -1, 27, 15,  32, 33,  -1);




//BitluniI2S.cpp BEGIN
i2s_dev_t *i2sDevices[] = {&I2S0, &I2S1};

I2S::I2S(const int i2sIndex)
{
	const periph_module_t deviceModule[] = {PERIPH_I2S0_MODULE, PERIPH_I2S1_MODULE};
	this->i2sIndex = i2sIndex;
	//enable I2S peripheral
	periph_module_enable(deviceModule[i2sIndex]);
	interruptHandle = 0;
	dmaBufferDescriptorCount = 0;
	dmaBufferDescriptorActive = 0;
	dmaBufferDescriptors = 0;
	stopSignal = false;
}

void IRAM_ATTR I2S::interruptStatic(void *arg)
{
	volatile i2s_dev_t &i2s = *i2sDevices[((I2S *)arg)->i2sIndex];
	//i2s object not safely accesed in DRAM or IRAM
	//i2s.int_clr.val = i2s.int_raw.val;
	//using REG_WRITE to clear the interrupt instead
	//note: there are still other alternatives, see i2s driver .c file
	//inside the i2s_intr_handler_default() function
	REG_WRITE(I2S_INT_CLR_REG(((I2S *)arg)->i2sIndex), (REG_READ(I2S_INT_RAW_REG(((I2S *)arg)->i2sIndex)) & 0xffffffc0) | 0x3f);
	//the call to the overloaded (or any) non-static member function definitely breaks the IRAM rule
	// causing an exception when concurrently accessing the flash (or flash-filesystem) or wifi
	//the reason is unknown but probably related with the compiler instantiation mechanism
	//(note: defining the code of the [member] interrupt function outside the class declaration,
	// and with IRAM flag does not avoid the crash)
	//((I2S *)arg)->interrupt();
	
	if(((I2S *)arg)->interruptStaticChild)
		((I2S *)arg)->interruptStaticChild(arg);
}

void I2S::reset()
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	const unsigned long lc_conf_reset_flags = I2S_IN_RST_M | I2S_OUT_RST_M | I2S_AHBM_RST_M | I2S_AHBM_FIFO_RST_M;
	i2s.lc_conf.val |= lc_conf_reset_flags;
	i2s.lc_conf.val &= ~lc_conf_reset_flags;

	const uint32_t conf_reset_flags = I2S_RX_RESET_M | I2S_RX_FIFO_RESET_M | I2S_TX_RESET_M | I2S_TX_FIFO_RESET_M;
	i2s.conf.val |= conf_reset_flags;
	i2s.conf.val &= ~conf_reset_flags;
	while (i2s.state.rx_fifo_reset_back)
		;
}

void I2S::i2sStop()
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	esp_intr_disable(interruptHandle);
	reset();
	i2s.conf.rx_start = 0;
	i2s.conf.tx_start = 0;
}

void I2S::startTX()
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	//JJ DEBUG_PRINTLN("I2S TX");
	Serial.print("I2S TX\r\n");
	esp_intr_disable(interruptHandle);
	reset();
    i2s.lc_conf.val    = I2S_OUT_DATA_BURST_EN | I2S_OUTDSCR_BURST_EN;
	dmaBufferDescriptorActive = 0;
	i2s.out_link.addr = (uint32_t)firstDescriptorAddress();
	i2s.out_link.start = 1;
	i2s.int_clr.val = i2s.int_raw.val;
	i2s.int_ena.val = 0;
	if(useInterrupt())
	{
		i2s.int_ena.out_eof = 1;
		//enable interrupt
		esp_intr_enable(interruptHandle);
	}
	//start transmission
	i2s.conf.tx_start = 1;
}

void I2S::startRX()
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	//JJ DEBUG_PRINTLN("I2S RX");
	Serial.print("I2S RX\r\n");
	esp_intr_disable(interruptHandle);
	reset();
	dmaBufferDescriptorActive = 0;
	i2s.rx_eof_num = dmaBufferDescriptors[0].sampleCount();	//TODO: replace with cont of sample to be recorded
	i2s.in_link.addr = (uint32_t)firstDescriptorAddress();
	i2s.in_link.start = 1;
	i2s.int_clr.val = i2s.int_raw.val;
	i2s.int_ena.val = 0;
	i2s.int_ena.in_done = 1;
	esp_intr_enable(interruptHandle);
	i2s.conf.rx_start = 1;
}

void I2S::resetDMA()
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	i2s.lc_conf.in_rst = 1;
	i2s.lc_conf.in_rst = 0;
	i2s.lc_conf.out_rst = 1;
	i2s.lc_conf.out_rst = 0;
}

void I2S::resetFIFO()
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	i2s.conf.rx_fifo_reset = 1;
	i2s.conf.rx_fifo_reset = 0;
	i2s.conf.tx_fifo_reset = 1;
	i2s.conf.tx_fifo_reset = 0;
}

DMABufferDescriptor *I2S::firstDescriptorAddress() const
{
	return &dmaBufferDescriptors[0];
}

bool I2S::useInterrupt()
{ 
	return false; 
};

void I2S::getClockSetting(long *sampleRate, int *n, int *a, int *b, int *div)
{
	if(sampleRate)
		*sampleRate = 2000000;
	if(n)
		*n = 2;
	if(a)
		*a = 1;
	if(b)
		*b = 0;
	if(div)
		*div = 1;
}


//bool I2S::initParallelInputMode(const int *pinMap, long sampleRate, const int bitCount, int wordSelect, int baseClock)
//{
//	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
//	//route peripherals
//	const int deviceBaseIndex[] = {I2S0I_DATA_IN0_IDX, I2S1I_DATA_IN0_IDX};
//	const int deviceClockIndex[] = {I2S0I_BCK_IN_IDX, I2S1I_BCK_IN_IDX};
//	const int deviceWordSelectIndex[] = {I2S0I_WS_IN_IDX, I2S1I_WS_IN_IDX};
//	const periph_module_t deviceModule[] = {PERIPH_I2S0_MODULE, PERIPH_I2S1_MODULE};
//	//works only since indices of the pads are sequential
//	for (int i = 0; i < bitCount; i++)
//	{
//		//if (pinMap[i] > -1) //Uso unsigned char
//		if (pinMap[i] != 255)
//		{
//			PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[pinMap[i]], PIN_FUNC_GPIO);
//			gpio_set_direction((gpio_num_t)pinMap[i], (gpio_mode_t)GPIO_MODE_DEF_INPUT);
//			gpio_matrix_in(pinMap[i], deviceBaseIndex[i2sIndex] + i, false);
//		}
//	}
//	if (baseClock > -1)
//		gpio_matrix_in(baseClock, deviceClockIndex[i2sIndex], false);
//	if (wordSelect > -1)
//		gpio_matrix_in(wordSelect, deviceWordSelectIndex[i2sIndex], false);
//
//	//enable I2S peripheral
//	periph_module_enable(deviceModule[i2sIndex]);
//
//	//reset i2s
//	i2s.conf.rx_reset = 1;
//	i2s.conf.rx_reset = 0;
//	i2s.conf.tx_reset = 1;
//	i2s.conf.tx_reset = 0;
//
//	resetFIFO();
//	resetDMA();
//
//	//parallel mode
//	i2s.conf2.val = 0;
//	i2s.conf2.lcd_en = 1;
//	//from technical datasheet figure 64
//	//i2s.conf2.lcd_tx_sdx2_en = 1;
//	//i2s.conf2.lcd_tx_wrx2_en = 1;
//
//	i2s.sample_rate_conf.val = 0;
//	i2s.sample_rate_conf.rx_bits_mod = 16;
//
//	//maximum rate
//	i2s.clkm_conf.val = 0;
//	i2s.clkm_conf.clka_en = 0;
//	i2s.clkm_conf.clkm_div_num = 6; //3//80000000L / sampleRate;
//	i2s.clkm_conf.clkm_div_a = 6;   // 0;
//	i2s.clkm_conf.clkm_div_b = 1;   // 0;
//	i2s.sample_rate_conf.rx_bck_div_num = 2;
//
//	i2s.fifo_conf.val = 0;
//	i2s.fifo_conf.rx_fifo_mod_force_en = 1;
//	i2s.fifo_conf.rx_fifo_mod = 1; //byte packing 0A0B_0B0C = 0, 0A0B_0C0D = 1, 0A00_0B00 = 3,
//	i2s.fifo_conf.rx_data_num = 32;
//	i2s.fifo_conf.dscr_en = 1; //fifo will use dma
//
//	i2s.conf1.val = 0;
//	i2s.conf1.tx_stop_en = 1;
//	i2s.conf1.tx_pcm_bypass = 1;
//
//	i2s.conf_chan.val = 0;
//	i2s.conf_chan.rx_chan_mod = 0;
//
//	//high or low (stereo word order)
//	i2s.conf.rx_right_first = 1;
//
//	i2s.timing.val = 0;
//
//	//clear serial mode flags
//	i2s.conf.rx_msb_right = 0;
//	i2s.conf.rx_msb_shift = 0;
//	i2s.conf.rx_mono = 0;
//	i2s.conf.rx_short_sync = 0;
//
//	//allocate disabled i2s interrupt
//	const int interruptSource[] = {ETS_I2S0_INTR_SOURCE, ETS_I2S1_INTR_SOURCE};
//	if(useInterrupt())
//		esp_intr_alloc(interruptSource[i2sIndex], ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM, &interruptStatic, this, &interruptHandle);
//	return true;
//}

//JJ bool I2S::initParallelOutputMode(const int *pinMap, long sampleRate, const int bitCount, int wordSelect, int baseClock)
//bool I2S::initParallelOutputMode(const short int *pinMap, long sampleRate, const unsigned char bitCount, int wordSelect, short int baseClock)
bool I2S::initParallelOutputMode(const unsigned char *pinMap, long sampleRate, const unsigned char bitCount, int wordSelect, short int baseClock)
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	//route peripherals
	//in parallel mode only upper 16 bits are interesting in this case
	const int deviceBaseIndex[] = {I2S0O_DATA_OUT0_IDX, I2S1O_DATA_OUT0_IDX};
	const int deviceClockIndex[] = {I2S0O_BCK_OUT_IDX, I2S1O_BCK_OUT_IDX};
	const int deviceWordSelectIndex[] = {I2S0O_WS_OUT_IDX, I2S1O_WS_OUT_IDX};
	const periph_module_t deviceModule[] = {PERIPH_I2S0_MODULE, PERIPH_I2S1_MODULE};
	//works only since indices of the pads are sequential
	for (int i = 0; i < bitCount; i++)
	{
		//JJ if (pinMap[i] > -1) //uso unsigned char 255 pin nulo
		if (pinMap[i] != 255)
		{
			PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[pinMap[i]], PIN_FUNC_GPIO);
			gpio_set_direction((gpio_num_t)pinMap[i], (gpio_mode_t)GPIO_MODE_DEF_OUTPUT);
			//rtc_gpio_set_drive_capability((gpio_num_t)pinMap[i], (gpio_drive_cap_t)GPIO_DRIVE_CAP_3 );
			if(i2sIndex == 1)
			{
				if(bitCount == 16)
					gpio_matrix_out(pinMap[i], deviceBaseIndex[i2sIndex] + i + 8, false, false);
				else
					gpio_matrix_out(pinMap[i], deviceBaseIndex[i2sIndex] + i, false, false);
			}
			else
			{
				//there is something odd going on here in the two different I2S
				//the configuration seems to differ. Use i2s1 for high frequencies.
				gpio_matrix_out(pinMap[i], deviceBaseIndex[i2sIndex] + i + 24 - bitCount, false, false);
			}
		}
	}
	if (baseClock > -1)
		gpio_matrix_out(baseClock, deviceClockIndex[i2sIndex], false, false);
	if (wordSelect > -1)
		gpio_matrix_out(wordSelect, deviceWordSelectIndex[i2sIndex], false, false);

		//enable I2S peripheral
	periph_module_enable(deviceModule[i2sIndex]);

	//reset i2s
	i2s.conf.tx_reset = 1;
	i2s.conf.tx_reset = 0;
	i2s.conf.rx_reset = 1;
	i2s.conf.rx_reset = 0;

	resetFIFO();
	resetDMA();

	//parallel mode
	i2s.conf2.val = 0;
	i2s.conf2.lcd_en = 1;
	//from technical datasheet figure 64
	i2s.conf2.lcd_tx_wrx2_en = 1;
	i2s.conf2.lcd_tx_sdx2_en = 0;

	i2s.sample_rate_conf.val = 0;
	i2s.sample_rate_conf.tx_bits_mod = bitCount;
	//clock setup
	int clockN = 2, clockA = 1, clockB = 0, clockDiv = 1;
	if(sampleRate == 0)
		getClockSetting(&sampleRate, &clockN, &clockA, &clockB, &clockDiv);
	if(sampleRate > 0)
	{
		//xtal is 40M
		//chip revision 0
		//fxtal * (sdm2 + 4) / (2 * (odir + 2))
		//chip revision 1
		//fxtal * (sdm2 + (sdm1 / 256) + (sdm0 / 65536) + 4) / (2 * (odir + 2))
		//fxtal * (sdm2 + (sdm1 / 256) + (sdm0 / 65536) + 4) needs to be btween 350M and 500M
		//rtc_clk_apll_enable(enable, sdm0, sdm1, sdm2, odir);
		//                           0-255 0-255  0-63  0-31
		//sdm seems to be simply a fixpoint number with 16bits fractional part
		//freq = 40000000L * (4 + sdm) / (2 * (odir + 2))
		//sdm = freq / (20000000L / (odir + 2)) - 4;
		long freq = sampleRate * 2 * (bitCount / 8);
		int sdm, sdmn;
		int odir = -1;
		do
		{	
			odir++;
			sdm = long((double(freq) / (20000000. / (odir + 2))) * 0x10000) - 0x40000;
			sdmn = long((double(freq) / (20000000. / (odir + 2 + 1))) * 0x10000) - 0x40000;
		}while(sdm < 0x8c0ecL && odir < 31 && sdmn < 0xA1fff); //0xA7fffL doesn't work on all mcus 
		//DEBUG_PRINTLN(sdm & 255);
		//DEBUG_PRINTLN((sdm >> 8) & 255);
		//DEBUG_PRINTLN(sdm >> 16);
		//DEBUG_PRINTLN(odir);
		//sdm = 0xA1fff;
		//odir = 0;
		if(sdm > 0xA1fff) sdm = 0xA1fff;
		rtc_clk_apll_enable(true, sdm & 255, (sdm >> 8) & 255, sdm >> 16, odir);
	}

	i2s.clkm_conf.val = 0;
	i2s.clkm_conf.clka_en = sampleRate > 0 ? 1 : 0;
	i2s.clkm_conf.clkm_div_num = clockN;
	i2s.clkm_conf.clkm_div_a = clockA;
	i2s.clkm_conf.clkm_div_b = clockB;
	i2s.sample_rate_conf.tx_bck_div_num = clockDiv;

	i2s.fifo_conf.val = 0;
	i2s.fifo_conf.tx_fifo_mod_force_en = 1;
	i2s.fifo_conf.tx_fifo_mod = 1;  //byte packing 0A0B_0B0C = 0, 0A0B_0C0D = 1, 0A00_0B00 = 3,
	i2s.fifo_conf.tx_data_num = 32; //fifo length
	i2s.fifo_conf.dscr_en = 1;		//fifo will use dma

	i2s.conf1.val = 0;
	i2s.conf1.tx_stop_en = 0;
	i2s.conf1.tx_pcm_bypass = 1;

	i2s.conf_chan.val = 0;
	i2s.conf_chan.tx_chan_mod = 1;

	//high or low (stereo word order)
	i2s.conf.tx_right_first = 1;

	i2s.timing.val = 0;

	//clear serial mode flags
	i2s.conf.tx_msb_right = 0;
	i2s.conf.tx_msb_shift = 0;
	i2s.conf.tx_mono = 0;
	i2s.conf.tx_short_sync = 0;

	//allocate disabled i2s interrupt
	const int interruptSource[] = {ETS_I2S0_INTR_SOURCE, ETS_I2S1_INTR_SOURCE};
	if(useInterrupt())
		esp_intr_alloc(interruptSource[i2sIndex], ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM, &interruptStatic, this, &interruptHandle);
	return true;
}

void I2S::SetVideoInterrupt(unsigned char auxState)
{
 if (auxState == 1)
 {
  if (interruptHandle!=0)
  {
   esp_intr_enable(interruptHandle);   
  }
 }
 else
 {
  if (interruptHandle!=0)
  {
   esp_intr_disable(interruptHandle);   
  }
 } 
}

void I2S::FreeInterrupt()
{//Libero handle interrupcion para coexistir Ricardo Massaro y Bitluni
 Serial.printf("BitluniVGA FreeInterrupt Begin\r\n");
 if(useInterrupt())
 {//Se fijo que es interrupcion, lo dejo para agregar mas al futuro
  if (interruptHandle!=0)
  {
   esp_intr_free(interruptHandle);   
   interruptHandle= 0; //Deberia chequear return ESP_OK
  }
 }
 Serial.printf("BitluniVGA FreeInterrupt End\r\n");
}

void I2S::setAPLLClock(long sampleRate, int bitCount)
{
	//xtal is 40M
	//chip revision 0
	//fxtal * (sdm2 + 4) / (2 * (odir + 2))
	//chip revision 1
	//fxtal * (sdm2 + (sdm1 / 256) + (sdm0 / 65536) + 4) / (2 * (odir + 2))
	//fxtal * (sdm2 + (sdm1 / 256) + (sdm0 / 65536) + 4) needs to be btween 350M and 500M
	//rtc_clk_apll_enable(enable, sdm0, sdm1, sdm2, odir);
	//                           0-255 0-255  0-63  0-31
	//sdm seems to be simply a fixpoint number with 16bits fractional part
	//freq = 40000000L * (4 + sdm) / (2 * (odir + 2))
	//sdm = freq / (20000000L / (odir + 2)) - 4;
	long freq = sampleRate * 2 * (bitCount / 8);
	int sdm, sdmn;
	int odir = -1;
	do
	{	
		odir++;
		sdm = long((double(freq) / (20000000. / (odir + 2))) * 0x10000) - 0x40000;
		sdmn = long((double(freq) / (20000000. / (odir + 2 + 1))) * 0x10000) - 0x40000;
	}while(sdm < 0x8c0ecL && odir < 31 && sdmn < 0xA1fff); //0xA7fffL doesn't work on all mcus 
	//DEBUG_PRINTLN(sdm & 255);
	//DEBUG_PRINTLN((sdm >> 8) & 255);
	//DEBUG_PRINTLN(sdm >> 16);
	//DEBUG_PRINTLN(odir);
	//sdm = 0xA1fff;
	//odir = 0;
	if(sdm > 0xA1fff) sdm = 0xA1fff;
	rtc_clk_apll_enable(true, sdm & 255, (sdm >> 8) & 255, sdm >> 16, odir);
}

void I2S::setClock(long sampleRate, int bitCount, bool useAPLL)
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	int factor = 1;
	if(bitCount > 8)
		factor = 2;
	else if(bitCount > 16)
		factor = 4;
	i2s.clkm_conf.val = 0;
	i2s.sample_rate_conf.val = 0;
	i2s.sample_rate_conf.tx_bits_mod = bitCount;

	if(useAPLL)
	{
		setAPLLClock(sampleRate, bitCount);
		i2s.clkm_conf.clka_en = 1;
		i2s.clkm_conf.clkm_div_num = 2; //clockN;
		i2s.clkm_conf.clkm_div_a = 1;   //clockA;
		i2s.clkm_conf.clkm_div_b = 0;   //clockB;
		i2s.sample_rate_conf.tx_bck_div_num = 1;
	}
	else
	{
		i2s.clkm_conf.clkm_div_num = 40000000L / (sampleRate * factor); //clockN;
		i2s.clkm_conf.clkm_div_a = 1;   //clockA;
		i2s.clkm_conf.clkm_div_b = 0;   //clockB;
		i2s.sample_rate_conf.tx_bck_div_num = 1;
	}
}

bool I2S::initSerialOutputMode(int dataPin, const int bitCount, int wordSelect, int baseClock)
{
	volatile i2s_dev_t &i2s = *i2sDevices[i2sIndex];
	//route peripherals
	//in parallel mode only upper 16 bits are interesting in this case
	const int deviceBaseIndex[] = {I2S0O_DATA_OUT0_IDX, I2S1O_DATA_OUT0_IDX};
	const int deviceClockIndex[] = {I2S0O_BCK_OUT_IDX, I2S1O_BCK_OUT_IDX};
	const int deviceWordSelectIndex[] = {I2S0O_WS_OUT_IDX, I2S1O_WS_OUT_IDX};
	const periph_module_t deviceModule[] = {PERIPH_I2S0_MODULE, PERIPH_I2S1_MODULE};
	
	//works only since indices of the pads are sequential
	//rtc_gpio_set_drive_capability((gpio_num_t)dataPin, (gpio_drive_cap_t)GPIO_DRIVE_CAP_3 );
	
	//serial output on 23, input on 15
	gpio_matrix_out(dataPin, deviceBaseIndex[i2sIndex] + 23, false, false);

	if (baseClock > -1)
		gpio_matrix_out(baseClock, deviceClockIndex[i2sIndex], false, false);
	if (wordSelect > -1)
		gpio_matrix_out(wordSelect, deviceWordSelectIndex[i2sIndex], false, false);

	//reset i2s
	i2s.conf.tx_reset = 1;
	i2s.conf.tx_reset = 0;
	i2s.conf.rx_reset = 1;
	i2s.conf.rx_reset = 0;

	resetFIFO();
	resetDMA();

	//parallel mode
	i2s.conf2.val = 0;
	i2s.conf2.lcd_en = 0;


	i2s.fifo_conf.val = 0;
	i2s.fifo_conf.tx_fifo_mod_force_en = 1;
	i2s.fifo_conf.tx_fifo_mod = 2;  //byte packing 
	i2s.fifo_conf.tx_data_num = 32; //fifo length
	i2s.fifo_conf.dscr_en = 1;		//fifo will use dma

	i2s.conf_chan.val = 0;
	i2s.conf_chan.tx_chan_mod = 0;

	i2s.conf1.val = 0;
	i2s.conf1.tx_stop_en = 0;
	i2s.conf1.tx_pcm_bypass = 1;

	i2s.timing.val = 0;

	//high or low (stereo word order)
	i2s.conf.tx_right_first = 1;
	//clear serial mode flags
	i2s.conf.tx_msb_right = 1;
	i2s.conf.tx_msb_shift = 0;
	i2s.conf.tx_mono = 0;
	i2s.conf.tx_short_sync = 0;


	//allocate disabled i2s interrupt
	const int interruptSource[] = {ETS_I2S0_INTR_SOURCE, ETS_I2S1_INTR_SOURCE};
	if(useInterrupt())
		esp_intr_alloc(interruptSource[i2sIndex], ESP_INTR_FLAG_INTRDISABLED | ESP_INTR_FLAG_LEVEL3 | ESP_INTR_FLAG_IRAM, &interruptStatic, this, &interruptHandle);
	return true;
}

/// simple ringbuffer of blocks of size bytes each
void I2S::allocateDMABuffers(int count, int bytes)
{
	Serial.printf("allocateDMABuffers count:%d bytes:%d size:%d size:%d\r\n",count,bytes,(count*sizeof(DMABufferDescriptor)), (count*bytes));

	dmaBufferDescriptorCount = count;
	dmaBufferDescriptors = DMABufferDescriptor::allocateDescriptors(count);
	for (int i = 0; i < dmaBufferDescriptorCount; i++)
	{
		dmaBufferDescriptors[i].setBuffer(DMABufferDescriptor::allocateBuffer(bytes, true), bytes);
		if (i)
			dmaBufferDescriptors[i - 1].next(dmaBufferDescriptors[i]);
	}
	dmaBufferDescriptors[dmaBufferDescriptorCount - 1].next(dmaBufferDescriptors[0]);
}

void I2S::deleteDMABuffers()
{
	if (!dmaBufferDescriptors)
		return;
	for (int i = 0; i < dmaBufferDescriptorCount; i++)
		free(dmaBufferDescriptors[i].buffer());
	free(dmaBufferDescriptors);
	dmaBufferDescriptors = 0;
	dmaBufferDescriptorCount = 0;
}

void I2S::stop()
{
	stopSignal = true;
	while (stopSignal)
		;
}
//BitluniI2S.cpp END




VGA::VGA(const int i2sIndex)
	: I2S(i2sIndex)
{
	lineBufferCount = 8;
	dmaBufferDescriptors = 0;
}

//JJ bool VGA::init(const Mode &mode, const int *pinMap, const int bitCount, const int clockPin)
//bool VGA::init(const Mode &mode, const short int *pinMap, const unsigned char bitCount, const short int clockPin)
bool VGA::init(const Mode &mode, const unsigned char *pinMap, const unsigned char bitCount, const short int clockPin)
{
    Serial.printf("BitluniVGA init\r\n");

	this->mode = mode;
	int xres = mode.hRes;
	int yres = mode.vRes / mode.vDiv;
	initSyncBits();	
	propagateResolution(xres, yres);
	//JJ this->vsyncPin = vsyncPin;
	//JJ this->hsyncPin = hsyncPin;
	//this->vsyncPin = PIN_VSYNC; //No lo necesito
	//this->hsyncPin = PIN_HSYNC; //No lo necesito
	totalLines = mode.linesPerField();

	Serial.printf("BitluniVGA Antes allocateLineBuffers\r\n");
	allocateLineBuffers();
	Serial.printf("BitluniVGA Despues allocateLineBuffers\r\n");

	currentLine = 0;
	vSyncPassed = false;
	initParallelOutputMode(pinMap, mode.pixelClock, bitCount, clockPin);
	startTX();

    Serial.printf("BitluniVGA init video\r\n");
	Serial.printf(" hFront:%d hSync:%d hBack:%d hRes:%d vFront:%d\r\n",this->mode.hFront,this->mode.hSync,this->mode.hBack,this->mode.hRes,this->mode.vFront);
	Serial.printf(" vSync:%d vBack:%d vRes:%d vDiv:%d\r\n",this->mode.vSync,this->mode.vBack,this->mode.vRes,this->mode.vDiv);
	Serial.printf(" pixelClock:%ld hSyncPolarity:%d vSyncPolarity:%d\r\n",this->mode.pixelClock,this->mode.hSyncPolarity,this->mode.vSyncPolarity);
	Serial.printf(" linesPerField:%d\r\n",totalLines);

	return true;
}

void VGA::setLineBufferCount(int lineBufferCount)
{
	this->lineBufferCount = lineBufferCount;
}

void VGA::allocateLineBuffers()
{
 Serial.printf("Llamada antes allocateLineBuffers\r\n");
	allocateLineBuffers(lineBufferCount);
 Serial.printf("Llamada despues allocateLineBuffers\r\n");
}

/// simple ringbuffer of blocks of size bytes each
void VGA::allocateLineBuffers(const int lines)
{
    Serial.printf("allocateLineBuffers lines:%d size:%d size:%d\r\n",lines,(lines*sizeof(DMABufferDescriptor)), (lines*(mode.hFront + mode.hSync + mode.hBack + mode.hRes) * bytesPerSample()) );
	Serial.printf(" hFront:%d hSync:%d hBack:%d hRes:%d\r\n",mode.hFront,mode.hSync,mode.hBack,mode.hRes);

	dmaBufferDescriptorCount = lines;
	dmaBufferDescriptors = DMABufferDescriptor::allocateDescriptors(dmaBufferDescriptorCount);
	int bytes = (mode.hFront + mode.hSync + mode.hBack + mode.hRes) * bytesPerSample();
	for (int i = 0; i < dmaBufferDescriptorCount; i++)
	{
		dmaBufferDescriptors[i].setBuffer(DMABufferDescriptor::allocateBuffer(bytes, true), bytes); //front porch + hsync + back porch + pixels
		if (i)
			dmaBufferDescriptors[i - 1].next(dmaBufferDescriptors[i]);
	}
	dmaBufferDescriptors[dmaBufferDescriptorCount - 1].next(dmaBufferDescriptors[0]);
}

///complete ringbuffer from frame
/* No lo necesito
void VGA::allocateLineBuffers(void **frameBuffer)
{
 Serial.printf("allocateLineBuffers **frameBuffer lines:%d size:%d\r\n",(totalLines*2),(totalLines*2*sizeof(DMABufferDescriptor)));
 
 	dmaBufferDescriptorCount = totalLines * 2;
 	int inactiveSamples = (mode.hFront + mode.hSync + mode.hBack + 3) & 0xfffffffc;
 	vSyncInactiveBuffer = DMABufferDescriptor::allocateBuffer(inactiveSamples * bytesPerSample(), true);
 	vSyncActiveBuffer = DMABufferDescriptor::allocateBuffer(mode.hRes * bytesPerSample(), true);
 	inactiveBuffer = DMABufferDescriptor::allocateBuffer(inactiveSamples * bytesPerSample(), true);
 	blankActiveBuffer = DMABufferDescriptor::allocateBuffer(mode.hRes * bytesPerSample(), true);
     
 	Serial.printf("allocateLineBuffers **frameBuffer bytesPerSample:%d vSyncInactiveBuffer:%d\r\n", bytesPerSample(), (inactiveSamples * bytesPerSample()));
 	Serial.printf(" vSyncActiveBuffer:%d inactiveBuffer:%d blankActiveBuffer:%d\r\n", (mode.hRes * bytesPerSample()), (inactiveSamples * bytesPerSample()), (mode.hRes * bytesPerSample()) );
 
 	if(bytesPerSample() == 1)
 	{
 		for (int i = 0; i < inactiveSamples; i++)
 		{
 			if (i >= mode.hFront && i < mode.hFront + mode.hSync)
 			{
 				((unsigned char *)vSyncInactiveBuffer)[i ^ 2] = hsyncBit | vsyncBit;
 				((unsigned char *)inactiveBuffer)[i ^ 2] = hsyncBit | vsyncBitI;
 			}
 			else
 			{
 				((unsigned char *)vSyncInactiveBuffer)[i ^ 2] = hsyncBitI | vsyncBit;
 				((unsigned char *)inactiveBuffer)[i ^ 2] = hsyncBitI | vsyncBitI;
			}
		}
		for (int i = 0; i < mode.hRes; i++)
		{
			((unsigned char *)vSyncActiveBuffer)[i ^ 2] = hsyncBitI | vsyncBit;
			((unsigned char *)blankActiveBuffer)[i ^ 2] = hsyncBitI | vsyncBitI;
		}
	}
	else if(bytesPerSample() == 2)
	{
		for (int i = 0; i < inactiveSamples; i++)
		{
			if (i >= mode.hFront && i < mode.hFront + mode.hSync)
			{
				((unsigned short *)vSyncInactiveBuffer)[i ^ 1] = hsyncBit | vsyncBit;
				((unsigned short *)inactiveBuffer)[i ^ 1] = hsyncBit | vsyncBitI;
			}
			else
			{
				((unsigned short *)vSyncInactiveBuffer)[i ^ 1] = hsyncBitI | vsyncBit;
				((unsigned short *)inactiveBuffer)[i ^ 1] = hsyncBitI | vsyncBitI;
			}
		}
		for (int i = 0; i < mode.hRes; i++)
		{
			((unsigned short *)vSyncActiveBuffer)[i ^ 1] = hsyncBitI | vsyncBit;
			((unsigned short *)blankActiveBuffer)[i ^ 1] = hsyncBitI | vsyncBitI;
		}
	}

	dmaBufferDescriptors = DMABufferDescriptor::allocateDescriptors(dmaBufferDescriptorCount);
	for (int i = 0; i < dmaBufferDescriptorCount; i++)
		dmaBufferDescriptors[i].next(dmaBufferDescriptors[(i + 1) % dmaBufferDescriptorCount]);
	int d = 0;
	for (int i = 0; i < mode.vFront; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(inactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(blankActiveBuffer, mode.hRes * bytesPerSample());
	}
	for (int i = 0; i < mode.vSync; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(vSyncInactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(vSyncActiveBuffer, mode.hRes * bytesPerSample());
	}
	for (int i = 0; i < mode.vBack; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(inactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(blankActiveBuffer, mode.hRes * bytesPerSample());
	}
	for (int i = 0; i < mode.vRes; i++)
	{
		dmaBufferDescriptors[d++].setBuffer(inactiveBuffer, inactiveSamples * bytesPerSample());
		dmaBufferDescriptors[d++].setBuffer(frameBuffer[i / mode.vDiv], mode.hRes * bytesPerSample());
	}
}
*/

void VGA::vSync()
{
	vSyncPassed = true;
}

void VGA::interrupt()
{
	unsigned long *signal = (unsigned long *)dmaBufferDescriptors[dmaBufferDescriptorActive].buffer();
	unsigned long *pixels = &((unsigned long *)dmaBufferDescriptors[dmaBufferDescriptorActive].buffer())[(mode.hSync + mode.hBack) / 2];
	unsigned long base, baseh;
	if (currentLine >= mode.vFront && currentLine < mode.vFront + mode.vSync)
	{
		baseh = (vsyncBit | hsyncBit) | ((vsyncBit | hsyncBit) << 16);
		base = (vsyncBit | hsyncBitI) | ((vsyncBit | hsyncBitI) << 16);
	}
	else
	{
		baseh = (vsyncBitI | hsyncBit) | ((vsyncBitI | hsyncBit) << 16);
		base = (vsyncBitI | hsyncBitI) | ((vsyncBitI | hsyncBitI) << 16);
	}
	for (int i = 0; i < mode.hSync / 2; i++)
		signal[i] = baseh;
	for (int i = mode.hSync / 2; i < (mode.hSync + mode.hBack) / 2; i++)
		signal[i] = base;

	int y = (currentLine - mode.vFront - mode.vSync - mode.vBack) / mode.vDiv;
	if (y >= 0 && y < mode.vRes)
		interruptPixelLine(y, pixels, base);
	else
		for (int i = 0; i < mode.hRes / 2; i++)
		{
			pixels[i] = base | (base << 16);
		}
	for (int i = 0; i < mode.hFront / 2; i++)
		signal[i + (mode.hSync + mode.hBack + mode.hRes) / 2] = base;
	currentLine = (currentLine + 1) % totalLines;
	dmaBufferDescriptorActive = (dmaBufferDescriptorActive + 1) % dmaBufferDescriptorCount;
	if (currentLine == 0)
		vSync();
}

//JJ void VGA::interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits)
void VGA::interruptPixelLine(unsigned short int y, unsigned long *pixels, unsigned long syncBits)
{
}























void VGA3BitI::PrecalculaInterrupt()
{
 precalcula_baseh_bit= (hsyncBit | vsyncBit) * 0x1010101;
 precalcula_base_bit = (hsyncBitI | vsyncBit) * 0x1010101;

 precalcula_baseh_bitI = (hsyncBit | vsyncBitI) * 0x1010101;
 precalcula_base_bitI = (hsyncBitI | vsyncBitI) * 0x1010101;

 
 precalcula_optimiceModehSync2= (mode.hSync>>2);
 precalcula_optimiceModehRes2= (mode.hRes>>2);
 
 precalcula_optimiceSumHfront= ((mode.hSync + mode.hBack + mode.hRes)>>2);	
}


//BitluniVGA3BitI.cpp BEGIN
void IRAM_ATTR VGA3BitI::interrupt(void *arg)
{
 unsigned char a3bpp[2];
 unsigned char p7,p6,p5,p4,p3,p2,p1,p0;

	VGA3BitI * staticthis = (VGA3BitI *)arg;
	
	unsigned long *signal = (unsigned long *)staticthis->dmaBufferDescriptors[staticthis->dmaBufferDescriptorActive].buffer();
	//JJ unsigned long *pixels = &((unsigned long *)staticthis->dmaBufferDescriptors[staticthis->dmaBufferDescriptorActive].buffer())[(staticthis->mode.hSync + staticthis->mode.hBack) / 4];
	unsigned long *pixels = &((unsigned long *)staticthis->dmaBufferDescriptors[staticthis->dmaBufferDescriptorActive].buffer())[(staticthis->mode.hSync + staticthis->mode.hBack)>>2];
	unsigned long base, baseh;
	//unsigned char optimiceModehSync2= (staticthis->mode.hSync>>2);
	//unsigned short int optimiceModehRes2= ((staticthis->mode.hRes)>>2);
	//JJ if (staticthis->currentLine >= staticthis->mode.vFront && staticthis->currentLine < staticthis->mode.vFront + staticthis->mode.vSync)
	//JJ {
	//JJ 	baseh = (staticthis->hsyncBit | staticthis->vsyncBit) * 0x1010101;
	//JJ 	base = (staticthis->hsyncBitI | staticthis->vsyncBit) * 0x1010101;
	//JJ }
	//JJ else
	//JJ {
	//JJ 	baseh = (staticthis->hsyncBit | staticthis->vsyncBitI) * 0x1010101;
	//JJ 	base = (staticthis->hsyncBitI | staticthis->vsyncBitI) * 0x1010101;
	//JJ }

    if (staticthis->currentLine >= staticthis->mode.vFront && staticthis->currentLine < staticthis->mode.vFront + staticthis->mode.vSync)
	{
     baseh= precalcula_baseh_bit;
	 base= precalcula_base_bit;
	}
	else
	{
     baseh= precalcula_baseh_bitI;
	 base= precalcula_base_bitI;
	}

	//JJ for (int i = 0; i < staticthis->mode.hSync / 4; i++)
	//JJ for (int i = 0; i < (staticthis->mode.hSync>>2); i++)
	for (unsigned char i = 0; i < precalcula_optimiceModehSync2; i++)
	{
		signal[i] = baseh;
	}
	//JJ for (int i = staticthis->mode.hSync / 4; i < (staticthis->mode.hSync + staticthis->mode.hBack) / 4; i++)
	//JJ for (int i = (staticthis->mode.hSync>>2); i < ((staticthis->mode.hSync + staticthis->mode.hBack)>>2); i++)
	for (unsigned short int i = precalcula_optimiceModehSync2; i < ((staticthis->mode.hSync + staticthis->mode.hBack)>>2); i++)
	{
		signal[i] = base;
	}

	short int y = (staticthis->currentLine - staticthis->mode.vFront - staticthis->mode.vSync - staticthis->mode.vBack) / staticthis->mode.vDiv;
	if (y >= 0 && y < staticthis->mode.vRes)
	{				
		
		//JJ staticthis->interruptPixelLine(y, pixels, base, arg); //Optimizar mas rapido

//BEGIN interrupt
     unsigned char *line= (gb_vga_text==1) ? staticthis->frontBuffer[(gb_vga_videomode == vgaMode_TextModeSimpleRetro)||((gb_vga_videomode == vgaMode_TextMode3bppRetro)) ? (y>>4):(y>>3)] : staticthis->frontBuffer[y];
     unsigned short j = 0;
		
	if ((gb_vga_videomode == vgaMode_TextModeSimple)||(gb_vga_videomode == vgaMode_TextModeSimpleRetro))
	{
     if ((gb_vga_videomode == vgaMode_TextModeSimpleRetro) && ((y & 0x01) == 1))
	 {      
	 }
	 else	 
	 {
	  if (gb_vga_videomode == vgaMode_TextModeSimpleRetro)
	  {
	   y= y>>1;
	  }
	  //for (unsigned short int i = 0; i < ((staticthis->mode.hRes)>>2); i+=2)
	  for (unsigned short int i = 0; i < precalcula_optimiceModehRes2; i+=2)
	  {
	   unsigned int idCar= line[j++];
	   unsigned int ofsCar= (idCar<<3)+(y&0x07);	  
	   unsigned char auxData= gb_sdl_font_6x8_ram[ofsCar];
 
  	   p7 = gb_colorBW[(auxData & 1)];
 	   p6 = gb_colorBW[((auxData>>1) & 1)];
	   p5 = gb_colorBW[((auxData>>2) & 1)];
	   p4 = gb_colorBW[((auxData>>3) & 1)];
	   p3 = gb_colorBW[((auxData>>4) & 1)];
	   p2 = gb_colorBW[((auxData>>5) & 1)];
	   p1 = gb_colorBW[((auxData>>6) & 1)];
	   p0 = gb_colorBW[((auxData>>7) & 1)];
	   pixels[i] = base | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	   pixels[i+1] = base | (p6 << 0) | (p7 << 8) | (p4 << 16) | (p5 << 24);
	  }	 
	 }
	}
	else
	{
	 if ((gb_vga_videomode == vgaMode_TextMode3bpp)||(gb_vga_videomode == vgaMode_TextMode3bppRetro))
	 {
      if ((gb_vga_videomode == vgaMode_TextMode3bppRetro) && ((y & 0x01) == 1))
	  {      
	  }
	  else	 
	  {
	   if (gb_vga_videomode == vgaMode_TextMode3bppRetro)
	   {
	    y= y>>1;
	   }		  
	   //for (unsigned short int i = 0; i < ((staticthis->mode.hRes)>>2); i+=2)
	   for (unsigned short int i = 0; i < precalcula_optimiceModehRes2; i+=2)
	   {		
	    unsigned int idCar= line[j++];
	    unsigned int auxColor= line[j++];
	    unsigned int ofsCar= (idCar<<3)+(y&0x07);	  
	    //unsigned char auxData= gb_sdl_font_6x8[ofsCar];
	    unsigned char auxData= gb_sdl_font_6x8_ram[ofsCar];
        a3bpp[0]= (auxColor>>4); //backcolor
	    a3bpp[1]= (auxColor&0x07); //forecolor
       
	    //unsigned char foreColor= (auxColor&0x07);
	    //unsigned char backColor= (auxColor>>4); //((auxColor>>4)&0x07);	   

	    //unsigned char p7 = ((auxData & 1)==1) ? foreColor:backColor;
	    //unsigned char p6 = (((auxData>>1) & 1)==1) ? foreColor:backColor;
	    //unsigned char p5 = (((auxData>>2) & 1)==1) ? foreColor:backColor;
	    //unsigned char p4 = (((auxData>>3) & 1)==1) ? foreColor:backColor;
	    //unsigned char p3 = (((auxData>>4) & 1)==1) ? foreColor:backColor;
	    //unsigned char p2 = (((auxData>>5) & 1)==1) ? foreColor:backColor;
	    //unsigned char p1 = (((auxData>>6) & 1)==1) ? foreColor:backColor;
	    //unsigned char p0 = (((auxData>>7) & 1)==1) ? foreColor:backColor;
	   
	    p7 = a3bpp[(auxData & 1)];
	    p6 = a3bpp[((auxData>>1) & 1)];
	    p5 = a3bpp[((auxData>>2) & 1)];
	    p4 = a3bpp[((auxData>>3) & 1)];
	    p3 = a3bpp[((auxData>>4) & 1)];
	    p2 = a3bpp[((auxData>>5) & 1)];
	    p1 = a3bpp[((auxData>>6) & 1)];
	    p0 = a3bpp[((auxData>>7) & 1)]; 

	    pixels[i] = base | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	    pixels[i+1] = base | (p6 << 0) | (p7 << 8) | (p4 << 16) | (p5 << 24);	 
	   }
	  }
	 }
	 else
	 {
	  if (gb_vga_videomode == vgaMode_Bitmap1bpp)
	  {
	   for (unsigned short int i = 0; i < precalcula_optimiceModehRes2; i+=2)
	   {
	    p0 = gb_colorBW[(line[j] & 1)];
	    p1 = gb_colorBW[((line[j]>>1) & 1)];
	    p2 = gb_colorBW[((line[j]>>2) & 1)];
	    p3 = gb_colorBW[((line[j]>>3) & 1)];
	    p4 = gb_colorBW[((line[j]>>4) & 1)];
	    p5 = gb_colorBW[((line[j]>>5) & 1)];
	    p6 = gb_colorBW[((line[j]>>6) & 1)];
	    p7 = gb_colorBW[((line[j++]>>7) & 1)];
	    pixels[i] = base | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	    pixels[i+1] = base | (p6 << 0) | (p7 << 8) | (p4 << 16) | (p5 << 24);	  
	   }       
	  }
	 }
	}
	
//END interrupt



	}
	else
	{
		//JJ for (int i = 0; i < staticthis->mode.hRes / 4; i++)
		//for (int i = 0; i < (staticthis->mode.hRes>>2); i++)
		for (unsigned short int i = 0; i < precalcula_optimiceModehRes2; i++)
		{
			pixels[i] = base;
		}
	}
	//JJ for (int i = 0; i < staticthis->mode.hFront / 4; i++)

	//unsigned short int optimiceSumHfront= ((staticthis->mode.hSync + staticthis->mode.hBack + staticthis->mode.hRes)>>2);	
	for (unsigned char i = 0; i < (staticthis->mode.hFront>>2); i++)
	{
		//JJ signal[i + (staticthis->mode.hSync + staticthis->mode.hBack + staticthis->mode.hRes) / 4] = base;
		//JJ signal[i + ((staticthis->mode.hSync + staticthis->mode.hBack + staticthis->mode.hRes)>>2)] = base;
		signal[i + precalcula_optimiceSumHfront] = base;
	}
	staticthis->currentLine = (staticthis->currentLine + 1) % staticthis->totalLines;
	staticthis->dmaBufferDescriptorActive = (staticthis->dmaBufferDescriptorActive + 1) % staticthis->dmaBufferDescriptorCount;
	if (staticthis->currentLine == 0)
	{
		staticthis->vSyncPassed = true;
	}
}

/*
//JJ void IRAM_ATTR VGA3BitI::interruptPixelLine(int y, unsigned long *pixels, unsigned long syncBits, void *arg)
void IRAM_ATTR VGA3BitI::interruptPixelLine(unsigned short int y, unsigned long *pixels, unsigned long syncBits, void *arg)
{
//	//original
//	VGA3BitI * staticthis = (VGA3BitI *)arg;
//	unsigned char *line = staticthis->frontBuffer[y];
//	int j = 0;
//	for (int i = 0; i < staticthis->mode.hRes / 4; i++)
//	{
//		int p0 = (line[j] >> 0) & 7;
//		int p1 = (line[j++] >> 4) & 7;
//		int p2 = (line[j] >> 0) & 7;
//		int p3 = (line[j++] >> 4) & 7;
//		pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
//	}
	

    //nuevo
	unsigned char *line;
	//unsigned char colorBW[2]={0,0x7};
	VGA3BitI * staticthis = (VGA3BitI *)arg;
	//if (gb_vga_text==1)
	//{	 
	// line= (y>250) ? staticthis->frontBuffer[(200>>3)] : staticthis->frontBuffer[(y>>3)];
	//}
	//else
	//{
	// line= staticthis->frontBuffer[y];
	//}
	line= (gb_vga_text==1) ? staticthis->frontBuffer[(y>>3)] : staticthis->frontBuffer[y];
	unsigned short int j = 0;

    //if (gb_vga_text==1)
	if (gb_vga_videomode == vgaMode_TextModeSimple)
	{
	 //50x37
	 //if ((y&1)==0)
	 //{
	 // for (unsigned short int i = 0; i < 100; i++)
	 // {
	 //  pixels[i] = syncBits | 0;
	 // }
	 //}
	 //else
	 //{
	 //for (unsigned short int i = 0; i < 100; i+=2)	 
	 //40x25 - es 80   320x200 es 40x200  40x2=80  Divido 320/4
	 for (unsigned short int i = 0; i < ((staticthis->mode.hRes)>>2); i+=2)
	 {
	  unsigned int idCar= line[j++];
	  unsigned int ofsCar= (idCar<<3)+(y&0x07);
	  //unsigned int ofsCar= (idCar<<3)+((y>>1)&0x07);
	  //unsigned char auxData= gb_sdl_font_6x8[ofsCar];
	  unsigned char auxData= gb_sdl_font_6x8_ram[ofsCar];

	  unsigned char p7 = gb_colorBW[(auxData & 1)];
	  unsigned char p6 = gb_colorBW[((auxData>>1) & 1)];
	  unsigned char p5 = gb_colorBW[((auxData>>2) & 1)];
	  unsigned char p4 = gb_colorBW[((auxData>>3) & 1)];
	  unsigned char p3 = gb_colorBW[((auxData>>4) & 1)];
	  unsigned char p2 = gb_colorBW[((auxData>>5) & 1)];
	  unsigned char p1 = gb_colorBW[((auxData>>6) & 1)];
	  unsigned char p0 = gb_colorBW[((auxData>>7) & 1)];
	  pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	  pixels[i+1] = syncBits | (p6 << 0) | (p7 << 8) | (p4 << 16) | (p5 << 24);
	 }
	 //}
	 return;
	}

	if (gb_vga_videomode == vgaMode_TextMode3bpp)
	{	 
	 for (unsigned short int i = 0; i < ((staticthis->mode.hRes)>>2); i+=2)
	 {		
	  unsigned int idCar= line[j++];
	  unsigned int auxColor= line[j++];
	  unsigned int ofsCar= (idCar<<3)+(y&0x07);	  
	  //unsigned char auxData= gb_sdl_font_6x8[ofsCar];
	  unsigned char auxData= gb_sdl_font_6x8_ram[ofsCar];
	  unsigned char foreColor= (auxColor&0x07);
	  unsigned char backColor= ((auxColor>>4)&0x07);

	  unsigned char p7 = gb_color3bpp[((auxData & 1)==1) ? foreColor:backColor];
	  unsigned char p6 = gb_color3bpp[(((auxData>>1) & 1)==1) ? foreColor:backColor];
	  unsigned char p5 = gb_color3bpp[(((auxData>>2) & 1)==1) ? foreColor:backColor];
	  unsigned char p4 = gb_color3bpp[(((auxData>>3) & 1)==1) ? foreColor:backColor];
	  unsigned char p3 = gb_colorBW[(((auxData>>4) & 1)==1) ? foreColor:backColor];
	  unsigned char p2 = gb_color3bpp[(((auxData>>5) & 1)==1) ? foreColor:backColor];
	  unsigned char p1 = gb_color3bpp[(((auxData>>6) & 1)==1) ? foreColor:backColor];
	  unsigned char p0 = gb_color3bpp[(((auxData>>7) & 1)==1) ? foreColor:backColor];
	  pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	  pixels[i+1] = syncBits | (p6 << 0) | (p7 << 8) | (p4 << 16) | (p5 << 24);	 
	 }
	 return;
	}

	//for (int i = 0; i < staticthis->mode.hRes / 4; i++)
	//if (gb_vga_1bpp==1)
	if (gb_vga_videomode == vgaMode_Bitmap1bpp)
	{//moo 1 bpp
	 for (unsigned short int i = 0; i < (staticthis->mode.hRes >>2); i+=2)
	 {
	  unsigned char p0 = gb_colorBW[(line[j] & 1)];
	  unsigned char p1 = gb_colorBW[((line[j]>>1) & 1)];
	  unsigned char p2 = gb_colorBW[((line[j]>>2) & 1)];
	  unsigned char p3 = gb_colorBW[((line[j]>>3) & 1)];
	  unsigned char p4 = gb_colorBW[((line[j]>>4) & 1)];
	  unsigned char p5 = gb_colorBW[((line[j]>>5) & 1)];
	  unsigned char p6 = gb_colorBW[((line[j]>>6) & 1)];
	  unsigned char p7 = gb_colorBW[((line[j++]>>7) & 1)];
	  pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	  pixels[i+1] = syncBits | (p6 << 0) | (p7 << 8) | (p4 << 16) | (p5 << 24);
	  //pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	 }
	}
	//else
	//{//modo 4 bpp
	// for (int i = 0; i < staticthis->mode.hRes / 4; i++)
	// {
	//  unsigned char p0 = (line[j] >> 0) & 7;
	//  unsigned char p1 = (line[j++] >> 4) & 7;
	//  unsigned char p2 = (line[j] >> 0) & 7;
	//  unsigned char p3 = (line[j++] >> 4) & 7;
	//  pixels[i] = syncBits | (p2 << 0) | (p3 << 8) | (p0 << 16) | (p1 << 24);
	// }		
	//}	
}
*/
//BitluniVGA3BitI.cpp END

