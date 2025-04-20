#include "interrupters.hpp"
#include <Arduino.h>

#define AC A0
#define ACTIVATE_VOLTAGE 0
#define TRIGGER_VOLTAGE 15
#define MAINS_VOLTAGE 300
#define MIN_ONTIME 0
#define MAX_ONTIME 5000
#define MIN_SKIP 0
#define MAX_SKIP 200

Staccato::Staccato() {
  pinMode(AC, INPUT);
  DDRB = 1 << DDB2;
  PORTB &= ~(1 << 2);
}

inline void Staccato::trigger() {
  active = false;
  if (onTime == 0)
    return;
  if (countdown == 0) {
    PORTB |= (1 << 2);
    delayMicroseconds(onTime);
    PORTB &= ~(1 << 2);
    countdown = skip;
  } else
    countdown--;
}

inline unsigned int readMains() {
  return map(analogRead(AC), 0, 1023, 0, MAINS_VOLTAGE);
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

  const auto start = micros();
  while (micros() - start < 50000) {
    unsigned int mains = readMains();
    if (active && mains > TRIGGER_VOLTAGE) {
      trigger();
      break;
    } else if (mains < TRIGGER_VOLTAGE && mains >= ACTIVATE_VOLTAGE)
      active = true;
  }
}

StaccatoView Staccato::view() { return {.skip = skip, .ontime = onTime}; }
