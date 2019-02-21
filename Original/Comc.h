#ifndef COMC_ORIG
#define COMC_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "AdvSerial.h"

typedef struct {
	bool exists;
	bool isFW;
} comc_t;

class Comc {
	public:
		Comc(uint8_t, uint8_t, uint16_t, uint16_t);
		comc_t communicate(bool, bool);
		bool getCanUse();
	private:
		void sndWireless(bool, bool);
		bool rcvWireless();

		AdvSerial sComc;

		uint8_t P_ONOFF;
		uint16_t MAX_C_SND;
		uint16_t MAX_C_NR;

		uint16_t countSnd = 0;
		uint16_t countNoRcv = 0;

		comc_t prvFellow;
};

#endif