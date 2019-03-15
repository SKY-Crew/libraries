#ifndef BALL_ORIG
#define BALL_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "Angle.h"
#include "Count.h"

class Ball {
	public:
		Ball(uint8_t, uint8_t*,
			uint8_t, uint16_t, double,
			uint16_t*, uint8_t, double(*)[2], double(*)[2], double(*)[2],
			uint8_t, uint16_t, uint8_t);
		vectorRT_t get(bool = false);
		uint16_t getForward();
		Angle getDir(vectorRT_t);
		bool getCatch();
		bool compareCatch(double);
		uint16_t *getValue();
		uint8_t getQTY();
		uint16_t getValueCatch();
	private:
		uint8_t QTY;
		uint8_t *PORT;

		double *COS_IR;
		double *SIN_IR;

		const uint16_t CYCLE = 837;
		uint8_t MEASURING_COUNT;
		uint16_t BORDER_WEAK;
		double MULTI_AVG;

		uint16_t BORDER_DIST[2];
		uint8_t SIZE_SLOPE_DIR;
		double (*SLOPE_DIR)[2];
		double (*INTERCEPT_DIR)[2];
		double (*POINT_DIR)[2];

		uint16_t *value;
		uint16_t *weak;
		uint16_t *prv;
		uint16_t *crt;

		uint8_t P_CATCH;
		uint16_t BORDER_CATCH;

		uint16_t valueCatch;
		Count cCatch;
};

#endif