#ifndef BUZZER_ORIG
#define BUZZER_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Count.h"
#include "Debug.h"

class Buzzer {
	public:
		Buzzer(uint8_t);
		void set(uint8_t, uint16_t);
		void beep(bool);
	private:
		uint8_t PORT;

		uint64_t timeStart;
		uint64_t period;

		const uint8_t QTY_NOTE = 89;
		const uint8_t INDEX_A = 46;
		const uint16_t NOTE_A = 440;
		const double R = 1.059463;
		uint16_t *NOTE;
		double *D_NOTE;

		const uint8_t LOUDNESS = 122;
};

#endif