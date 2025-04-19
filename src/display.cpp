#include "display.hpp"

Display::Display() {
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;) {
      // Serial.println(F("SSD1306 allocation failed"));
      delay(1000);
    }
  }
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);
  display.setTextSize(2);
  display.println("Tesla Interrupter I");
  display.display();
}

void Display::render(const State &state) {}
