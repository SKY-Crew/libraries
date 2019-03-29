#ifndef PSD_ORIG
#define PSD_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "Count.h"
#include "AdvWire.h"

class PSD {
	public:
		PSD(uint8_t, double, uint16_t, uint8_t);
		void get();
		bool getBool(bool = true);
		double getValue(bool = true);
	private:
		AdvWire wPSD;

		double CHANGE_RATE;
		uint16_t THRE_IS_CLOSE;

		double value;
		double prv = 0;

		Count cClose = 0;
};

#endif