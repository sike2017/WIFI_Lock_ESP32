#include "motor.h"
int PIN_ARRAY[PIN_NUMBER] = {PIN_IN1, PIN_IN2, PIN_IN3, PIN_IN4};    // stepper motor driver board IN1 IN2 IN3 IN4
void clockwise(int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < PIN_NUMBER; j++) {
      digitalWrite(PIN_ARRAY[j], HIGH);
      delay(3);
      digitalWrite(PIN_ARRAY[j], LOW);
    }
  }
}

void counterClockwise(int n) {
  for (int i = 0; i < n; i++) {
    for (int j = PIN_NUMBER; j >= 0; j--) {
      digitalWrite(PIN_ARRAY[j], HIGH);
      delay(3);
      digitalWrite(PIN_ARRAY[j], LOW);
    }
  }
}