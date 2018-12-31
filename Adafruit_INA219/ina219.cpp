#include "INA219.h"

INA219::INA219(uint8_t get_X, double get_LOW_VOLT, double get_HIGH_VOLT, uint8_t get_MAX_CR, uint8_t get_MAX_CVL) {
	//copy
	LOW_VOLT = get_LOW_VOLT;
	HIGH_VOLT = get_HIGH_VOLT;
	MAX_CR = get_MAX_CR;
	MAX_CVL = get_MAX_CVL;

	//init
	ina219.begin(get_X, 0x41);
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