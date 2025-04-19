#include "interrupters.hpp"
#include <Arduino.h>

static const unsigned int MAX_DUTY = 50, MAX_TOP = __UINT16_MAX__,
                          MIN_TOP = 2000, MIN_DUTY = 0;

static const float BASE_SLOW = F_CPU / 256. / 2.;
static const float BASE_FAST = F_CPU / 8. / 2.;

PWMView PWM::view() {
  unsigned int freq = BASE_FAST / top, duty = (float)trig / top * 100.0;

  return {.freq = freq, .duty = duty};
}

void PWM::update(const State &state) {
  bool isEnabled = state.enabled && state.mod == Fixed;
  bool isFast = state.speed == Fast;
  bool needsSetup = isFast != fast || isEnabled != enabled;

  enabled = isEnabled;
  fast = isFast;

  if (enabled) {
    top = map(state.freq, 0, 1023, MIN_TOP, MAX_TOP);
    trig = map(state.ontime, 0, 1023, MIN_DUTY, MAX_DUTY) / 100.0 * top;

    if (needsSetup) {
      if (fast)
        setupFast();
      else
        setupSlow();
    } else {
      OCR1A = top;
      OCR1B = trig;
    }

  } else
    disable();
}

void PWM::disable() {
  noInterrupts();
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
  TCCR1A = 1 << COM1B1 | 1 << WGM11 | 1 << WGM10;
  TCCR1B = 1 << WGM13 | 1 << CS11;
  DDRB = 1 << DDB2;
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
  TCCR1A = 1 << COM1B1 | 1 << WGM11 | 1 << WGM10;
  TCCR1B = 1 << WGM13 | 1 << CS12;
  DDRB = 1 << DDB2;
  OCR1A = top;
  OCR1B = trig;
  interrupts();
}
