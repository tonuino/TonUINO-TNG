#ifndef SRC_SETTINGS_HPP_
#define SRC_SETTINGS_HPP_

#include <Arduino.h>

#include "array.hpp"
#include "chip_card.hpp"
#include <EEPROM.h>

// admin settings stored in eeprom
struct Settings
{
  typedef array<folderSettings, 4> shortCuts_t;
  typedef array<uint8_t, 4> pin_t;

  void writeByteToFlash(uint16_t address, uint8_t value);
  uint8_t readByteFromFlash(uint16_t address);

  void clearEEPROM();

  void writeSettingsToFlash();
  void resetSettings();
  void migrateSettings(int oldVersion);
  void loadSettingsFromFlash();

  void writeFolderSettingToFlash(uint8_t folder, uint16_t track);
  uint16_t readFolderSettingFromFlash(uint8_t folder);

  uint32_t cookie;
  byte version;
  uint8_t maxVolume;
  uint8_t minVolume;
  uint8_t initVolume;
  uint8_t eq;
  bool locked;
  long standbyTimer;
  bool invertVolumeButtons;
  shortCuts_t shortCuts;
  uint8_t adminMenuLocked;
  pin_t adminMenuPin;
  bool pauseWhenCardRemoved;
};

// emulates EEPROM.put() .get() and .update() on LGT8F328P platform
// partially sourced from: https://playground.arduino.cc/Code/EEPROMWriteAnything/
template <class T>
int EEPROM_put(int ee, const T &value)
{
  const byte *p = (const byte *)(const void *)&value;
  unsigned int i;

  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T>
int EEPROM_get(int ee, T &value)
{
  byte *p = (byte *)(void *)&value;
  unsigned int i;

  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

template <class T>
void EEPROM_update(int ee, const T &value)
{
  EEPROM.read(ee) != value ? EEPROM.write(ee, value) : delay(0);
}

#endif /* SRC_SETTINGS_HPP_ */
