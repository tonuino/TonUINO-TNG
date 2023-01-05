#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>

struct EEPROMClass{
  static constexpr int max_len = 300;
  uint8_t eeprom_mem[max_len];
  uint8_t read( int idx )              { if (idx >= 0 && idx < max_len) return eeprom_mem[idx]; return 0; }
  void write( int idx, uint8_t val )   { if (idx >= 0 && idx < max_len) eeprom_mem[idx] = val; }
  uint16_t length()                    { return max_len; }
};

extern EEPROMClass EEPROM;
#endif
