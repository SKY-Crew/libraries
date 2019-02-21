#ifndef ACTUATOR_ORIG
#define ACTUATOR_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "Angle.h"

class Actuator {
	public:
		Actuator(bool, uint8_t, uint8_t*, uint8_t*, int16_t, double, double,
			uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
		void run(Angle, int16_t, uint16_t);
		void spin(uint8_t, int16_t);
		void setHaveRun(bool);
		void kick(bool);
		void checkKick();
		bool getCanUseKicker();
		bool getIsKicking();
		void setHaveCheckKick(bool);
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

		uint8_t P_KICKER;
		uint8_t P_ONOFF_KICKER;
		uint8_t P_RUN_KICKER;
		uint16_t MAX_CK;
		uint16_t MAX_CKW;
		uint8_t countKick = 0;
		bool kicking = false;
		bool haveCheckKick = false;
};

#endif