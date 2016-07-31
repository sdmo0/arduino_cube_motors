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
                rotateDeg(B_DIR_PIN, B_STEP_PIN, 90, 1);
            }
            else if (inputString[i] == 'b') {
                rotateDeg(B_DIR_PIN, B_STEP_PIN, -90, 1);
            }
            else  if (inputString[i] == 'D') {
                rotateDeg(D_DIR_PIN, D_STEP_PIN, 90, 1);
            }
            else  if (inputString[i] == 'd') {
                rotateDeg(D_DIR_PIN, D_STEP_PIN, -90, 1);
            }
            else  if (inputString[i] == 'L') {
                rotateDeg(L_DIR_PIN, L_STEP_PIN, 90, 1);
            }
            else  if (inputString[i] == 'l') {
                rotateDeg(L_DIR_PIN, L_STEP_PIN, -90, 1);
            }
            delay(500);
            Serial.println(inputString[i]);
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
