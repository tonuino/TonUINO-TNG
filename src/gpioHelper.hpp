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

inline constexpr int   level2int(levelType t, level l) { return (l == level::inactive) ? (t == levelType::activeHigh ? LOW : HIGH)
                                                                                       : (t == levelType::activeHigh ? HIGH : LOW); }

inline void input_pin_mode (uint8_t pin, levelType pin_type) { pinMode(pin, (pin_type == levelType::activeHigh) ? INPUT : INPUT_PULLUP); };

inline bool pin_is_active  (uint8_t pin, levelType pin_type) { return digitalRead(pin)==level2int(pin_type, level::  active); };
inline bool pin_is_inactive(uint8_t pin, levelType pin_type) { return digitalRead(pin)==level2int(pin_type, level::inactive); };

inline void pin_set_active  (uint8_t pin, levelType pin_type) { digitalWrite(pin, level2int(pin_type, level::  active)); };
inline void pin_set_inactive(uint8_t pin, levelType pin_type) { digitalWrite(pin, level2int(pin_type, level::inactive)); };
inline void pin_set_level   (uint8_t pin, levelType pin_type, level l) { digitalWrite(pin, level2int(pin_type, l)); };


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

template<int i> struct check_pcb {
  enum {v = 0};
  enum {s = v + check_pcb<i-1>::s};
};
template<> struct check_pcb<0> {
  enum {s = 0};
};

#define DECL_PCB(i)           \
template<> struct check_pcb<i> {       \
  enum {v = 1};                  \
  enum {s = v + check_pcb<i-1>::s};    \
};

#define SUM_PCB check_pcb<20>::s

#endif /* SRC_GPIOHELPER_HPP_ */
