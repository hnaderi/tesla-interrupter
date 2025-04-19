#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "data.hpp"
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

class Display {
  Adafruit_SSD1306 display;

public:
  Display();

  void render(const State &state);
};

#endif // DISPLAY_H_
