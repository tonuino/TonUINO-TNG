#ifndef SRC_LED_MANAGER_HPP_
#define SRC_LED_MANAGER_HPP_

#include <Arduino.h>
#include "constants.hpp"

/**
 * STARTUP: Running light animation during initialization
 * AWAIT_INPUT: All LEDs blink synchronously
 * PLAYING: All LEDs turned on continuously
 * PAUSED: Only the PLAY LED blinks
 * SHUTDOWN: 
 */
enum LedState { STARTUP, AWAIT_INPUT, PLAYING, PAUSED, SHUTDOWN };

constexpr uint8_t ledPins[] = {LED_DOWN_PIN, LED_PLAY_PIN, LED_UP_PIN};
constexpr size_t NUM_LEDS = 3;

class LedManager {
public:
  void begin();
  void update();
  void setState(LedState state);
  void handleBlinkOnce();

private:
  LedState currentState = STARTUP;
  unsigned long lastUpdate = 0;
  uint8_t animationStep = 0;
  bool blinkAllOnceActive = false;
  bool buttonPressBlinkActive = false;


  void setAll(bool state);
  void setAllOn();
  void setAllOff();

  void setLED(bool state, uint8_t pin);
  void toggleLED(uint8_t pin);
 
};

#endif
