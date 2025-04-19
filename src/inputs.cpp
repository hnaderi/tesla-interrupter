#include "inputs.hpp"
#include <Arduino.h>

#define AC A0
#define FreqKnob A1
#define OnTimeKnob A2

#define StaccatoBTN D2
#define FastBTN D3
#define EnableBTN D4

InputReader::InputReader(){
  pinMode(StaccatoBTN, INPUT);
  pinMode(FastBTN, INPUT);
  pinMode(EnableBTN, INPUT);

  pinMode(AC, INPUT);
  pinMode(FreqKnob, INPUT);
  pinMode(OnTimeKnob, INPUT);
}

void InputReader::read(State &state) {
  state.enabled = digitalRead(EnableBTN);
  state.speed = digitalRead(FastBTN) ? Fast : Slow;
  state.mod = digitalRead(StaccatoBTN) ? Staccato : Fixed;
  state.freq = analogRead(FreqKnob);
  state.ontime = analogRead(OnTimeKnob);
  if (state.mod == Staccato)
    state.mains = analogRead(AC);
}
