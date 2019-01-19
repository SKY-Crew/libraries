#ifndef COUNT_ORIG
#define COUNT_ORIG

#include "Arduino.h"
#include "advMath.h"

class Count {
	private:
		int MAX;
		int MIN;
		int count;
		bool COUNT_UP;
	public:
		inline Count(int given_MAX = 0, bool given_COUNT_UP = true, int given_MIN = 0) {
			set_MAX(given_MAX);
			set_COUNT_UP(given_COUNT_UP);
			set_MIN(given_MIN);
			count = MIN;
		}

		inline operator int() const{ return count; }
		inline explicit operator bool() const{ return COUNT_UP ? count >= MAX : count > MIN; }

		inline void set_MAX(int given_MAX) { MAX = given_MAX; }
		inline void set_MIN(int given_MIN) { MIN = given_MIN; }
		inline void set_COUNT_UP(int given_COUNT_UP) { COUNT_UP = given_COUNT_UP; }

		inline void reset(int set_count = -1) {
			if(set_count == -1) {
				set_count = COUNT_UP ? MIN : MAX;
			}
			count = constrain(set_count, MIN, MAX);
		}
		inline void increment(bool canIncrement = true) {
			count = canIncrement ? count : constrain(COUNT_UP ? ++ count : -- count, MIN, MAX);
		}
		inline bool compare(double rate) {
			return rate < 0 ? true
				: rate == 0 ? count > MIN
				: rate <= 1 ? count >= MAX * rate + MIN * (1 - rate)
				: false;
		}
};

#endif