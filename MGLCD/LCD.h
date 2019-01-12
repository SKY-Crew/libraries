#ifndef LCD_ORIG
#define LCD_ORIG

#include "Arduino.h"
#include "AdvMath.h"
#include "MGLCD.h"
#include "Line.h"
#include "Comc.h"
#include "Cam.h"

#define OOO char(160)

class LCD {
	public:
		LCD(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
		void run(int16_t, line_t, bool, bool, bool, bool, bool, comc_t,
			uint8_t, uint16_t*, uint8_t*, double, cam_t,
			vectorRT_t, uint8_t, uint16_t*,
			uint16_t, bool, uint16_t, bool, bool,
			uint16_t, bool, uint16_t, bool);
		void run4IR(uint8_t, uint16_t*, uint16_t*, double*);
		void clear(bool);
		void write(String, uint8_t, uint8_t);
		void drawMeter(int16_t, uint8_t, bool);
		void drawAngelRing(uint8_t, uint8_t*, uint8_t);
		void redraw();
	private:
		uint8_t P_REDRAW;
		uint8_t MAX_CP;
		uint8_t MAX_PAGE;
		MGLCD_AQM1248A_SoftwareSPI *lcd;

		uint8_t QTY_LINE;
		double *SIN_LINE;
		double *COS_LINE;

		uint8_t countPrint = 0;
		uint8_t page = 0;
};

#endif