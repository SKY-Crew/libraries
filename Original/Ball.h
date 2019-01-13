#ifndef BALL_ORIG
#define BALL_ORIG

#include "Arduino.h"
#include "AdvMath.h"

class Ball {
	public:
		Ball(uint8_t, uint8_t*, uint16_t*, uint16_t*, double,
			uint8_t, double(*)[2], double(*)[2], double(*)[2],
			uint8_t, uint16_t, uint8_t);
		vectorRT_t get(bool);
		uint16_t getForward();
		Angle getDir(Angle, bool);
		bool getCatching();
		uint16_t *getValue();
		uint8_t getQTY();
		uint16_t getValueCatch();
		uint8_t getCountCatch();
		uint8_t getMAX_C_CATCH();
	private:
		uint8_t QTY;
		uint8_t *PORT;

		double *COS_IR;
		double *SIN_IR;

		uint16_t *MAX_IR;
		uint16_t *AVG_IR;
		uint16_t avg_MAX_IR = 0;
		uint16_t avg_AVG_IR = 0;

		double MULTI_AVG;

		uint8_t QTY_SLOPE_DIR;
		double (*SLOPE_DIR)[2];
		double (*INTERCEPT_DIR)[2];
		double (*POINT_DIR)[2];

		uint16_t *value;
		uint16_t *prv;
		uint16_t *crt;

		uint8_t P_CATCH;
		uint16_t BORDER_CATCH;
		uint8_t MAX_C_CATCH;

		uint16_t valueCatch;
		uint8_t countCatch;
};

#endif