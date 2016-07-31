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

#define SPEED_FACTOR 120

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
void rotateDeg(int dir_pin, int step_pin, float deg, float speed) {
    int dir = (deg > 0) ? HIGH : LOW;
    digitalWrite(dir_pin, dir);
    int steps = abs(deg) * (1 / 0.225);
    int usDelay = (1 / speed) * SPEED_FACTOR;
    for (int i = 0; i < steps; i++) {
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(step_pin, LOW);
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
                rotateDeg(R_DIR_PIN, R_STEP_PIN, 90, 1);
            }
            else if (inputString[i] == 'r') {
                rotateDeg(R_DIR_PIN, R_STEP_PIN, -90, 1);
            }
            else  if (inputString[i] == 'U') {
                rotateDeg(U_DIR_PIN, U_STEP_PIN, 90, 1);
            }
            else  if (inputString[i] == 'u') {
                rotateDeg(U_DIR_PIN, U_STEP_PIN, -90, 1);
            }
            else  if (inputString[i] == 'F') {
                rotateDeg(F_DIR_PIN, F_STEP_PIN, 90, 1);
            }
            else  if (inputString[i] == 'f') {
                rotateDeg(F_DIR_PIN, F_STEP_PIN, -90, 1);
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
