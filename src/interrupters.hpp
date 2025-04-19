#ifndef INTERRUPTERS_H_
#define INTERRUPTERS_H_

#include "data.hpp"

class PWM {
  bool enabled, fast;
  unsigned int top, trig;

  void setupFast();
  void setupSlow();
  void disable();

public:
  PWM();
  void update(const State &state);
  PWMView view();
};

class Staccato {
  bool hasSeenAnything = false;
  unsigned int lastSeenVoltage, countdown = 0, skip, onTime;

public:
  Staccato();
  void update(const State &state);
  StaccatoView view();
};

#endif // TIMERS_H_
