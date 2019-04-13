#define trace(xx) if(trace_break(xx))
//#define trace(xx) if(false)

#define trace_MAX 32
uint8_t trace_setting[trace_MAX] = {0};
char trace_cmdbf[16];
int trace_data;
uint8_t trace_brkall=0, trace_cmdbfp = 0;

inline uint8_t trace_cmdset(uint8_t dmd){
	uint8_t i, kd, cmd, bno;
	// int ndt;
	while(Serial.available()) {
		kd = Serial.read();
		if (kd>=' ' && kd != '.') trace_cmdbf[trace_cmdbfp++] = kd;
		else {
			if(kd==0x08 && trace_cmdbfp!=0) trace_cmdbfp--;
			if(kd=='.'){
				trace_cmdbf[trace_cmdbfp] = 0;
				Serial.println(trace_cmdbf);
				cmd = trace_cmdbf[0]; bno = atoi(&trace_cmdbf[1]); 
				if(cmd=='t') {// Trace
					if(bno) trace_setting[bno] |= 0x01; 
					else		trace_brkall ^=0x01;
				}
				if(cmd=='b') {// Break
					if(bno) trace_setting[bno] |= 0x02; 
					else		trace_brkall ^=0x02;
				}
				if(cmd=='e') {// Exec 
					if(bno) trace_setting[bno] |= 0x04; 
					else		trace_brkall ^=0x01;
				}
				if(cmd=='l') {// Trace
					Serial.print("Set [ ");
					for( i=1; i<trace_MAX; i++) 
						if( trace_setting[i] != 0) {Serial.print(i);Serial.print(":");Serial.print(trace_setting[i]);Serial.print(" ");}
					Serial.println(" ]");
				}
				if(cmd=='c') {// Clear
					if(bno)trace_setting[bno] = 0x00;
					else {
						for( i=1; i<trace_MAX; i++) trace_setting[i] = 0;
						trace_brkall = 0;
					}
				}
				if(cmd=='w') { // Data
					i = 0; while(trace_cmdbf[i++]!= ' ') ; // ' 'を探す
					trace_data = atoi(&trace_cmdbf[i]); // 設定値
					trace_setting[bno] |= 0x08; // Set oneshot
				}
				if(cmd=='g') { // Go
					trace_brkall = 0;
					dmd = 0; Serial.println("Go ");
				}
				if(cmd=='s') { // Step
					trace_brkall = 0x02;
					dmd = 0; Serial.print("Next");
				}
				trace_cmdbfp = 0;
			}
		}
	}
	return dmd;
}
inline void trace_cmdloop(uint8_t dmd){
	uint8_t i;
	if(trace_setting[0]==0){ // 初期化
	Serial.begin(9600); delay(1000); Serial.println("");
		Serial.println("<Arduino MicroDebuger>");
		Serial.println(" txx trace");
		Serial.println(" bxx break");
		Serial.println(" exx exec");
		Serial.println(" cxx clear");
		Serial.println(" wxx yyy write");
		Serial.println(" g go");
		Serial.println(" s step");
		Serial.println(" l list bp");
		for( i=0; i<trace_MAX; i++) trace_setting[i] = 0; trace_setting[0] = 0xff;
	} else{ // ユーザ入力確認 
			do{
				if(Serial.available()) dmd = trace_cmdset(dmd);
			} while(dmd);
	}
}
uint8_t trace_tcnt = 0;
inline int trace_break(int dno){
	uint8_t bkf;
	bkf = trace_brkall | trace_setting[dno];
	if(bkf==0) return 0; // 何もしない
	if((bkf & 0x01)){ // Trace
		Serial.print("[");Serial.print(dno);Serial.print("]");
		if(trace_tcnt++ >10) {Serial.println(""); trace_tcnt = 0;}
	}
	if((bkf & 0x02)){ // Break
		Serial.print("\nBreak at ");Serial.println(dno);
		trace_cmdloop(1);
	}
	trace_setting[dno] &= 0x07;
	return (bkf & 0x0c);
}