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
		inline AdvWire(uint8_t P_WIRE) { set(P_WIRE); }

		inline void set(uint8_t P_WIRE) {
			switch(P_WIRE) {
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