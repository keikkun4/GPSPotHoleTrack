#include "botones.h"
#include <Arduino.h>

BUTTON::BUTTON(int pin):
nPin(pin)
{
  state_ant = false;
  state_act = false;
}

void BUTTON::begin() {
  pinMode(nPin, INPUT_PULLUP);
}

void BUTTON::read() {
  state_ant = state_act;
  state_act = !digitalRead(nPin);
}

bool BUTTON::pressed() {
  bool pressed_;

  pressed_ = !state_ant && state_act;
  state_ant = state_act;
  return pressed_;
}

