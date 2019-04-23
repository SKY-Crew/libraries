#ifndef DIST_ORIG
#define DIST_ORIG

#include <Arduino.h>

#include "AdvMath.h"

enum Dist {
	TOO_CLOSE, CLOSE, PROPER, FAR, TOO_FAR
};

const String DIST_NAME[] = {"TOO_CLOSE", "CLOSE", "PROPER", "FAR", "TOO_FAR"};
const char DIST_CHAR[] = {'.', '-', '=', '#', char(160)};
const uint8_t DIST_TYPE = 5;

inline String str(const Dist& d) { return DIST_NAME[d]; }
inline char toChar(const Dist& d) { return DIST_CHAR[d]; }

template<typename T1, typename T2> inline Dist compare(T1 val, const T2* THRE, uint16_t TYPE, Dist MIN) {
	uint16_t index = 0;
	bool proportional = THRE[0] < THRE[TYPE - 2];
	for (; index < TYPE - 1; index ++) {
		if(proportional) {
			if(val <= THRE[index]) { break; }
		}else {
			if(val > THRE[index]) { break; }
		}
	}
	return Dist(index + MIN);
}

#endif