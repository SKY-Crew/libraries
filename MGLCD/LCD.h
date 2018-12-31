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
		LCD(uint8_t get_P_REDRAW, uint8_t get_P_SCK, uint8_t get_P_MOSI, uint8_t get_P_CS, uint8_t get_P_DI,
			uint32_t get_WAIT, uint8_t get_MAX_CP, uint8_t get_QTY_LINE);
		void run(int16_t gyro, line_t line, bool canUseKicker, bool canUseCam, bool canUseGyro, bool isFW, bool canUseComc, comc_t fellow,
			uint8_t QTY_LINE, uint16_t *valueLine, uint8_t *stateLine, double valueIna219, cam_t goal,
			vectorRT_t ball, uint8_t QTY_IR, uint16_t *valueIR,
			uint16_t valueCatch, bool catchingBall, uint16_t valueBF, bool isBallForward, bool isBallClose,
			uint16_t valueFrontPSD, bool enemyStandsFront, uint16_t valueBackPSD, bool isGoalClosePSD);
		void run4IR(uint8_t QTY, uint16_t *maxIR, uint16_t *minIR, double *avgIR);
		void clear(bool resetPage);
		void write(String data, uint8_t XCoord, uint8_t YCoord);
		void drawMeter(int16_t angleM,uint8_t XCoordM, bool canDrawCircle);
		void drawAngelRing(uint8_t QTY_LINE, uint8_t *stateLine, uint8_t XCoord);
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