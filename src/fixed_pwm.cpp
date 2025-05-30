#include "interrupters.hpp"
#include <Arduino.h>

#define MIN_TOP 2000
#define MAX_TOP __UINT16_MAX__
#define MAX_ONTIME_MICROS 120

static const float BASE_SLOW = F_CPU / 256. / 2.;
static const float BASE_FAST = F_CPU / 8. / 2.;

static const float TICK_SLOW = 1.e6 / BASE_SLOW;
static const float TICK_FAST = 1.e6 / BASE_FAST;
static const float MAX_ON_TIME_SLOW = MAX_ONTIME_MICROS / TICK_SLOW;
static const float MAX_ON_TIME_FAST = MAX_ONTIME_MICROS / TICK_FAST;

PWM::PWM() { DDRB = 1 << DDB2; }

PWMView PWM::view() {
  unsigned int freq = (fast ? BASE_FAST : BASE_SLOW) / top,
               duty = trig * (fast ? TICK_FAST : TICK_SLOW);

  return {.freq = freq, .duty = duty};
}

void PWM::update(const State &state) {
  bool isFast = state.speed == Fast;
  bool isSelected = state.mod == Mod::Fixed;
  bool needsSetup = isFast != fast || enabled != state.enabled;

  if (isSelected) {
    fast = isFast;
    top = map(state.freq, 0, 1023, MIN_TOP, MAX_TOP);
    trig = map(state.ontime, 0, 1023, 0,
               isFast ? MAX_ON_TIME_FAST : MAX_ON_TIME_SLOW);
  }
  if (!(state.enabled && isSelected)) {
    if (enabled)
      disable();
    return;
  }

  if (needsSetup) {
    if (fast)
      setupFast();
    else
      setupSlow();
  } else {
    OCR1A = top;
    OCR1B = trig;
  }
}

void PWM::disable() {
  noInterrupts();
  enabled = false;
  TCCR1A = 0;
  TCCR1B = 0;
  interrupts();
}

/**
 * URL:
 * https://dbuezas.github.io/arduino-web-timers/#mcu=LGT8F328P&timer=1&topValue=OCR1A&OCR1A=65535&interruptA=off&timerMode=PCPWM&CompareOutputModeB=clear-up%2C+set-down&OCnB_OutputPort=B2&OCR1B=30191&FCPU_UI=32Mhz&clockPrescalerOrSource=8
 * Mode     : PCPWM
 * Period   : 32.7675 ms
 * Frequency: 30.51804 Hz
 * Outputs  :
 *  - B2: 46.58%, clear-up, set-down
 */

void PWM::setupFast() {
  noInterrupts();
  enabled = true;
  TCCR1A = 1 << COM1B1 | 1 << WGM11 | 1 << WGM10;
  TCCR1B = 1 << WGM13 | 1 << CS11;
  OCR1A = top;
  OCR1B = trig;
  interrupts();
}

/**
 * URL:
 * https://dbuezas.github.io/arduino-web-timers/#mcu=LGT8F328P&timer=1&topValue=OCR1A&OCR1A=65535&interruptA=off&timerMode=PCPWM&CompareOutputModeB=clear-up%2C+set-down&OCnB_OutputPort=B2&OCR1B=30527&FCPU_UI=32Mhz&clockPrescalerOrSource=8
 * Mode     : PCPWM
 * Period   : 1.04856 s
 * Frequency: 0.95369 Hz
 * Outputs  :
 *  - B2: 46.58%, clear-up, set-down
 */
void PWM::setupSlow() {
  noInterrupts();
  enabled = true;
  TCCR1A = 1 << COM1B1 | 1 << WGM11 | 1 << WGM10;
  TCCR1B = 1 << WGM13 | 1 << CS12;
  OCR1A = top;
  OCR1B = trig;
  interrupts();
}
