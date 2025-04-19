#include <Arduino.h>
#include "inputs.hpp"
#include "display.hpp"
#include "interrupters.hpp"

void setup() {
  InputReader inputs;
  Display display;
  State state;
  PWM pwm;
  class Staccato staccato;
  delay(2000);

  while (true) {
    inputs.read(state);
    pwm.update(state);
    staccato.update(state);

    if(state.mod== Mod::Fixed)
      display.render(state, pwm.view());
    else
      display.render(state, staccato.view());
  }
}

void loop() {}
