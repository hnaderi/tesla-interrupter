#include "display.hpp"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

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
  display.println("Interrupter");
  display.setTextSize(1);
  display.println("by hnaderi");
  display.display();
}

void Display::printEnabled(const State &state){
  if (state.enabled)
    display.print("[On] ");
  else
    display.print("[Off] ");
}

void Display::render(const State &state, PWMView view) {
  display.setTextColor(WHITE, BLACK);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Frequency: ");
  display.print(view.freq);
  display.println("Hz");

  display.print("Duty:     ");
  display.print(view.duty);
  display.println("%");
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
