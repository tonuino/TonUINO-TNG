#ifndef SRC_RING_HPP
#define SRC_RING_HPP

#include <Arduino.h>

#include "constants.hpp"
#include "array.hpp"

#include <Adafruit_NeoPixel.h>

inline constexpr uint8_t  pulse_per_second = 1;
inline constexpr uint8_t  brightness_pulse_max = 200;
inline constexpr uint8_t  brightness_pulse_min =  50;

inline constexpr uint8_t  brightness_max   = 32;
inline constexpr uint8_t  brightness_init  = 16;

enum class direction: uint8_t {
  cw,
  ccw,
};

class OneRing {
public:
  OneRing(uint8_t pin = neoPixelRingPin, direction dir = direction::cw);

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
  static constexpr color_t orange{ 192,  64,   0 };

  void call_on_startup  () { setAll   (red  ); }
  void call_on_idle     () { pulse    (green); }
  void call_on_startPlay() { pulse    (red  ); }
  void call_on_play     () { rainbow  (5    ); }
  void call_on_game     () { rainbow  (10   ); }
  void call_on_pause    () { rainbow  (0    ); }
  void call_on_admin    () { pulse    (blue ); }
  void call_on_sleep    () { setAll   (black); }
  void call_on_volume(uint8_t v)
                           { level    (v    ); }
  void call_on_sleep_timer() { if (++fire_sim%4==0) setAll   (orange*random(100,255)); }
  void call_before_sleep(uint8_t r) { setAll   (orange*r); }

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
  void level(uint8_t l);

  uint8_t brightness { brightness_init };

  // for pulse()
  uint8_t brightness_pulse{ brightness_pulse_min };
  int8_t  brightness_inc  { cycleTime*255/pulse_per_second/1000 };

  // for rainbow()
  uint8_t pixelCycle { 0 };  // Pattern Pixel Cycle

  // for sleep timer
  uint8_t fire_sim = 0;

  direction dir;

  Adafruit_NeoPixel strip;
};

#ifdef NEO_RING_2
class TwoRings {
public:
  TwoRings()
  : ring1(neoPixelRingPin)
  , ring2(neoPixelRingPin2, direction::ccw)
  {}

  void init                      () { ring1.init               ( ); ring2.init               ( ); }

  void call_on_startup           () { ring1.call_on_startup    ( ); ring2.call_on_startup    ( ); }
  void call_on_idle              () { ring1.call_on_idle       ( ); ring2.call_on_idle       ( ); }
  void call_on_startPlay         () { ring1.call_on_startPlay  ( ); ring2.call_on_startPlay  ( ); }
  void call_on_play              () { ring1.call_on_play       ( ); ring2.call_on_play       ( ); }
  void call_on_game              () { ring1.call_on_game       ( ); ring2.call_on_game       ( ); }
  void call_on_pause             () { ring1.call_on_pause      ( ); ring2.call_on_pause      ( ); }
  void call_on_admin             () { ring1.call_on_admin      ( ); ring2.call_on_admin      ( ); }
  void call_on_sleep             () { ring1.call_on_sleep      ( ); ring2.call_on_sleep      ( ); }
  void call_on_volume(uint8_t    v) { ring1.call_on_volume     (v); ring2.call_on_volume     (v); }
  void call_on_sleep_timer       () { ring1.call_on_sleep_timer( ); ring2.call_on_sleep_timer( ); }
  void call_before_sleep(uint8_t r) { ring1.call_before_sleep  (r); ring2.call_before_sleep  (r); }
  void brightness_up             () { ring1.brightness_up      ( ); ring2.brightness_up      ( ); }
  void brightness_down           () { ring1.brightness_down    ( ); ring2.brightness_down    ( ); }

private:
  OneRing ring1;
  OneRing ring2;
};
using Ring = TwoRings;
#else
using Ring = OneRing;
#endif // NEO_RING_2

void OneRing::setAll(auto&& f) {
  for (uint8_t i = 0; i < neoPixelNumber; ++i) {
    uint8_t fi = (dir == direction::cw) ? i : neoPixelNumber - i - 1;
    setPixel(i, f(fi));
  }
  showStrip();
}

#endif // SRC_RING_HPP
