#include "interrupters.hpp"
#include <Arduino.h>

#define TRIGGER_VOLTAGE 30
#define MIN_ONTIME 0
#define MAX_ONTIME 10000
#define MIN_SKIP 0
#define MAX_SKIP 500

Staccato::Staccato() {
  DDRB = 1 << DDB2;
  PORTB &= ~(1 << 2);
}

void Staccato::update(const State &state) {
  bool isEnabled = state.enabled && state.mod == Mod::Staccato;
  if (!isEnabled) {
    hasSeenAnything = false;
    return;
  }

  onTime = map(state.ontime, 0, 1023, MIN_ONTIME, MAX_ONTIME);
  skip = map(state.freq, 0, 1023, MIN_SKIP, MAX_SKIP);
  unsigned int mains = map(state.mains, 0, 1023, 0, 220);

  if (hasSeenAnything) {
    if (mains > lastSeenVoltage && mains > TRIGGER_VOLTAGE) {
      if (countdown == 0) {
        PORTB |= (1 << 2); // Set high
        delayMicroseconds(onTime);
        PORTB &= ~(1 << 2); // Set low
        countdown = skip;
      } else
        countdown--;
    }
  } else {
    hasSeenAnything = true;
    lastSeenVoltage = mains;
  }
}

StaccatoView Staccato::view() { return {.skip = skip, .ontime = onTime}; }
