#ifndef ADV_MATH_ORIG
#define ADV_MATH_ORIG

#include <Arduino.h>

typedef struct {
	double x;
	double y;
} vectorXY_t;

inline int8_t signum(double x) { return x > 0 ? 1 : x < 0 ? -1 : 0; }
inline double map(double x, double in_min, double in_max, double out_min, double out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
inline double absConstrain(double x, double a) { return constrain(x, - abs(a), abs(a)); }
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
	return val - absConstrain(signum(val) * minus, val);
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

inline void copyArray(uint8_t *copiedArray, uint8_t *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) { copiedArray[num] = basisArray[num]; }
}
inline void copyArray(uint16_t *copiedArray, uint16_t *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) { copiedArray[num] = basisArray[num]; }
}
inline void copyArray(int16_t *copiedArray, int16_t *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) { copiedArray[num] = basisArray[num]; }
}
inline void copyArray(double *copiedArray, double *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) { copiedArray[num] = basisArray[num]; }
}
inline void copyArray(double **copiedArray, double **basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) { copiedArray[num] = basisArray[num]; }
}

inline String str() { return ""; }
inline String str(int64_t a) { return String(a); }
inline String str(uint64_t a) { return String(a); }
inline String str(int32_t a) { return String(a); }
inline String str(uint32_t a) { return String(a); }
inline String str(int16_t a) { return String(a); }
inline String str(uint16_t a) { return String(a); }
inline String str(int8_t a) { return String(a); }
inline String str(uint8_t a) { return String(a); }
inline String str(int a) { return String(a); }
inline String str(double a) { return String(a); }
inline String str(float a) { return String(a); }
inline String str(char a) { return String(a); }
inline String str(const char* a) { return String(a); }
inline String str(bool a) { return a ? "true" : "false"; }
inline String str(double a, uint16_t index) {
	char c[16];
	dtostrf(a, -1, index, c);
	return str(c);
}

#endif