#ifndef COMC_ORIG
#define COMC_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Debug.h"
#include "AdvSerial.h"

typedef struct {
	bool exists;
	bool isFW;
	double ball_r;
	double distOwn;
	bool allowChangeRole;
	bool isInAir;
} comc_t;

class Comc {
	public:
		Comc(uint8_t, uint8_t, uint16_t, uint16_t);
		comc_t rcv(bool);
		void snd(bool, bool, double, double, bool, bool);
		bool getCanUse();
	private:
		void sndWireless(bool, uint8_t);
		comc_t rcvWireless();

		AdvSerial sComc;

		uint8_t P_ONOFF;
		uint16_t MAX_C_SND;
		uint16_t MAX_C_NR;

		uint16_t countSnd = 0;
		uint16_t countNoRcv = 0;

		comc_t prvFellow;
};

#endif