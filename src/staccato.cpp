#include "interrupters.hpp"
#include <Arduino.h>

#define ACTIVATE_VOLTAGE 10
#define TRIGGER_VOLTAGE 30
#define MAINS_VOLTAGE 300
#define MIN_ONTIME 0
#define MAX_ONTIME 5000
#define MIN_SKIP 0
#define MAX_SKIP 200

Staccato::Staccato() {
  DDRB = 1 << DDB2;
  PORTB &= ~(1 << 2);
}

inline void Staccato::trigger() {
  active = false;
  if (countdown == 0) {
    PORTB |= (1 << 2); // Set high
    delayMicroseconds(onTime);
    PORTB &= ~(1 << 2); // Set low
    countdown = skip;
  } else
    countdown--;
}

void Staccato::update(const State &state) {
  bool isSelected = state.mod == Mod::Staccato;
  if (isSelected) {
    onTime = map(state.ontime, 0, 1023, MIN_ONTIME, MAX_ONTIME);
    skip = map(state.freq, 0, 1023, MIN_SKIP, MAX_SKIP);
  }
  if (!(state.enabled && isSelected)) {
    active = false;
    return;
  }

  unsigned int mains = map(state.mains, 0, 1023, 0, MAINS_VOLTAGE);

  if (active && mains > TRIGGER_VOLTAGE)
    trigger();
  else if (mains < TRIGGER_VOLTAGE && mains > ACTIVATE_VOLTAGE)
    active = true;
}

StaccatoView Staccato::view() { return {.skip = skip, .ontime = onTime}; }
