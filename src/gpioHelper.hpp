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
#if not defined(TonUINO_Esp32) and not defined(D0)
#define D0   0
#define D1   1
#define D2   2
#define D3   3
#define D4   4
#define D5   5
#define D6   6
#define D7   7

#define D8   8
#define D9   9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#endif

#endif /* SRC_GPIOHELPER_HPP_ */
