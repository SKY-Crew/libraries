#ifndef PSD_ORIG
#define PSD_ORIG

#include "Arduino.h"
#include "Wire.h"
#include "advMath.h"

class PSD {
	public:
		PSD(uint8_t get_X, double get_MULTI_AVG, uint16_t get_BORDER_IS_CLOSE, uint8_t get_MAX_CC);
		bool get();
		double getValue();
	private:
		TwoWire *WireX;

		double MULTI_AVG;
		uint16_t BORDER_IS_CLOSE;

		double value;
		double prv = 0;

		uint8_t MAX_CC;
		uint8_t countClose = 0;
};

#endif