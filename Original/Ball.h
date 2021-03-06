#ifndef BALL_ORIG
#define BALL_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Count.h"
#include "Debug.h"

class Ball {
	public:
		Ball(uint8_t, uint8_t*,
			uint8_t, uint16_t, double, double, double,
			uint8_t, double*, uint8_t, double**, double**,
			uint8_t, uint16_t, uint16_t, uint16_t);
		vectorRT_t get();
		uint16_t *getVal();
		uint8_t getQTY();

		uint16_t getForward();

		Angle getDir(vectorRT_t);

		bool getCatch();
		bool getMayKick();
		bool compareCatch(double);
		uint16_t getValCatch();
	private:
		uint8_t QTY;
		uint8_t *PORT;

		double *COS_IR;
		double *SIN_IR;

		const uint16_t CYCLE = 837;
		uint8_t MEASURING_COUNT;
		uint16_t THRE_WEAK;
		double CHANGE_RATE;
		double CHANGE_RATE_T;
		double PLUS_T;

		uint8_t SIZE_THRE_DIST;
		double *THRE_DIST;
		uint8_t SIZE_DIR;
		double **DIR;
		double **PLUS_DIR;

		uint16_t *val;
		uint16_t *weak;
		uint16_t *prv;
		uint16_t *crt;

		vectorRT_t prvBall;

		uint8_t P_CATCH;
		uint16_t THRE_CATCH;

		uint16_t valCatch;
		Count cCatch;
		Count cMayKick;
};

#endif