#include "display.hpp"
#include "logo.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
#define SCREEN_UPDATE 100

Display::Display() {
  delay(500);
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;) {
      delay(1000);
    }
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);

  display.drawBitmap(0, 0, logo, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.setTextSize(1);
  display.display();
}

void Display::printEnabled(const State &state) {
  if (state.enabled)
    display.print("[On] ");
  else
    display.print("[Off] ");
}

void Display::render(const State &state, PWMView view) {
  unsigned long now = millis();
  if (lastRender - now < SCREEN_UPDATE)
    return;
  else
    lastRender = now;
  display.setTextColor(WHITE, BLACK);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Frequency: ");
  display.print(view.freq);
  display.println("Hz");

  display.print("On time:   ");
  display.print(view.duty);
  display.println("uS");
  display.println();
  display.setTextColor(BLACK, WHITE);

  display.print("[PWM] ");
  printEnabled(state);
  if (state.speed == Fast)
    display.print("[Fast] ");
  else
    display.print("[Slow] ");

  display.display();
}

void Display::render(const State &state, StaccatoView view) {
  unsigned long now = millis();
  if (lastRender - now < SCREEN_UPDATE)
    return;
  else
    lastRender = now;
  display.setTextColor(WHITE, BLACK);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Skip: ");
  display.println(view.skip);

  display.print("On time:     ");
  display.print(view.ontime);
  display.println("uS");
  display.println();
  display.setTextColor(BLACK, WHITE);

  display.print("[Staccato] ");
  printEnabled(state);

  display.display();
}
