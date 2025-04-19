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
  void update(const State &state);
  PWMView view();
};

class Staccato {
public:
  void update(const State &state);
  StaccatoView view();
};

#endif // TIMERS_H_
