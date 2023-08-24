#include "constants.hpp"
#ifdef NEO_RING
#include "ring.hpp"

#include <limits.h>

Ring::Ring()
: strip(neoPixelNumber, neoPixelRingPin, NEO_GRB + NEO_KHZ800)
{}

void Ring::init() {
  strip.begin();
  strip.setBrightness(brightness);
}

void Ring::pulse(const color_t color) {
  brightness_pulse += brightness_inc;
  if (brightness_pulse >= 200 or brightness_pulse <= 50)
    brightness_inc *= -1;

  setAll(color*brightness_pulse);
}

// Rainbow cycle along whole strip.
void Ring::rainbow(uint8_t incr){
  setAll([this](uint8_t i){ return wheel((255/neoPixelNumber * (neoPixelNumber-1-i) + pixelCycle) & 255); });

  pixelCycle += incr;
}

void Ring::setAll(const color_t color) {
  for (uint8_t i = 0; i < neoPixelNumber; i++) {
    setPixel(i, color);
  }
  showStrip();
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
Ring::color_t Ring::wheel(byte WheelPos) const {
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
