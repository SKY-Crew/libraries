#ifndef ADV_MATH_ORIG
#define ADV_MATH_ORIG

#include <Arduino.h>

typedef struct {
	double x;
	double y;
} vectorXY_t;

enum Side {
	LEFT = -1, CENTER, RIGHT
};
enum Diff {
	NONE, SMALL, LARGE, TOO_LARGE, SIZE_DIFF
};

inline int8_t signum(double x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }
#ifdef CORE_TEENSY
inline double map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif
template<typename T> inline T sum(T *array, uint16_t size) {
	T sum = 0;
	for(uint8_t i = 0; i < size; i ++) {
		sum += array[i];
	}
	return sum;
}
template<typename T> inline double mean(T *array, uint16_t size) {
	return double(sum(array, size)) / size;
}

inline double absCon(double x, double a) { return constrain(x, - abs(a), abs(a)); }
inline double conMap(double x, double from_min, double from_max, double to_min, double to_max, double min = -10000, double max = -10000) {
	return constrain(map(x, from_min, from_max, to_min, to_max), min == -10000 ? to_min : min, max == -10000 ? to_max : max);
}
inline double toDegrees(double theta) { return theta * 180.0 / M_PI; }
inline double toRadians(double deg) { return deg * M_PI / 180.0; }

inline double advRound(double val, uint16_t index) {
	double pow10 = pow(10, index);
	return round(val * pow10) / pow10;
}

inline double simplifyDeg(double deg) {
	return fmod(fmod(deg, 360) + 360 + 180, 360) - 180;
}

inline double absMinus(double val, double minus) {
	return val - absCon(signum(val) * minus, val);
}

inline double filter(double val, double prv, double CHANGE_RATE) {
	return map(CHANGE_RATE, 0, 1, prv, val);
}

inline double polyLine(double val, double *X, double* Y, uint8_t SIZE) {
	double ans = 0;
	uint8_t index = 0;
	for(; index < SIZE; index ++) {
		if(X[index] > abs(val)) { break; }
	}
	ans = index == 0 ? Y[index]
			: index == SIZE ? Y[index - 1]
			: map(abs(val), X[index - 1], X[index], Y[index - 1], Y[index]);
	ans *= signum(val);
	return ans;
}

inline uint8_t createBinOnes(uint8_t length) { return (1 << length) - 1; }
inline uint8_t extractBit(uint8_t byte, uint8_t firstDigit, uint8_t lastDigit) {
	return (byte & createBinOnes(lastDigit + 1)) >> firstDigit;
}

template<typename T> inline void copyArray(T *copiedArray, T *basisArray, uint8_t SIZE) {
	memcpy(copiedArray, basisArray, sizeof(T) * SIZE);
}
template<typename T> inline void copyArray(T **copiedArray, T **basisArray, uint8_t SIZE) {
	for(int num = 0; num < SIZE; num ++) { copiedArray[num] = basisArray[num]; }
}

inline String str() { return ""; }
template<typename T> inline String str(T a) { return String(a); }
inline String str(const char* a) { return String(a); }
inline String str(bool a) { return a ? "#" : "."; }
inline String str(double a, uint16_t index) {
	char c[16];
	dtostrf(a, -1, index, c);
	return str(c);
}
template<typename T> inline String str(T *a, uint8_t SIZE) {
	String s = "[";
	for(uint8_t i = 0; i < SIZE; i ++) { s += str(a[i])+" "; }
	return s+"]";
}

#endif