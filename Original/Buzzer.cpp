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

void Buzzer::set(uint8_t INDEX_NOTE, uint16_t period, bool startbeep, uint16_t rankBeep) {
	if(!startbeep || this->rankBeep > rankBeep) { return; }
	if(INDEX_NOTE >= 0 && INDEX_NOTE < QTY_NOTE) { crtNote = NOTE[INDEX_NOTE]; }
	if(period > 0) { this->period = period; }
	this->rankBeep = rankBeep;
}

void Buzzer::beep() {
	if(crtNote != prvNote) { analogWriteFrequency(PORT, crtNote); }
	prvNote = crtNote;
	if(rankBeep > 0) { timeStart = millis(); }
	if(!haveBeep) { analogWrite(PORT, millis() - timeStart < period ? LOUDNESS : 0); }
	haveBeep = true;
}

void Buzzer::reset() {
	haveBeep = false;
	rankBeep = 0;
}

uint8_t Buzzer::noteMap(double val, double from_min, double from_max) {
	return conMap(val, from_min, from_max, 0, QTY_NOTE - 1);
}