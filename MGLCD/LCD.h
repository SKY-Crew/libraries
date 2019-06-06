#ifndef LCD_ORIG
#define LCD_ORIG

#include <Arduino.h>
#include "MGLCD.h"
#include "Line.h"
#include "Comc.h"
#include "Cam.h"

#include "AdvMath.h"
#include "Angle.h"

#define OOO str(char(160))
#define XXX str("-")

class LCD {
	public:
		LCD(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint32_t, uint8_t, Angle, uint8_t);
		void run(Angle, line_t, bool, bool, bool, bool, comc_t,
			uint8_t, uint16_t*, Color*, double, cam_t,
			vectorRT_t, uint8_t, uint16_t*,
			uint16_t, bool, uint16_t, bool, Dist,
			uint16_t, uint16_t, bool*, Dist, Dist);
		void run4IR(uint8_t, uint16_t*, uint16_t*, double*);
		void clear(bool);
		void write(String, uint8_t, uint8_t);
		void drawMeter(Angle, uint8_t, bool);
		void drawAngelRing(uint8_t, Color*, uint8_t);
		void redraw();
	private:
		uint8_t P_REDRAW;
		uint8_t MAX_CP;
		uint8_t MAX_PAGE;
		MGLCD_AQM1248A_SoftwareSPI *lcd;

		Angle PLUS_T;

		uint8_t QTY_LINE;
		double *SIN_LINE;
		double *COS_LINE;

		uint8_t countPrint = 0;
		uint8_t page = 0;
		bool prvRedraw = false;
};

#endif