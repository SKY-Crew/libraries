#ifndef ADV_MATH_ORIG
#define ADV_MATH_ORIG

#include "Arduino.h"

typedef struct {
	double x;
	double y;
} vectorXY_t;

typedef struct {
	double r;
	double t;
} vectorRT_t;

int8_t signum(double x);
double absConstrain(double x, double a);
double toDegrees(double theta);
double toRadians(double deg);
double simplifyDeg(double deg);
bool insideAngle(double deg, double min, double max);
uint8_t extractBit(uint8_t byte, uint8_t firstDigit, uint8_t lastDigit);
uint8_t createBinOnes(uint8_t length);
void copyArray(uint8_t *copiedArray, uint8_t *basisArray, uint8_t size);
void copyArray(uint16_t *copiedArray, uint16_t *basisArray, uint8_t size);
void copyArray(double *copiedArray, double *basisArray, uint8_t size);
void copyArray(double *copiedArray, double *basisArray, uint8_t size, uint8_t size2);

#endif