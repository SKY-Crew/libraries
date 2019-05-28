#include "Buzzer.h"

Buzzer::Buzzer(uint8_t PORT) {
	// copy
	this->PORT = PORT;

	// init
	D_NOTE = new double[QTY_NOTE];
	NOTE = new uint16_t[QTY_NOTE];

	D_NOTE[INDEX_A] = NOTE_A;
	for(int8_t i = INDEX_A - 1; i >= 0; i --) {
		D_NOTE[i] = D_NOTE[i + 1] / R;
	}
	for(uint8_t i = INDEX_A + 1; i < QTY_NOTE; i ++) {
		D_NOTE[i] = D_NOTE[i - 1] * R;
	}
	for(uint8_t i = 0; i < QTY_NOTE; i ++) {
		NOTE[i] = round(D_NOTE[i]);
	}
}

void Buzzer::set(uint8_t INDEX_NOTE, uint16_t period = 0) {
	if(INDEX_NOTE >= 0 && INDEX_NOTE < QTY_NOTE) { analogWriteFrequency(PORT, NOTE[INDEX_NOTE]); }
	if(period > 0) { this->period = period; }
	beep(true);
}

void Buzzer::beep(bool startBeep = false) {
	if(startBeep) { timeStart = millis(); }
	analogWrite(PORT, millis() - timeStart < period ? LOUDNESS : 0);
}