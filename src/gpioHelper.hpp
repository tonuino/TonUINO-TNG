#ifndef SRC_GPIOHELPER_HPP_
#define SRC_GPIOHELPER_HPP_

#include <Arduino.h>

// ####### helper for level ############################

enum class level : uint8_t {
  inactive,
  active  ,
  unknown ,
};
enum class levelType : uint8_t {
  activeHigh, // normally open
  activeLow , // normally closed
};

inline constexpr int   getLevel(levelType t, level l) { return (l == level::inactive) ? (t == levelType::activeHigh ? LOW : HIGH)
                                                                                      : (t == levelType::activeHigh ? HIGH : LOW); }
inline constexpr level getLevel(levelType t, int   l) { return (l == LOW)             ? (t == levelType::activeHigh ? level::inactive : level::active  )
                                                                                      : (t == levelType::activeHigh ? level::active   : level::inactive); }
#endif /* SRC_GPIOHELPER_HPP_ */
