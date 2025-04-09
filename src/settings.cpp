#include "settings.hpp"

#include "constants.hpp"
#include "logger.hpp"

namespace {

//  ############### EEPROM ################################
//  Address       Usage
//    0- 99       Folder Settings (Hoerbuch Fortschritt)
//  100-140       AdminSettings (41 Byte)
//  141-155       reserved (15 Byte)
//  156-255       extra Shortcuts (100 Byte, max. 25 Shortcuts)

// Nano:      2048 byte
// Nano Every: 256 byte
// AiO:        512 byte (with framework-lgt8fx@1.0.6 emulated)
// AiOplus:    256 byte
// Esp32 Nano  256 byte (configured in EEPROM.begin())

constexpr uint16_t startAddressFolderSettings =   0;
constexpr uint16_t startAddressAdminSettings  = 100;
constexpr uint16_t startAddressExtraShortcuts = 156;
constexpr uint16_t endAddress                 = 256;

#ifdef BUTTONS3X3
constexpr uint16_t maxExtraShortcuts = (endAddress - startAddressExtraShortcuts) / sizeof(folderSettings);
static_assert(buttonExtSC_buttons < maxExtraShortcuts, "Too many ExtraShortCuts");
#endif
}

void Settings::init() {
#ifdef TonUINO_Esp32
  bool res = EEPROM.begin(endAddress);
  LOG(settings_log, s_info, F("EEPROM begin: "), res);
#endif
}

void Settings::commit() {
#ifdef TonUINO_Esp32
  bool res = EEPROM.commit();
  LOG(settings_log, s_info, F("EEPROM commit: "), res);
#endif
}

void Settings::clearEEPROM() {
  LOG(settings_log, s_info, F("clEEPROM"));
  for (uint16_t i = startAddressFolderSettings; i < endAddress; ++i) {
    EEPROM_put(i, '\0');
  }
  commit();
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
  spkMaxVolume         = 25;
  spkMinVolume         =  5;
  spkInitVolume        = 15;
  eq                   =  1;
  dummy                =  0;
  standbyTimer         =  0;
  invertVolumeButtons  =  1;
  shortCuts[0]         =  { 0, pmode_t::none, 0, 0 };
  shortCuts[1]         =  { 0, pmode_t::none, 0, 0 };
  shortCuts[2]         =  { 0, pmode_t::none, 0, 0 };
  shortCuts[3]         =  { 0, pmode_t::none, 0, 0 };
  adminMenuLocked      =  0;
  adminMenuPin[0]      =  1;
  adminMenuPin[1]      =  1;
  adminMenuPin[2]      =  1;
  adminMenuPin[3]      =  1;
  pauseWhenCardRemoved =  0;
  hpMaxVolume          = 25;
  hpMinVolume          =  5;
  hpInitVolume         = 15;

  writeSettingsToFlash();
}

void Settings::loadSettingsFromFlash() {
  LOG(settings_log, s_debug, F("loadSettings"));
  EEPROM_get(startAddressAdminSettings, *this);
  if (cookie != cardCookie)
    resetSettings();

  if (pauseWhenCardRemoved == 255) {
    pauseWhenCardRemoved = 0;
    writeSettingsToFlash();
  }

  if (hpMaxVolume == 255 || hpMaxVolume == 0) {
    hpMaxVolume          = 25;
    hpMinVolume          =  5;
    hpInitVolume         = 15;
    writeSettingsToFlash();
  }

  LOG(settings_log, s_info, F("Ver:"), version);
  LOG(settings_log, s_info, F("Vol:"), spkMaxVolume, F(" "), spkMinVolume, F(" "), spkInitVolume, F(" "), hpMaxVolume, F(" "), hpMinVolume, F(" "), hpInitVolume);
  LOG(settings_log, s_info, F("EQ:" ), eq);
  LOG(settings_log, s_info, F("ST:" ), standbyTimer);
  LOG(settings_log, s_info, F("IB:" ), invertVolumeButtons);
  LOG(settings_log, s_info, F("AL:" ), adminMenuLocked);
  LOG(settings_log, s_info, F("AP:" ), adminMenuPin[0], adminMenuPin[1], adminMenuPin[2], adminMenuPin[3]);
  LOG(settings_log, s_info, F("PCR:"), pauseWhenCardRemoved);
}

void Settings::writeFolderSettingToFlash(uint8_t folder, uint8_t track) {
  if (folder < 100)
    EEPROM_put(folder, track);
}

uint8_t Settings::readFolderSettingFromFlash(uint8_t folder) {
  uint8_t ret = 0;
  if (folder < 100)
    EEPROM_get(folder, ret);
  return ret;
}

void Settings::writeExtShortCutToFlash (uint8_t shortCut, const folderSettings& value) {
  const int address = startAddressExtraShortcuts + shortCut * sizeof(folderSettings);
  EEPROM_put(address, value);
}

void Settings::readExtShortCutFromFlash(uint8_t shortCut,       folderSettings& value) {
  const int address = startAddressExtraShortcuts + shortCut * sizeof(folderSettings);
  EEPROM_get(address, value);
}


folderSettings Settings::getShortCut(uint8_t shortCut) {
#ifdef TonUINO_Esp32
  if (shortCut == 0)
    return cardFromWeb;
#endif
  if (shortCut > 0 && shortCut <= 4)
    return shortCuts[shortCut-1];
#ifdef BUTTONS3X3
  else if (shortCut >= buttonExtSC_begin && shortCut < buttonExtSC_begin + buttonExtSC_buttons) {
    folderSettings extShortCut;
    readExtShortCutFromFlash(shortCut-buttonExtSC_begin, extShortCut);
    return extShortCut;
  }
#endif

  return { 0, pmode_t::none, 0, 0 };
}

void Settings::setShortCut(uint8_t shortCut, const folderSettings& value) {
#ifdef TonUINO_Esp32
  if (shortCut == 0)
    cardFromWeb = value;
#endif
  if (shortCut > 0 && shortCut <= 4) {
    shortCuts[shortCut-1] = value;
    // writeSettingsToFlash(); -- will be done in state machine
  }
#ifdef BUTTONS3X3
  else if (shortCut >= buttonExtSC_begin && shortCut < buttonExtSC_begin + buttonExtSC_buttons) {
    writeExtShortCutToFlash(shortCut-buttonExtSC_begin, value);
  }
#endif
}

