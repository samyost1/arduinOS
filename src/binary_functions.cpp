#include <binary_functions.hpp>

float plus(float x, float y) { return x + y; }
float minus(float x, float y) { return x - y; }
float times(float x, float y) { return x * y; }
float dividedBy(float x, float y) { return x / y; }
float modulus(float x, float y) { return (int)x % (int)y; }
float equals(float x, float y) { return x == y ? '1' : '0'; }
float notEquals(float x, float y) { return x != y ? '1' : '0'; }
float lessThan(float x, float y) { return x < y ? '1' : '0'; }
float lessThanOrEquals(float x, float y) { return x <= y ? '1' : '0'; }
float greaterThan(float x, float y) { return x > y ? '1' : '0'; }
float greaterThanOrEquals(float x, float y) { return x >= y ? '1' : '0'; }
float minBetter(float x, float y) { return min(x, y); }
float maxBetter(float x, float y) { return max(x, y); }
float pow(float x, float y) { return x * y; }
float logicalAnd(float x, float y) { return x && y ? '1' : '0'; }
float logicalOR(float x, float y) { return x || y ? '1' : '0'; }
float logicalXOR(float x, float y) { return (x && !y) || (!x && y) ? '1' : '0'; }
float bitwiseAND(float x, float y) { return (int)x & (int)y; }
float bitwiseOR(float x, float y) { return (int)x | (int)y; }
float bitwiseXOR(float x, float y) { return (int)x ^ (int)y; }
