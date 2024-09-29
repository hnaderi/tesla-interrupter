#include "SignalOutput.hpp"
#include "inputs.hpp"
#include <Arduino.h>

#define PIN_BTN1 2
#define PIN_BTN2 3
#define PIN_LED 4
#define PIN_OUT 9
#define PIN_MOD A3

void setup() {
  AnalogInput ac(A0, 0, 100, 1e3), interval(A1, 1e4, 1e6), length(A2, 0, 4000);
  SwitchButton mod(PIN_MOD);
  SignalOutput interrupter(PIN_OUT, PIN_LED);

  State state;
  while (unsigned long now = micros()) {
    state.now = now;
    state.ac = ac.readValue(now);
    state.interval = interval.readValue(now);
    state.length = length.readValue(now);
    state.fixed = mod.read(now);

    if (interrupter.tick(state)) {
      state.signal = !state.signal;
      state.lastEdge = now;
    }
  }
}

void loop() {}
