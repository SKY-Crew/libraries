#ifndef WIRE_ORIG
#define WIRE_ORIG

#include <Arduino.h>
#include "advMath.h"
#include "Wire.h"

class advWire {
	private:
		TwoWire *WireX;
	public:
		inline advWire() {  }
		inline advWire(uint8_t given_X) { set(given_X); }

		inline void set(uint8_t given_X) {
			switch(given_X) {
				case 0: WireX = &Wire; break;
				case 1: WireX = &Wire1; break;
				case 2: WireX = &Wire2; break;
			}
		}
		inline TwoWire get() { return *WireX; }
};

#endif