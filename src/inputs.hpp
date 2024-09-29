#ifndef INPUTS_H_
#define INPUTS_H_

#include <Arduino.h>

class AnalogInput {
  const unsigned int pin;
  const unsigned long _min, _max, minWait;
  unsigned int value;
  unsigned long _lastChange;

public:
  AnalogInput(const unsigned int pin, const unsigned long min = 0,
              const unsigned long max = 100, const unsigned long minWait = 2e5)
      : pin(pin), _min(min), _max(max), minWait(minWait) {
    pinMode(pin, INPUT);
  }

  unsigned long lastChanged() { return _lastChange; }

  unsigned int readValue(const unsigned long now) {
    if (now - _lastChange > minWait) {
      const auto read = map(analogRead(pin), 0, 1023, _min, _max);
      if (read != value) {
        _lastChange = now;
        value = read;
      }
    }
    return value;
  }
};

class SwitchButton {
  const unsigned int pin;
  unsigned long lastRead;
  bool value;

public:
  SwitchButton(const unsigned int pin) : pin(pin) { pinMode(pin, INPUT); }

  bool read(const unsigned long now) {
    if (now - lastRead > 2e5 || lastRead == 0) {
      value = digitalRead(pin) == HIGH;
      lastRead = now;
    }
    return value;
  }
};

#endif // INPUTS_H_
