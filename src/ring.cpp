#include "constants.hpp"
#ifdef NEO_RING

#include "ring.hpp"

#include <limits.h>

Ring::Ring(uint32_t cycle_time_ms)
: cycles_per_second(1000 / cycle_time_ms)
, strip(neoPixelNumber, neoPixelRingPin, NEO_GRB + NEO_KHZ800)
{}

void Ring::init() {
  strip.begin();
  strip.setBrightness(50);
}

void Ring::setAll(color_t color) {
  for (uint8_t i = 0; i < neoPixelNumber; i++) {
    setPixel(i, color);
  }
  showStrip();
}

void Ring::colorWipe(color_t color) {
  setPixel(pixelCurrent, color);
  showStrip();

  pixelCurrent++;
  if (pixelCurrent >= neoPixelNumber) {
    pixelCurrent = 0;
    strip.clear();
  }
}

// Rainbow cycle along whole strip.
void Ring::rainbow() {
  for (uint8_t i = 0; i < neoPixelNumber; i++) {
    setPixel(i, wheel((10 * i + pixelCycle) & 255));
  }
  showStrip();

  pixelCycle += 5;
  if (pixelCycle >= cycles_per_second * 5 * 5) {
    pixelCycle = 0;
  }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
Ring::color_t Ring::wheel(byte WheelPos) {
  color_t color;
  if (WheelPos == 255) WheelPos = 254;
  const uint16_t WheelPos_times_3 = (WheelPos % 85) * 3;
  if (WheelPos < 85) {
    color.r = WheelPos_times_3;
    color.g = 255 - WheelPos_times_3;
    color.b = 0;
  } else if (WheelPos < 170) {
    color.r = 255 - WheelPos_times_3;
    color.g = 0;
    color.b = WheelPos_times_3;
  } else {
    color.r = 0;
    color.g = WheelPos_times_3;
    color.b = 255 - WheelPos_times_3;
  }

  return color;
}

#endif // NEO_RING
