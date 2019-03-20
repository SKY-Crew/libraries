#include "INA219.h"

INA219::INA219(uint8_t X, double LOW_VOLT, double HIGH_VOLT, uint8_t MAX_CR, uint8_t MAX_CVL) {
	//copy
	this->LOW_VOLT = LOW_VOLT;
	this->HIGH_VOLT = HIGH_VOLT;
	this->MAX_CR = MAX_CR;
	this->MAX_CVL = MAX_CVL;

	//init
	ina219.begin(X, 0x41);
}

bool INA219::checkVolt() {
	countRead ++;
	if(countRead >= MAX_CR) {
		value = ina219.getBusVoltage_V();
		if(countVoltLow >= MAX_CVL) {
			countVoltLow = value > HIGH_VOLT ? 0 : MAX_CVL;
			return true;
		}else {
			countVoltLow = value < LOW_VOLT ? (countVoltLow + 1) : 0;
		}
	countRead = 0;
	}
	return false;
}

double INA219::getValue() {
	return value;
}