#ifndef SRC_LED_MANAGER_HPP_
#define SRC_LED_MANAGER_HPP_

#include "constants.hpp"

#include "timer.hpp"

#ifdef USE_LED_BUTTONS

/**
 * startup: Running light animation during initialization
 * await_input: All LEDs blink synchronously
 * playing: All LEDs turned on continuously
 * paused: Only the PLAY LED blinks
 * shutdown:
 */
enum class ledState: uint8_t {
  startup,
  await_input,
  playing,
  paused,
  shutdown
};

constexpr uint8_t ledPins[] = {led_down_pin, led_play_pin, led_up_pin};
constexpr size_t NUM_LEDS   = sizeof(ledPins)/sizeof(ledPins[0]);

class LedManager {
public:
  void begin          ();
  void update         ();
  void setState       (ledState state);
  void handleBlinkOnce();

private:
  ledState      currentState          {ledState::startup};
  uint8_t       animationStep         {0};
  bool          blinkAllOnceActive    {false};
  bool          buttonPressBlinkActive{false};
  Timer         updateTimer           {};


  void setAll   (uint8_t state);
  void toggleAll();
  void setAllOn () { setAll(HIGH); }
  void setAllOff() { setAll(LOW); }

  void setLED   (uint8_t state, uint8_t pin);
};
#endif // USE_LED_BUTTONS
#endif
