#include <unary_functions.hpp>

float incrementBetter(int type, float value) {
    // Pushes same type
    return value + 1;
}
float decrementBetter(int type, float value) {
    // Pushes same type
    return value - 1;
}
float unaryminus(int type, float value) {
    // Pushes same type
    return -value;
}
float logicalNot(int type, float value) {
    // Pushes type CHAR
    return !value;
}
float bitwiseNot(int type, float value) {
    // Pushes same type
    return ~(int)value;  // TODO
}
float ToChar(int type, float value) {
    // Pushes type CHAR
    return (char)value;
}
float ToInt(int type, float value) {
    // Pushes type INT
    return (int)value;
}
float ToFloat(int type, float value) {
    // Pushes type FLOAT
    return (float)value;
}
float Round(int type, float value) {
    // Pushes type INT
    return round(value);
}
float Floor(int type, float value) {
    // Pushes type INT
    return floor(value);
}
float Ceil(int type, float value) {
    // Pushes type INT
    return ceil(value);
}
float Abs(int type, float value) {
    // Pushes same type
    return abs(value);
}
float Sq(int type, float value) {
    // Pushes same type
    return value * value;
}
float Sqrt(int type, float value) {
    // Pushes same type
    return sqrt(value);
}
float AnalogRead(int type, float value) {
    // Pushes type INT
    int pin = (int)value;
    int rawValue = analogRead(pin);
    return (float)rawValue;

}
float DigitalRead(int type, float value) {
    // Pushes type CHAR
    int pin = (int)value;
    int rawValue = digitalRead(pin);
    return (float)rawValue;
}