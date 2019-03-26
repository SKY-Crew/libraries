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
			uint16_t*, uint8_t, double(*)[2], double(*)[2],
			uint8_t, uint16_t, uint8_t,
			uint8_t, uint16_t);
		vectorRT_t get(bool = false);
		uint16_t getForward();
		Angle getDir(vectorRT_t);
		bool getCatch();
		bool compareCatch(double);
		uint16_t *getValue();
		uint8_t getQTY();
		uint16_t getValueCatch();
		bool isUp();
	private:
		uint8_t QTY;
		uint8_t *PORT;

		double *COS_IR;
		double *SIN_IR;

		const uint16_t CYCLE = 837;
		uint8_t MEASURING_COUNT;
		uint16_t THRE_WEAK;
		double CHANGE_RATE;

		uint16_t THRE_DIST[2];
		uint8_t SIZE_POINT_DIR;
		double (*POINT_DIR)[2];
		double (*PLUS_DIR)[2];

		uint16_t *value;
		uint16_t *weak;
		uint16_t *prv;
		uint16_t *crt;


		uint8_t P_CATCH;
		uint16_t THRE_CATCH;

		uint16_t valueCatch;
		Count cCatch;


		uint8_t P_UP;
		uint16_t THRE_UP;

		uint16_t valueUp;
		uint16_t upDiff;
};

#endif