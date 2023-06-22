#ifndef BINARY_FUNCTIONS_H
#define BINARY_FUNCTIONS_H

#include <Arduino.h>


float plus(float x, float y);
float minus(float x, float y);
float times(float x, float y);
float dividedBy(float x, float y);
float modulus(float x, float y);
float equals(float x, float y);
float notEquals(float x, float y);
float lessThan(float x, float y);
float lessThanOrEquals(float x, float y);
float greaterThan(float x, float y);
float greaterThanOrEquals(float x, float y);
float minBetter(float x, float y);
float maxBetter(float x, float y);
float pow(float x, float y);
float logicalAnd(float x, float y);
float logicalOR(float x, float y);
float logicalXOR(float x, float y);
float bitwiseAND(float x, float y);
float bitwiseOR(float x, float y);
float bitwiseXOR(float x, float y);


#endif // BINARY_FUNCTIONS_H