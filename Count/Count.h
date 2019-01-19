#ifndef COUNT_ORIG
#define COUNT_ORIG

#include "Arduino.h"
#include "advMath.h"

class Count {
	private:
		int16_t MAX;
		int16_t MIN;
		int16_t count;
		bool COUNT_UP;
	public:
		inline Count(int16_t given_MAX = 0, bool given_COUNT_UP = true, int16_t given_MIN = 0) {
			set_MAX(given_MAX);
			set_COUNT_UP(given_COUNT_UP);
			set_MIN(given_MIN);
			count = MIN;
		}

		inline operator int16_t() const{ return count; }
		inline explicit operator bool() const{ return COUNT_UP ? count >= MAX : count > MIN; }

		inline void set_MAX(int16_t given_MAX) { MAX = given_MAX; }
		inline void set_MIN(int16_t given_MIN) { MIN = given_MIN; }
		inline void set_COUNT_UP(int16_t given_COUNT_UP) { COUNT_UP = given_COUNT_UP; }

		inline void reset(int16_t given_count = -1) {
			if(given_count == -1) {
				given_count = COUNT_UP ? MIN : MAX;
			}
			count = constrain(given_count, MIN, MAX);
		}
		inline void increment(bool canIncrement = true) {
			count = canIncrement ? (COUNT_UP ? MIN : MAX) : constrain(COUNT_UP ? ++ count : -- count, MIN, MAX);
		}
		inline bool compare(double rate) {
			return rate < 0 ? true
				: rate == 0 ? count > MIN
				: rate <= 1 ? count >= MAX * rate + MIN * (1 - rate)
				: false;
		}
};

#endif