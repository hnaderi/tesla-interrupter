#include "interrupters.hpp"
#include <Arduino.h>

void Staccato::update(const State &state) {}

StaccatoView Staccato::view() { return {.rep = 1, .ontime = 1}; }
