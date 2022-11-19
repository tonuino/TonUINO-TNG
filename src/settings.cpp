#include "settings.hpp"

#include "constants.hpp"
#include "logger.hpp"

namespace {

//  ############### EEPROM ################################
//  Address       Usage
//    0- 99       Folder Settings (Hoerbuch Fortschritt)
//  100-137       AdminSettings (38 Byte)
//  138-155       reserved (18 Byte)
//  156-255       extra Shortcuts (100 Byte, max. 25 Shortcuts)

constexpr uint16_t startAddressFolderSettings =   0;
constexpr uint16_t startAddressAdminSettings  = 100;
constexpr uint16_t startAddressExtraShortcuts = 156;
constexpr uint16_t startAddressReserved       = 256;

#ifdef BUTTONS3X3
constexpr uint16_t maxExtraShortcuts = (startAddressReserved - startAddressExtraShortcuts) / sizeof(folderSettings);
static_assert(buttonExtSC_buttons <= maxExtraShortcuts, "Too many ExtraShortCuts");
#endif
}

#ifdef BUTTONS3X3
folderSettings Settings::extShortCut{};
#endif

void Settings::clearEEPROM() {
  LOG(settings_log, s_info, F("clearEEPROM"));
  for (uint16_t i = startAddressFolderSettings; i < startAddressReserved; ++i) {
    EEPROM.write(i, '\0');
  }
}

void Settings::writeSettingsToFlash() {
  static_assert(startAddressExtraShortcuts-startAddressAdminSettings > sizeof(Settings), "Settings to big");
  LOG(settings_log, s_debug, F("writeSettingsToFlash"));
  EEPROM_put(startAddressAdminSettings, *this);
}

void Settings::resetSettings() {
  LOG(settings_log, s_debug, F("resetSettings"));
  cookie               = cardCookie;
  version              =  2;
  maxVolume            = 25;
  minVolume            =  5;
  initVolume           = 15;
  eq                   =  1;
  locked               = false;
  standbyTimer         =  0;
#ifdef FIVEBUTTONS
  invertVolumeButtons  = false;
#else
  invertVolumeButtons  = true;
#endif
  shortCuts[0].folder  =  0;
  shortCuts[1].folder  =  0;
  shortCuts[2].folder  =  0;
  shortCuts[3].folder  =  0;
  adminMenuLocked      =  0;
  adminMenuPin[0]      =  1;
  adminMenuPin[1]      =  1;
  adminMenuPin[2]      =  1;
  adminMenuPin[3]      =  1;
  pauseWhenCardRemoved = false;

  writeSettingsToFlash();
}

void Settings::migrateSettings(int oldVersion) {
  if (oldVersion == 1) {
    LOG(settings_log, s_info, F("migradeSettings 1 -> 2"));
    version              = 2;
    adminMenuLocked      = 0;
    adminMenuPin[0]      = 1;
    adminMenuPin[1]      = 1;
    adminMenuPin[2]      = 1;
    adminMenuPin[3]      = 1;
    pauseWhenCardRemoved = false;
    writeSettingsToFlash();
  }
}

void Settings::loadSettingsFromFlash() {
  LOG(settings_log, s_debug, F("loadSettingsFromFlash"));
  EEPROM_get(startAddressAdminSettings, *this);
  if (cookie != cardCookie)
    resetSettings();
  migrateSettings(version);

  LOG(settings_log, s_info, F("Version: "                ), version);
  LOG(settings_log, s_info, F("Max Vol: "                ), maxVolume);
  LOG(settings_log, s_info, F("Min Vol: "                ), minVolume);
  LOG(settings_log, s_info, F("Init Vol: "               ), initVolume);
  LOG(settings_log, s_info, F("EQ: "                     ), eq);
  LOG(settings_log, s_info, F("Locked: "                 ), locked);
  LOG(settings_log, s_info, F("Sleep Timer: "            ), standbyTimer);
  LOG(settings_log, s_info, F("Inverted Vol Buttons: "   ), invertVolumeButtons);
  LOG(settings_log, s_info, F("Admin Menu locked: "      ), adminMenuLocked);
  LOG(settings_log, s_info, F("Admin Menu Pin: "         ), adminMenuPin[0], adminMenuPin[1], adminMenuPin[2], adminMenuPin[3]);
  LOG(settings_log, s_info, F("Pause when card removed: "), pauseWhenCardRemoved);
}

void Settings::writeFolderSettingToFlash(uint8_t folder, uint8_t track) {
  EEPROM.write(folder, track);
}

uint8_t Settings::readFolderSettingFromFlash(uint8_t folder) {
  return EEPROM.read(folder);
}

void Settings::writeExtShortCutToFlash (uint8_t shortCut, const folderSettings& value) {
  const int address = startAddressExtraShortcuts + shortCut * sizeof(folderSettings);
  EEPROM_put(address, value);
}

void Settings::readExtShortCutFromFlash(uint8_t shortCut,       folderSettings& value) {
  const int address = startAddressExtraShortcuts + shortCut * sizeof(folderSettings);
  EEPROM_get(address, value);
}


folderSettings& Settings::getShortCut(uint8_t shortCut) {
  if (shortCut > 0 && shortCut <= 4)
    return shortCuts[shortCut];
#ifdef BUTTONS3X3
  else if (shortCut >= buttonExtSC_begin && shortCut < buttonExtSC_begin + buttonExtSC_buttons) {
    readExtShortCutFromFlash(shortCut-buttonExtSC_begin, extShortCut);
    return extShortCut;
  }
#endif

  return shortCuts[0];
}

void Settings::setShortCut(uint8_t shortCut, const folderSettings& value) {
  if (shortCut > 0 && shortCut <= 4) {
    shortCuts[shortCut] = value;
    // writeSettingsToFlash(); -- will be done in state machine
  }
#ifdef BUTTONS3X3
  else if (shortCut >= buttonExtSC_begin && shortCut < buttonExtSC_begin + buttonExtSC_buttons) {
    writeExtShortCutToFlash(shortCut-buttonExtSC_begin, value);
  }
#endif
}

