#ifndef KICKER_ORIG
#define KICKER_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Count.h"

class Kicker {
	public:
		Kicker(uint8_t, uint8_t, uint16_t, uint16_t, uint16_t);
		void run(bool);
		void check();
		bool getIsKicking();
		void setHaveChecked(bool);
		void setPower(int16_t = -1);
	private:
		uint8_t P_KICKER;
		uint8_t P_RUN_KICKER;

		uint16_t MAX_CK;
		Count cKicking;
		uint16_t MAX_CKW;
		Count cWait;

		bool kicking = false;
		bool haveChecked = false;
		Count cStart;
		bool prvStart;
};

#endif