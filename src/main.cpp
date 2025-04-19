#include "display.hpp"
#include "interrupters.hpp"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define AC A0
#define FreqKnob A1
#define OnTimeKnob A2

#define StaccatoBTN D2
#define FastBTN D3
#define EnableBTN D4

void read(State &state) {
  state.enabled = digitalRead(EnableBTN);
  state.speed = digitalRead(FastBTN) ? Fast : Slow;
  state.mod = digitalRead(StaccatoBTN) ? Staccato : Fixed;
  state.freq = analogRead(FreqKnob);
  state.ontime = analogRead(OnTimeKnob);
}

void setup() {
  pinMode(StaccatoBTN, INPUT);
  pinMode(FastBTN, INPUT);
  pinMode(EnableBTN, INPUT);

  pinMode(AC, INPUT);
  pinMode(FreqKnob, INPUT);
  pinMode(OnTimeKnob, INPUT);

  Display display;
  State state;
  PWM pwm;
  delay(2000);

  while (true) {
    read(state);
    pwm.update(state);
  }
}

void loop() {}
