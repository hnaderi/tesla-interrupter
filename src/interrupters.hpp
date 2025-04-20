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
  bool active = false;
  unsigned int countdown = 0, skip, onTime;
  inline void trigger();

public:
  Staccato();
  void update(const State &state);
  StaccatoView view();
};

#endif // TIMERS_H_
