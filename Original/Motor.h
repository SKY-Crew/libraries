#ifndef MOTOR_ORIG
#define MOTOR_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "Angle.h"

class Motor {
	public:
		Motor(bool, uint8_t, uint8_t*, uint8_t*, int16_t, double, double);
		void run(Angle, int16_t, uint16_t);
		void spin(uint8_t, int16_t);
		void setHaveRun(bool);
	private:
		bool CAN_MOVE;
		uint8_t QTY;
		uint8_t *P_DIR;
		uint8_t *P_PWR;
		Angle *ROT_MOTOR;
		Angle *ROT_WHEEL;
		double *COS_RW;
		double *SIN_RW;
		double MAX_DVP;
		double SLOPE_POWER;
		double INTERCEPT_POWER;
		bool haveRun = false;
};

#endif