#include <Arduino.h>
#define R_DIR_PIN 5
#define R_STEP_PIN 2
#define U_DIR_PIN 6
#define U_STEP_PIN 3
#define F_DIR_PIN 7
#define F_STEP_PIN 4

#define EN 8
#define X_ENDSTOP 9
#define Y_ENDSTOP 10
#define Z_ENDSTOP 11
#define ABORT A0
#define HOLD A1
#define RESUME A2
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void serialEvent() {
    while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do somethistrng about it:
        stringComplete = true;
    }
}
void rotateDegR(float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;
    digitalWrite(R_DIR_PIN, dir);
    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * 100;
    for (int i = 0; i < steps; i++) {
        digitalWrite(R_STEP_PIN, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(R_STEP_PIN, LOW);
        delayMicroseconds(usDelay);
    }
}
void rotateDegU(float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;

    digitalWrite(U_DIR_PIN, dir);

    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * 100;

    for (int i = 0; i < steps; i++) {
        digitalWrite(U_STEP_PIN, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(U_STEP_PIN, LOW);
        delayMicroseconds(usDelay);
    }
}
void rotateDegF(float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;

    digitalWrite(F_DIR_PIN, dir);

    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * 100;

    for (int i = 0; i < steps; i++) {
        digitalWrite(F_STEP_PIN, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(F_STEP_PIN, LOW);
        delayMicroseconds(usDelay);
    }
}   
void setup() {
    Serial.begin(38400);
    inputString.reserve(200);
    pinMode(EN, OUTPUT);
    pinMode(R_DIR_PIN, OUTPUT);
    pinMode(R_STEP_PIN, OUTPUT);
    pinMode(U_DIR_PIN, OUTPUT);
    pinMode(U_STEP_PIN, OUTPUT);
    pinMode(F_DIR_PIN, OUTPUT);
    pinMode(F_STEP_PIN, OUTPUT);

    pinMode(X_ENDSTOP, INPUT_PULLUP);
    pinMode(Y_ENDSTOP, INPUT_PULLUP);
    pinMode(Z_ENDSTOP, INPUT_PULLUP);

    pinMode(ABORT, INPUT_PULLUP);
    pinMode(HOLD, INPUT_PULLUP);
    pinMode(RESUME, INPUT_PULLUP);
    digitalWrite(EN, LOW);
}

void loop() {
    if (stringComplete) {
        for (int i = 0; i < inputString.length(); i++) {
            if (inputString[i] == 'R') {
                rotateDegR(90, 1);
            }
            else if (inputString[i] == 'r') {
                rotateDegR(-90, 1);
            }
            else  if (inputString[i] == 'U') {
                rotateDegU(90, 1);
            }
            else  if (inputString[i] == 'u') {
                rotateDegU(-90, 1);
            }
            else  if (inputString[i] == 'F') {
                rotateDegF(90, 1);
            }
            else  if (inputString[i] == 'f') {
                rotateDegF(-90, 1);
            }
            delay(500);
            Serial.println('1');
            inputString = "";
            stringComplete = false;
        }
    }
}
    
int main(void)
{
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    setup();

    for (;;) {
        loop();
        if (serialEventRun) serialEventRun();
    }
    return 0;

}
