
#include "timer.hpp"

#include <Arduino.h>

void Timer::start(unsigned long timeout) {
  expireTime = millis() + timeout;
  active = true;
}
bool Timer::isExpired() {
  if (not active)
    return true;
  if (expireTime <= millis()) {
    active = false;
    return true;
  }
  return false;
}

void Timer::stop() {
  active = false;
}

unsigned long Timer::remainingTime() {
  if (not active)
    return 0xffffffff;
  return expireTime - millis();
}

