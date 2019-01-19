#ifndef ANGLE_ORIG
#define ANGLE_ORIG

#include "Arduino.h"
#include "advMath.h"

class Count {
	private:
		int MAX;
		int MIN;
		int count;
		bool COUNT_UP;
	public:
		inline Count(int given_MAX, bool given_COUNT_UP = true, int given_MIN = 0) {
			MAX = given_MAX;
			MIN = given_MIN;
			COUNT_UP = given_COUNT_UP;
			count = MIN;
		}

		inline operator int() const{ return count; }
		inline explicit operator bool() const{ return COUNT_UP ? count >= MAX : count > MIN; }

		inline void reset(int set_count = -1) {
			if(set_count == -1) {
				set_count = COUNT_UP ? MIN : MAX;
			}
			count = constrain(set_count, MIN, MAX);
		}
		inline void increment() {
			count = constrain(COUNT_UP ? ++ count : -- count, MIN, MAX);
		}
		inline bool compare(double rate) {
			return rate < 0 ? true
				: rate == 0 ? count > MIN
				: rate <= 1 ? count >= MAX * rate + MIN * (1 - rate)
				: false;
		}
};

#endif