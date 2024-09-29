#ifndef SIGNALOUTPUT_H_
#define SIGNALOUTPUT_H_

#define MAX_ON_TIME 4000

#include "data.hpp"
#include <Arduino.h>

class SignalOutput {
  const unsigned int pin, led;

  void write(bool output) {
    digitalWrite(pin, output);
    digitalWrite(led, output);
  }

  bool exceeded(const State state) {
    return state.signal && (state.now - state.lastEdge) >= MAX_ON_TIME;
  }

  bool staccato(const State state) {
    return false;//(state.signal && state.ac >= 98) || (!state.signal && state.ac < 2);
  }

  bool fixed(const State state) {
    const auto elapsed = state.now - state.lastEdge;
    return (state.signal && elapsed > state.length) ||
           (!state.signal && elapsed > state.interval);
  }

public:
  SignalOutput(unsigned int pin, unsigned int led) : pin(pin), led(led) {
    pinMode(led, OUTPUT);
    pinMode(pin, OUTPUT);
    digitalWrite(led, LOW);
    digitalWrite(pin, LOW);
  }

  bool tick(const State state) {
    const auto changed = state.fixed ? fixed(state) : staccato(state);
    if (changed || exceeded(state)) {
      write(!state.signal);
      return true;
    }
    return false;
  }
};

#endif // SIGNALOUTPUT_H_
