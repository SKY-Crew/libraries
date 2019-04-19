#ifndef DEBUG_ORIG
#define DEBUG_ORIG

#include <Arduino.h>

#define trace(xx) if(trace_break(xx))
// #define trace(xx) if(false)

extern int trace_data;

uint8_t trace_cmdset(uint8_t);
void trace_cmdloop(uint8_t);
int trace_break(int);

#endif