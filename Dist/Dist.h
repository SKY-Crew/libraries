#ifndef DIST_ORIG
#define DIST_ORIG

#include <Arduino.h>
#include "AdvMath.h"

enum Dist {
	TOO_CLOSE, CLOSE, PROPER, FAR, TOO_FAR
};

const String DIST_NAME[] = {"TOO_CLOSE", "CLOSE", "PROPER", "FAR", "TOO_FAR"};
const char DIST_CHAR[] = {'|', '!', '+', '*', '#'};
const uint8_t DIST_TYPE = 5;

inline String toString(const Dist& d) {
	return DIST_NAME[d];
}
inline char toChar(const Dist& d) {
	return DIST_CHAR[d];
}

inline Dist compare(uint16_t val, const uint16_t* THRE, uint16_t SIZE, bool proportional, Dist MIN) {
	int index = 0;
	for (; index < SIZE - 1; index ++) {
		if(proportional) {
			if(val <= THRE[index]) { break; }
		}else {
			if(val > THRE[index]) { break; }
		}
	}
	return Dist(index + MIN);
}

#endif