#ifndef SRC_RING_HPP
#define SRC_RING_HPP

#include <Arduino.h>

#include "constants.hpp"
#include "array.hpp"

#include <Adafruit_NeoPixel.h>

inline constexpr uint8_t  pulse_per_second = 1;
inline constexpr uint8_t  brightness_max   = 32;
inline constexpr uint8_t  brightness_init  = 16;

class Ring {
public:
  Ring();

  void init();

  struct color_t {
    byte r;
    byte g;
    byte b;
    constexpr color_t operator*(const uint8_t s) const {
      return color_t { static_cast<byte>(static_cast<uint16_t>(r) * s / 255)
                     , static_cast<byte>(static_cast<uint16_t>(g) * s / 255)
                     , static_cast<byte>(static_cast<uint16_t>(b) * s / 255) };
    }
  };

  static constexpr color_t red   { 255,   0,   0 };
  static constexpr color_t green {   0, 255,   0 };
  static constexpr color_t blue  {   0,   0, 255 };
  static constexpr color_t black {   0,   0,   0 };

  void call_on_startup  () { setAll   (red  ); }
  void call_on_idle     () { pulse    (green); }
  void call_on_startPlay() { pulse    (red  ); }
  void call_on_play     () { rainbow  (5    ); }
  void call_on_pause    () { rainbow  (0    ); }
  void call_on_admin    () { pulse    (blue ); }
  void call_on_sleep    () { setAll   (black); }

  void brightness_up    () { if (brightness < brightness_max) ++brightness; strip.setBrightness(brightness); }
  void brightness_down  () { if (brightness > 0             ) --brightness; strip.setBrightness(brightness); }
private:

  void showStrip() { strip.show(); }
  void setPixel(int pixel, color_t color) { strip.setPixelColor(pixel, strip.Color(color.r, color.g, color.b)); }

  color_t wheel(byte wheelPos) const;

  void pulse(const color_t color);
  void rainbow(uint8_t incr);
  void setAll(const color_t color);
  void setAll(auto&& f);

  uint8_t brightness { brightness_init };

  // for pulse()
  uint8_t brightness_pulse { 50 };
  int8_t  brightness_inc { cycleTime*255/pulse_per_second/1000 };


  // for rainbow()
  uint8_t pixelCycle { 0 };  // Pattern Pixel Cycle

  Adafruit_NeoPixel strip;
};

void Ring::setAll(auto&& f) {
  for (uint8_t i = 0; i < neoPixelNumber; ++i) {
    setPixel(i, f(i));
  }
  showStrip();
}

#endif // SRC_RING_HPP
