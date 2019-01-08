#include "AdvMath.h"

int8_t signum(double x) {
	return x > 0 ? 1
		: x < 0 ? -1
		: 0;
}

double absConstrain(double x, double a) { return constrain(x, -abs(a), abs(a)); }
double toDegrees(double theta) { return theta * 180.0 / M_PI; }
double toRadians(double deg) { return deg * M_PI / 180.0; }

double advRound(double val, int index) {
	double pow10 = pow(10, index);
	return round(val * pow10) / pow10;
}

double simplifyDeg(double deg) {
	return fmod(fmod(deg, 360) + 360 + 180, 360) - 180;
}

uint8_t createBinOnes(uint8_t length) { return (1 << length) - 1; }
uint8_t extractBit(uint8_t byte, uint8_t firstDigit, uint8_t lastDigit) {
	return (byte & createBinOnes(lastDigit + 1)) >> firstDigit;
}

void copyArray(uint8_t *copiedArray, uint8_t *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) {
		copiedArray[num] = basisArray[num];
	}
}
void copyArray(uint16_t *copiedArray, uint16_t *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) {
		copiedArray[num] = basisArray[num];
	}
}
void copyArray(double *copiedArray, double *basisArray, uint8_t size) {
	for(int num = 0; num < size; num ++) {
		copiedArray[num] = basisArray[num];
	}
}
void copyArray(double *copiedArray, double *basisArray, uint8_t size, uint8_t size2) {
	for(int num = 0; num < size; num ++) {
		for(int num2 = 0; num2 < size2; num2 ++) {
			*(copiedArray + num * size2 + num2) = *(basisArray + num * size2 + num2);
		}
	}
}
////
bool insideAngle(double angle, double min, double max) {
	return simplifyDeg(angle - min) <= simplifyDeg(max - min);
}