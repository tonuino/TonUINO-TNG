#include "constants.hpp"
#ifdef USE_LED_BUTTONS
#include "led_manager.hpp"
#include "logger.hpp"

void LedManager::begin() {
  for (uint8_t pin : ledPins) {
    pinMode(pin, OUTPUT);
  }

  LOG(ledManager_log, s_info, F("LEDs(D/P/U): "), LED_DOWN_PIN, F(" "),
      LED_PLAY_PIN, F(" "), LED_UP_PIN);
  LOG(ledManager_log, s_info, F("NUM LEDS"), NUM_LEDS, F(" "));
}

void LedManager::setState(LedState state) {

  if (currentState != state) {
    currentState = state;
    animationStep = 0;

    if (!buttonPressBlinkActive) //Prevent immediate update if a blink triggered by a button press is active.
      lastUpdate = 0;

    if (currentState == SHUTDOWN) //Immediate Update
      lastUpdate = 0;

    update();
  }
}

void LedManager::handleBlinkOnce() {
  for (uint8_t pin : ledPins) {
    digitalWrite(pin, !digitalRead(pin));
  }

  buttonPressBlinkActive = true;
  lastUpdate = millis() - 400; // Reduce the blink cycle duration to speed up blinking.

}

void LedManager::setAll(bool state) {
  for (uint8_t pin : ledPins) {
    digitalWrite(pin, state);
  }
}

void LedManager::setAllOn() { setAll(true); }

void LedManager::setAllOff() { setAll(false); }

void LedManager::setLED(bool state, uint8_t pin) { digitalWrite(pin, state); }

void LedManager::toggleLED(uint8_t pin) {
  digitalWrite(pin, !digitalRead(pin));
}

void LedManager::update() {

  unsigned long now = millis();

  // Only update LEDs if enough time has passed since last update.
  if (now - lastUpdate < LED_UPDATE_INTERVAL) {
    return;
  }

  lastUpdate = now;

  if (buttonPressBlinkActive && currentState != SHUTDOWN) {
    for (uint8_t pin : ledPins) {
      toggleLED(pin);
    }
    buttonPressBlinkActive = false;
    return;
  }

  switch (currentState) {
  case STARTUP:
    // Running light effect: only one LED is on, moves to next LED on each step.
    for (size_t i = 0; i < NUM_LEDS; ++i) {
      digitalWrite(ledPins[i], animationStep % NUM_LEDS == i);
    }
    animationStep++;
    break;

  case AWAIT_INPUT:
    /// All LEDs blink on and off in sync.
    (animationStep % 2 == 0) ? setAllOn() : setAllOff();
     animationStep = (animationStep + 1) % 2;
    break;

  case PLAYING:
    // All LEDs turned on continuously. Ensures LEDs are only set once to avoid repeated writes.
    if (animationStep == 0) {
      setAllOn();
      animationStep = 1;
    } 
    break;

  case PAUSED:
    // Only the middle LED (PLAY) blinks; others remain off.
    digitalWrite(LED_DOWN_PIN, LOW);
    digitalWrite(LED_PLAY_PIN, animationStep % 2 == 0);
    digitalWrite(LED_UP_PIN, LOW);
    animationStep = (animationStep + 1) % 2;
    break;

  case SHUTDOWN:
    // Turn off all LEDs.
    setAllOff();
    break;
  }
}

#endif