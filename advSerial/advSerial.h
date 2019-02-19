#ifndef SERIAL_ORIG
#define SERIAL_ORIG

#include <Arduino.h>
#include "advMath.h"

class advSerial {
	private:
		HardwareSerial *SerialX;
	public:
		inline advSerial() {  }
		inline advSerial(uint8_t given_X) { set(given_X); }

		inline void set(uint8_t given_X) {
			switch(given_X) {
				case 1: SerialX = &Serial1; break;
				case 2: SerialX = &Serial2; break;
				case 3: SerialX = &Serial3; break;
				case 4: SerialX = &Serial4; break;
				case 5: SerialX = &Serial5; break;
				case 6: SerialX = &Serial6; break;
			}
		}
		inline HardwareSerial get() { return *SerialX; }
};

#endif