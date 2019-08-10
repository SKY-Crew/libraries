#ifndef SERIAL_ORIG
#define SERIAL_ORIG

#include <Arduino.h>

#include "AdvMath.h"

class AdvSerial {
	private:
		HardwareSerial *SerialX;
	public:
		inline AdvSerial() {  }
		inline AdvSerial(uint8_t P_SERIAL) { set(P_SERIAL); }

		inline void set(uint8_t P_SERIAL) {
			switch(P_SERIAL) {
				#ifdef CORE_TEENSY
					#ifdef HAS_KINETISK_UART1
						case 2: SerialX = &Serial2; break;
					#endif
					#ifdef HAS_KINETISK_UART2
						case 3: SerialX = &Serial3; break;
					#endif
					#ifdef HAS_KINETISK_UART3
						case 4: SerialX = &Serial4; break;
					#endif
					#ifdef HAS_KINETISK_UART4
						case 5: SerialX = &Serial5; break;
					#endif
					#ifdef HAS_KINETISK_UART5
						case 6: SerialX = &Serial6; break;
					#endif
				#else
					default: SerialX = &Serial; break;
				#endif
			}
		}
		inline HardwareSerial* get() { return SerialX; }
};

#endif