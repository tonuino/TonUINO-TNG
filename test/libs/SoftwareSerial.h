#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <Arduino.h>

class SoftwareSerial
{
public:
  SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false) {}
  ~SoftwareSerial() {}
};

#endif
