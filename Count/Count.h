#ifndef COUNT_ORIG
#define COUNT_ORIG

#include <Arduino.h>
#include "AdvMath.h"

class Count {
	private:
		int16_t MAX;
		int16_t MIN;
		int16_t count;
		bool COUNT_UP;
	public:
		inline Count(int16_t MAX = 0, bool COUNT_UP = true, int16_t MIN = 0) {
			set_MAX(MAX);
			set_COUNT_UP(COUNT_UP);
			set_MIN(MIN);
			count = MIN;
		}

		inline explicit operator int16_t() const{ return count; }
		inline explicit operator bool() const{ return COUNT_UP ? count >= MAX : count > MIN; }

		inline void set_MAX(int16_t MAX) { this->MAX = MAX; }
		inline void set_MIN(int16_t MIN) { this->MIN = MIN; }
		inline void set_COUNT_UP(int16_t COUNT_UP) { this->COUNT_UP = COUNT_UP; }

		inline void reset(int16_t count = -1) {
			if(count == -1) {
				count = COUNT_UP ? MIN : MAX;
			}
			this->count = constrain(count, MIN, MAX);
		}
		inline void increase(bool canIncrement = true) {
			count = COUNT_UP ?
				(canIncrement ? constrain(++ count, MIN, MAX) : MIN)
				: (canIncrement ? MAX : constrain(-- count, MIN, MAX));
		}
		inline bool compare(double rate) {
			return rate < 0 ? true
				: rate == 0 ? count > MIN
				: rate <= 1 ? count >= MAX * rate + MIN * (1 - rate)
				: false;
		}
};

#endif