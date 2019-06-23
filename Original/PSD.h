#ifndef PSD_ORIG
#define PSD_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Count.h"
#include "Debug.h"
#include "AdvWire.h"

class PSD {
	public:
		PSD(uint8_t, double, uint16_t, uint8_t, uint8_t);
		void get();
		bool getBool(bool = true);
		double getVal(bool = true);
	private:
		uint8_t PORT;

		double CHANGE_RATE;
		uint16_t THRE_IS_CLOSE;

		double val;
		double prv = 0;
		Count cCannotSee;

		Count cClose;
};

#endif