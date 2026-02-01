#include "led_manager.hpp"
#include "constants.hpp"

#ifdef USE_LED_BUTTONS

#include "logger.hpp"

void LedManager::begin() {
  for (uint8_t pin : ledPins) {
    pinMode(pin, OUTPUT);
  }

  setLED(led_down_pin, HIGH);
  setLED(led_play_pin, LOW );
  setLED(led_up_pin  , HIGH);

  LOG(ledManager_log, s_info, F("LEDs(D/P/U): "), led_down_pin, F(" "), led_play_pin, F(" "), led_up_pin);
  LOG(ledManager_log, s_info, F("NUM LEDS: "), NUM_LEDS);
}

void LedManager::setState(ledState state) {

  if (currentState != state) {
    LOG(ledManager_log, s_debug, F("LED Manager, new state: "), static_cast<uint8_t>(state));
    currentState = state;
    animationStep = 0;

    if (!buttonPressBlinkActive) //Prevent immediate update if a blink triggered by a button press is active.
      updateTimer.start(led_update_interval);

    if (currentState == ledState::shutdown) { //Immediate Update
      updateTimer.stop();
      update();
    }
  }
}

void LedManager::handleBlinkOnce() {
  if (buttonPressBlinkActive)
    return;

  toggleAll();

  buttonPressBlinkActive = true;
  updateTimer.start(led_short_blink);
  LOG(ledManager_log, s_debug, F("LED Manager start buttonPressBlink"));
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

void LedManager::setLED(uint8_t pin, uint8_t state) {
  digitalWrite(pin, state);
  LOG(ledManager_log, s_debug, F("LED Manager, set["), pin, F("] to "), state);
}

void LedManager::update() {

  // Only update LEDs if enough time has passed since last update.
  if (not updateTimer.isExpired())
    return;
  else
    updateTimer.start(led_update_interval);

  LOG(ledManager_log, s_debug, F("LED Manager update in state: "),  static_cast<uint8_t>(currentState));

  if (buttonPressBlinkActive && currentState != ledState::shutdown) {
    toggleAll();
    buttonPressBlinkActive = false;
    LOG(ledManager_log, s_debug, F("LED Manager end buttonPressBlink"));
    return;
  }

  switch (currentState) {
  case ledState::startup:
    // Running light effect: only one LED is on, moves to next LED on each step.
    for (size_t i = 0; i < NUM_LEDS; ++i) {
      setLED(ledPins[i], animationStep % NUM_LEDS == i);
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
    setLED(led_down_pin, LOW                   );
    setLED(led_play_pin, animationStep % 2 == 0);
    setLED(led_up_pin  , LOW                   );
    ++animationStep;
    break;

  case ledState::shutdown:
    // Turn off all LEDs.
    setAllOff();
    break;
  }
}

#endif
