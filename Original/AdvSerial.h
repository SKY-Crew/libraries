#ifndef SERIAL_ORIG
#define SERIAL_ORIG

#include <Arduino.h>
#include "AdvMath.h"

class AdvSerial {
	private:
		HardwareSerial *SerialX;
	public:
		inline AdvSerial() {  }
		inline AdvSerial(uint8_t X) { set(X); }

		inline void set(uint8_t X) {
			switch(X) {
				#ifdef CORE_TEENSY
					case 1: SerialX = &Serial1; break;
					case 2: SerialX = &Serial2; break;
					case 3: SerialX = &Serial3; break;
					case 4: SerialX = &Serial4; break;
					case 5: SerialX = &Serial5; break;
					case 6: SerialX = &Serial6; break;
				#else
					default: SerialX = &Serial; break;
				#endif
			}
		}
		inline HardwareSerial* get() { return SerialX; }
};

#endif