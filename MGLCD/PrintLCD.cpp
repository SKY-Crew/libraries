#include "LCD.h"

void LCD::run(Angle gyro, line_t line, bool canUseCam, bool canUseGyro, bool isFW, bool canUseComc, comc_t fellow,
		uint8_t QTY_LINE, uint16_t *valLine, Color *stateLine, double valIna219, cam_t goal,
		vectorRT_t ball, uint8_t QTY_IR, uint16_t *valIR, uint16_t valInAir, bool isBallInAir,
		uint16_t valCatch, bool catchingBall, uint16_t valBF, bool isBallForward, Dist distBall,
		uint16_t valFrontPSD, uint16_t valBackPSD, bool *enemyStands, Dist distGoalPSD, Dist distGoal) {
	redraw();
	countPrint ++;
	if(countPrint >= MAX_CP) {
		clear(false);
		switch (page) {
			case 0:
				drawMeter(gyro, 0, true);
				drawMeter(line.dirInside, 8, false);
				drawAngelRing(QTY_LINE, stateLine, 8);
				write(str(advRound(valIna219, 1), 1), 17, 0);
				write(canUseCam ? "cON" : "cOFF", 17, 1);
				write(canUseGyro ? "gON" : "gOFF", 17, 2);
				write(canUseComc ? "mON" : "mOFF", 17, 3);
				write(fellow.exists ? fellow.isFW ? OOO+"FW" : OOO+"GK" : XXX+XXX+XXX, 17, 4);
				write(isFW ? "FW" : "GK", 17, 5);
				break;
			case 1:
				for(uint8_t numLine = 0; numLine < QTY_LINE; numLine ++) {
					write(valLine[numLine], (numLine % 5) * 4, floor(numLine / 5.0));
				}
				write("WN Rot"+str(goal.rotOwn), 0, 4);
				write("Pos"+str(goal.diffOwn * goal.sideOwn), 10, 4);
				write("Dis"+str((uint8_t) goal.distOwn), 16, 4);
				write("OP Rot"+str(goal.rotOpp), 0, 5);
				write("Wide"+(goal.isOppWide ? OOO : XXX), 10, 5);
				write("Crn"+str(goal.isInCorner), 16, 5);
				break;
			case 2:
				drawMeter(ball.t, 0, true);
				for(uint8_t numIR = 0; numIR < QTY_IR; numIR ++) {
					write(valIR[numIR], 9 + (numIR % 3) * 4, floor(numIR / 3.0));
				}
				write("Up:"+str(valInAir), 13, 5);
				write((isBallInAir ? OOO : XXX), 19, 5);
				break;
			case 3:
				write("Catch:"+str(valCatch), 0, 0);
				write("T/F:"+(catchingBall ? OOO : XXX), 11, 0);
				write("frwdIR:"+str(valBF), 0, 1);
				write("T/F:"+(isBallForward ? OOO : XXX), 11, 1);
				write("Dis:"+str(round(ball.r)), 0, 2);
				write(str(distBall), 11, 2);
				write("Dis:", 0, 3);
				for(int8_t i = -1; i <= 1; i ++) {
					lcd->Line(30, 9 * 3 + i, 30 + ball.r * 0.15, 9 * 3 + i);
				}
				write("F_PSD:"+str(valFrontPSD), 0, 4);
				write("B_PSD:"+str(valBackPSD), 11, 4);
				write("C/F:"+(enemyStands[0] ? OOO : XXX)+" "+(enemyStands[1] ? OOO : XXX), 0, 5);
				write("Goal:"+str(toChar(distGoalPSD))+"  C&P:"+str(toChar(distGoal)), 8, 5);
				break;
		}
		MAX_PAGE = 4;
		countPrint = 0;
	}
}

void LCD::run4IR(uint8_t QTY, uint16_t *maxIR, uint16_t *minIR, double *avgIR) {
	redraw();
	countPrint ++;
	if(countPrint >= MAX_CP) {
		clear(false);
		switch (page) {
			case 0:
				write("MAX", 0, 0);
				for(int i = 0; i < QTY; i ++) {
					write(maxIR[i], (i % 4) * 5, 1 + floor(i / 4.0));
				}
				break;
			case 1:
				write("MIN", 0, 0);
				for(int i = 0; i < QTY; i ++) {
					write(minIR[i], (i % 4) * 5, 1 + floor(i / 4.0));
				}
				break;
			case 2:
				write("AVG", 0, 0);
				for(int i = 0; i < QTY; i ++) {
					write((int) avgIR[i], (i % 4) * 5, 1 + floor(i / 4.0));
				}
				break;
		}
		MAX_PAGE = 3;
		countPrint = 0;
	}
}