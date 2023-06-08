#ifndef SRC_RING_HPP
#define SRC_RING_HPP

#include <Arduino.h>

#include "array.hpp"

#include <Adafruit_NeoPixel.h>

class Ring {
public:
  Ring(uint32_t cycle_time);

  void init();

  struct color_t {
    byte r;
    byte g;
    byte b;
    color_t operator*(const double s) {
      return color_t { static_cast<byte>(r * s), static_cast<byte>(g * s),
          static_cast<byte>(b * s) };
    }
  };

  void colorWipe(color_t color);
  void rainbow();
  void setAll(color_t color);

private:

  const uint16_t cycles_per_second;

  void showStrip() { strip.show(); }
  void setPixel(int pixel, color_t color) { strip.setPixelColor(pixel, strip.Color(color.r, color.g, color.b)); }

  color_t wheel(byte wheelPos);

  unsigned int pixelCycle { 0 };  // Pattern Pixel Cycle
  uint16_t pixelCurrent { 0 };  // Pattern Current Pixel Number

  Adafruit_NeoPixel strip;
};

#endif // SRC_RING_HPP
