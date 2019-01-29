#ifndef PSD_ORIG
#define PSD_ORIG

#include <Arduino.h>
#include "Wire.h"
#include "AdvMath.h"
#include "Count.h"

class PSD {
	public:
		PSD(uint8_t, double, uint16_t, uint8_t);
		bool get();
		double getValue();
	private:
		TwoWire *WireX;

		double MULTI_AVG;
		uint16_t BORDER_IS_CLOSE;

		double value;
		double prv = 0;

		Count cClose = 0;
};

#endif