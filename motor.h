#pragma once
#include <Arduino.h>

// steps for one roll of the stepper motor
#define ROL_STEPS 512
#define PIN_NUMBER 4
#define PIN_IN1 14
#define PIN_IN2 27
#define PIN_IN3 26
#define PIN_IN4 25
extern int PIN_ARRAY[PIN_NUMBER];

void clockwise(int n);
void counterClockwise(int n);