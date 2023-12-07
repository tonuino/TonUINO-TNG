#ifndef Arduino_h
#define Arduino_h


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sstream>


#ifdef __cplusplus
extern "C"{
#endif

#define PROGMEM
inline void *memcpy_P(void * dest, const void * src, size_t s) {
  return memcpy(dest, src, s);
}

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))

typedef uint8_t byte;

typedef char __FlashStringHelper;
#define F(x) x

extern unsigned long current_time;
inline unsigned long millis() { return current_time; }
inline void delay(unsigned long ms) { current_time += ms; }

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2

#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#define PIN_A5   (19)
#define PIN_A6   (20)
#define PIN_A7   (21)

#define PIN_A8   (22)
#define PIN_A9   (23)
#define PIN_A10  (24)
#define PIN_A11  (25)
#define PIN_A12  (26)
#define PIN_A13  (27)
#define PIN_A14  (28)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

static const uint8_t A8  = PIN_A8;
static const uint8_t A9  = PIN_A9;
static const uint8_t A10 = PIN_A10;
static const uint8_t A11 = PIN_A11;
static const uint8_t A12 = PIN_A12;
static const uint8_t A13 = PIN_A13;
static const uint8_t A14 = PIN_A14;

static const uint8_t max_pin = 32;

extern uint8_t pin_mode[max_pin];
extern int pin_value[max_pin];

inline void reset_pin_value(uint8_t pin) {
  if (pin < max_pin) {
    if (pin_mode[pin] == INPUT_PULLUP)
      pin_value[pin] = 1;
    else
      pin_value[pin] = 0;
  }
}
inline void pinMode(uint8_t pin, uint8_t mode) {
  if (pin < max_pin) {
    pin_mode[pin] = mode;
    reset_pin_value(pin);
  }
}

inline void reset_all_pin_values() {
  for (uint8_t pin = 0; pin < max_pin; ++pin)
    reset_pin_value(pin);
}



inline int digitalRead(uint8_t pin) { if (pin < max_pin) return pin_value[pin]; return 0; }
inline void digitalWrite(uint8_t pin, uint8_t val) { if (pin < max_pin) pin_value[pin] = val; }
inline int analogRead(uint8_t pin) { if (pin <= A7 && pin >= A0) return pin_value[pin]; return 0; }
inline void analogReference(uint8_t mode) {}
#define INTERNAL2V5 0
#define INTERNAL2V048 0
inline void analogReadResolution(uint8_t) {}

// test functions
inline void press_button  (uint8_t pin) { if (pin < max_pin) pin_value[pin] = LOW; }
inline void release_button(uint8_t pin) { if (pin < max_pin) pin_value[pin] = HIGH; }
// test functions end

#define set_sleep_mode(mode)
#define cli()
#define sleep_mode()

#ifdef __cplusplus
} // extern "C"
#endif

inline void randomSeed(unsigned long seed) { srand(seed); }

inline long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
  return rand() % howbig;
}

inline long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

/*
 Print.cpp - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 Modified 23 November 2006 by David A. Mellis
 Modified 03 August 2015 by Chuck Todd

 Boerge1: adapted for mocking in unit tests
 */

class Print
{
  public:
    static std::stringstream s;
    static std::string get_output() { return s.str(); }
    static void clear_output() { s.str(std::string());; }

    size_t write(uint8_t o) { s << o; return 1; }
    size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
    }
    size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
    }

    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);
private:
    size_t printNumber(unsigned long n, uint8_t base);
    size_t printFloat(double number, uint8_t digits);
};

class HardwareSerial: public Print {
public:
  int available() { return 0; }
  long parseInt() { return 0; }
};

extern HardwareSerial Serial;
extern HardwareSerial Serial3;
#endif
