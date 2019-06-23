#ifndef PlD_ORIG
#define PlD_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Debug.h"

class PlD {
	public:
		PlD();
		void set(uint8_t, double*, double*, double);
		int16_t cal(Angle, Angle = 0, bool = false);
	private:
		uint8_t SIZE_POINT;
		double *POINT;
		double *ROT;
		double Kd;

		Angle prv = 0;
		double error[2] = {0, 0}; // Pl, D
};

#endif