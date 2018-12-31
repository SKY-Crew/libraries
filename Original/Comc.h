#ifndef COMC_ORIG
#define COMC_ORIG

#include "Arduino.h"
#include "AdvMath.h"

typedef struct {
	bool exists;
	bool isFW;
} comc_t;

class Comc {
	public:
		Comc(uint8_t get_X, uint8_t get_P_ONOFF, uint16_t get_MAX_C_SND, uint16_t get_MAX_C_NR);
		comc_t communicate(bool canRun, bool isFW);
		bool getCanUse();
	private:
		void sndWireless(bool canRun, bool isFW);
		bool rcvWireless();

		HardwareSerial *SerialX;

		uint8_t P_ONOFF;
		uint16_t MAX_C_SND;
		uint16_t MAX_C_NR;

		uint16_t countSnd = 0;
		uint16_t countNoRcv = 0;

		comc_t prvFellow;
};

#endif