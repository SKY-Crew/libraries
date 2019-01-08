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

int8_t signum(double);

double absConstrain(double, double);
double toDegrees(double);
double toRadians(double);

double advRound(double, int);

double simplifyDeg(double);

uint8_t createBinOnes(uint8_t);
uint8_t extractBit(uint8_t, uint8_t firstDigi, uint8_t);

void copyArray(uint8_t*, uint8_t*, uint8_t);
void copyArray(uint16_t*, uint16_t*, uint8_t);
void copyArray(double*, double*, uint8_t);
void copyArray(double*, double*, uint8_t, uint8_t);
////
bool insideAngle(double, double, double);
#endif