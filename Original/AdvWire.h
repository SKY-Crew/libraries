#ifndef WIRE_ORIG
#define WIRE_ORIG

#include <Arduino.h>
#include <Wire.h>
#include "AdvMath.h"

class AdvWire {
	private:
		TwoWire *WireX;
	public:
		inline AdvWire() {  }
		inline AdvWire(uint8_t X) { set(X); }

		inline void set(uint8_t X) {
			switch(X) {
				#ifdef CORE_TEENSY
				case 1: WireX = &Wire1; break;
				case 2: WireX = &Wire2; break;
				#endif
				default: WireX = &Wire; break;
			}
		}
		inline TwoWire* get() { return WireX; }
};

#endif