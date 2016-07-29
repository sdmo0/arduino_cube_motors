#include <Arduino.h>
#define B_DIR_PIN 5
#define B_STEP_PIN 2
#define D_DIR_PIN 6
#define D_STEP_PIN 3
#define L_DIR_PIN 7
#define L_STEP_PIN 4

#define EN 8
#define X_ENDSTOP 9
#define Y_ENDSTOP 10
#define Z_ENDSTOP 11
#define ABORT A0
#define HOLD A1
#define RESUME A2
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
void rotateDegB(float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;
    digitalWrite(B_DIR_PIN, dir);
    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * 100;
    for (int i = 0; i < steps; i++) {
        digitalWrite(B_STEP_PIN, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(B_STEP_PIN, LOW);
        delayMicroseconds(usDelay);
    }
}
void rotateDegD(float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;

    digitalWrite(D_DIR_PIN, dir);

    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * 100;

    for (int i = 0; i < steps; i++) {
        digitalWrite(D_STEP_PIN, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(D_STEP_PIN, LOW);
        delayMicroseconds(usDelay);
    }
}
void rotateDegL(float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;

    digitalWrite(L_DIR_PIN, dir);

    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * 100;

    for (int i = 0; i < steps; i++) {
        digitalWrite(L_STEP_PIN, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(L_STEP_PIN, LOW);
        delayMicroseconds(usDelay);
    }
}
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
void setup() {
    Serial.begin(38400);
    inputString.reserve(200);
    pinMode(EN, OUTPUT);
    pinMode(B_DIR_PIN, OUTPUT);
    pinMode(B_STEP_PIN, OUTPUT);
    pinMode(D_DIR_PIN, OUTPUT);
    pinMode(D_STEP_PIN, OUTPUT);
    pinMode(L_DIR_PIN, OUTPUT);
    pinMode(L_STEP_PIN, OUTPUT);

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
            if (inputString[i] == 'B') {
                rotateDegB(90, 1);
            }
            else if (inputString[i] == 'b') {
                rotateDegB(-90, 1);
            }
            else  if (inputString[i] == 'D') {
                rotateDegD(90, 1);
            }
            else  if (inputString[i] == 'd') {
                rotateDegD(-90, 1);
            }
            else  if (inputString[i] == 'L') {
                rotateDegL(90, 1);
            }
            else  if (inputString[i] == 'l') {
                rotateDegL(-90, 1);
            }
            delay(500);
            Serial.println('2');
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
