#ifndef UNARY_FUNCTIONS_H
#define UNARY_FUNCTIONS_H


#include <Arduino.h>


float incrementBetter(int type, float value);
float decrementBetter(int type, float value);
float unaryminus(int type, float value);
float logicalNot(int type, float value);
float bitwiseNot(int type, float value);
float ToChar(int type, float value);
float ToInt(int type, float value);
float ToFloat(int type, float value);
float Round(int type, float value);
float Floor(int type, float value);
float Ceil(int type, float value);
float Abs(int type, float value);
float Sq(int type, float value);
float Sqrt(int type, float value);
float AnalogRead(int type, float value);
float DigitalRead(int type, float value);

#endif // UNARY_FUNCTIONS_H