#ifndef BUZZER_ORIG
#define BUZZER_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Count.h"
#include "Debug.h"

class Buzzer {
	public:
		Buzzer(uint8_t);
		void set(uint8_t = 88, uint16_t = 0, bool = false, uint16_t = 0);
		void beep();
		void reset();
		uint8_t noteMap(double, double, double);
	private:
		uint8_t PORT;

		uint16_t crtNote;
		uint16_t prvNote;
		uint64_t timeStart;
		uint64_t period;
		uint16_t rankBeep;
		bool haveBeep = false;

		const uint8_t QTY_NOTE = 89;
		const uint8_t INDEX_A = 46;
		const uint16_t NOTE_A = 440;
		const double R = 1.059463;
		uint16_t *NOTE;
		double *D_NOTE;

		const uint8_t LOUDNESS = 122;
};

#endif