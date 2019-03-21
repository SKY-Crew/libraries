#ifndef KICKER_ORIG
#define KICKER_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "Angle.h"

class Kicker {
	public:
		Kicker(uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
		void kick(bool);
		void check();
		bool getCanUse();
		bool getIsKicking();
		void setHaveChecked(bool);
	private:
		uint8_t P_KICKER;
		uint8_t P_ONOFF_KICKER;
		uint8_t P_RUN_KICKER;
		uint16_t MAX_CK;
		uint16_t MAX_CKW;
		uint8_t countKick = 0;
		bool kicking = false;
		bool haveChecked = false;
		bool prvStart = false;
};

#endif