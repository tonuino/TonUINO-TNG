#include "led_manager.hpp"
#include "constants.hpp"

#ifdef USE_LED_BUTTONS

#include "logger.hpp"

void LedManager::begin() {
  for (uint8_t pin : ledPins) {
    pinMode(pin, OUTPUT);
  }

  setAllOff();

  LOG(ledManager_log, s_info, F("LEDs(D/P/U): "), led_down_pin, F(" "), led_play_pin, F(" "), led_up_pin);
  LOG(ledManager_log, s_info, F("NUM LEDS"), NUM_LEDS, F(" "));
}

void LedManager::setState(ledState state) {

  if (currentState != state) {
    currentState = state;
    animationStep = 0;

    if (!buttonPressBlinkActive) //Prevent immediate update if a blink triggered by a button press is active.
      updateTimer.start(led_update_interval);

    if (currentState == ledState::shutdown) //Immediate Update
      updateTimer.stop();

    update();
  }
}

void LedManager::handleBlinkOnce() {
  toggleAll();

  buttonPressBlinkActive = true;
  updateTimer.start(led_short_blink);
}

void LedManager::setAll(uint8_t state) {
  for (uint8_t pin : ledPins) {
    digitalWrite(pin, state);
  }
}

void LedManager::toggleAll() {
  for (uint8_t pin : ledPins) {
    digitalWrite(pin, !digitalRead(pin));
  }
}

void LedManager::setLED(uint8_t state, uint8_t pin) {
  digitalWrite(pin, state);
}

void LedManager::update() {

  // Only update LEDs if enough time has passed since last update.
  if (not updateTimer.isExpired())
    return;
  else
    updateTimer.start(led_update_interval);

  if (buttonPressBlinkActive && currentState != ledState::shutdown) {
    toggleAll();
    buttonPressBlinkActive = false;
    return;
  }

  switch (currentState) {
  case ledState::startup:
    // Running light effect: only one LED is on, moves to next LED on each step.
    for (size_t i = 0; i < NUM_LEDS; ++i) {
      digitalWrite(ledPins[i], animationStep % NUM_LEDS == i);
    }
    ++animationStep;
    break;

  case ledState::await_input:
    /// All LEDs blink on and off in sync.
    (animationStep % 2 == 0) ? setAllOn() : setAllOff();
     ++animationStep;
    break;

  case ledState::playing:
    // All LEDs turned on continuously. Ensures LEDs are only set once to avoid repeated writes.
    if (animationStep == 0) {
      setAllOn();
      animationStep = 1;
    } 
    break;

  case ledState::paused:
    // Only the middle LED (PLAY) blinks; others remain off.
    digitalWrite(led_down_pin, LOW);
    digitalWrite(led_play_pin, animationStep % 2 == 0);
    digitalWrite(led_up_pin, LOW);
    ++animationStep;
    break;

  case ledState::shutdown:
    // Turn off all LEDs.
    setAllOff();
    break;
  }
}

#endif
