#ifndef ACTUATOR_ORIG
#define ACTUATOR_ORIG

#include "Arduino.h"
#include "AdvMath.h"

class Actuator {
	public:
		Actuator(bool get_CAN_MOVE, uint8_t get_QTY, uint8_t *get_P_DIR, uint8_t *get_P_PWR,
			int16_t firstRM, double get_SLOPE_POWER, double get_INTERCEPT_POWER,
			uint8_t get_P_KICKER, uint8_t get_P_ONOFF_KICKER, uint8_t get_P_RUN_KICKER, uint16_t get_MAX_CK, uint16_t get_MAX_CKW);
		void run(int16_t moveAngle, int16_t rotPower, uint16_t maxPower);
		void spin(uint8_t port, int16_t power);
		void setHaveRun(bool get_haveRun);
		void kick(bool startKick);
		void checkKick();
		bool getCanUseKicker();
		bool getIsKicking();
	private:
		bool CAN_MOVE;
		uint8_t QTY;
		uint8_t *P_DIR;
		uint8_t *P_PWR;
		int16_t *ROT_MOTOR;
		int16_t *ROT_WHEEL;
		double *SIN_RW;
		double *COS_RW;
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
};

#endif