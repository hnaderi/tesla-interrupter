#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "data.hpp"
#include <Adafruit_SSD1306.h>

class Display {
  Adafruit_SSD1306 display;
  void printEnabled(const State &state);
  unsigned long lastRender;

public:
  Display();
  void render(const State &state, PWMView view);
  void render(const State &state, StaccatoView view);
};

#endif // DISPLAY_H_
