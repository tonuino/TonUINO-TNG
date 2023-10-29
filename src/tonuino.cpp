#include "tonuino.hpp"

#include <Arduino.h>
#include <avr/sleep.h>

#include "array.hpp"
#include "chip_card.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "state_machine.hpp"

namespace {

const __FlashStringHelper* str_bis      () { return F(" bis "); }

} // anonymous namespace

void Tonuino::setup() {
#ifdef BUTTONS3X3
#if defined(ALLinONE_Plus) or defined(TonUINO_Every)
  analogReference(INTERNAL2V5);
#endif
#ifdef ALLinONE
  analogReference(INTERNAL2V048);
  analogReadResolution(12);
#endif
#endif

  pinMode(shutdownPin  , OUTPUT);
  digitalWrite(shutdownPin, getLevel(shutdownPinType, level::inactive));

  randomSeed(generateRamdomSeed());

#if defined ALLinONE || defined ALLinONE_Plus || defined SPKONOFF
  pinMode(ampEnablePin, OUTPUT);
  digitalWrite(ampEnablePin, getLevel(ampEnablePinType, level::inactive));
#endif

#if defined ALLinONE || defined ALLinONE_Plus
  pinMode(usbAccessPin, OUTPUT);
  digitalWrite(usbAccessPin, getLevel(usbAccessPinType, level::inactive));
#endif

#ifdef NEO_RING
  ring.init();
  ring.call_on_startup();
#endif

  // load Settings from EEPROM
  settings.loadSettingsFromFlash();

  // DFPlayer Mini initialisieren
  mp3.begin();
  delay(2000);
  mp3.setVolume();
  mp3.setEq(static_cast<DfMp3_Eq>(settings.eq - 1));

  // NFC Leser initialisieren
  chip_card.initCard();

  // RESET --- ALLE DREI KNÖPFE BEIM STARTEN GEDRÜCKT HALTEN -> alle EINSTELLUNGEN werden gelöscht
  if (buttons.isReset()) {
    settings.clearEEPROM();
    settings.loadSettingsFromFlash();
  }

  SM_tonuino::start();
#if defined ALLinONE || defined ALLinONE_Plus || defined SPKONOFF
  digitalWrite(ampEnablePin, getLevel(ampEnablePinType, level::active));
#endif

  // ignore commands, if buttons already pressed during startup
  commands.getCommandRaw();

  // Start Shortcut "at Startup" - e.g. Welcome Sound
  SM_tonuino::dispatch(command_e(commandRaw::start));
}

void Tonuino::loop() {

  unsigned long  start_cycle = millis();
  checkStandby();

  mp3.loop();

  activeModifier->loop();

  SM_tonuino::dispatch(command_e(commands.getCommandRaw()));
  SM_tonuino::dispatch(card_e(chip_card.getCardEvent()));

#ifdef NEO_RING
  if (SM_tonuino::is_in_state<Idle>())
    ring.call_on_idle();
  else if (SM_tonuino::is_in_state<StartPlay>())
    ring.call_on_startPlay();
  else if (SM_tonuino::is_in_state<Play>())
    ring.call_on_play();
  else if (SM_tonuino::is_in_state<Pause>())
    ring.call_on_pause();
  else // admin menu
    ring.call_on_admin();
#endif

  unsigned long  stop_cycle = millis();

  if (stop_cycle-start_cycle < cycleTime)
    delay(cycleTime - (stop_cycle - start_cycle));
}

void Tonuino::playFolder() {
  LOG(play_log, s_debug, F("playFolder"));
  numTracksInFolder = mp3.getFolderTrackCount(myFolder->folder);
  LOG(play_log, s_warning, numTracksInFolder, F(" tr in folder "), myFolder->folder);
  numTracksInFolder = min(numTracksInFolder, 0xffu);
  mp3.clearAllQueue();

  switch (myFolder->mode) {

  case pmode_t::hoerspiel:
    // Hörspielmodus: eine zufällige Datei aus dem Ordner
    myFolder->special = 1;
    myFolder->special2 = numTracksInFolder;
    __attribute__ ((fallthrough));
    /* no break */
  case pmode_t::hoerspiel_vb:
    // Spezialmodus Von-Bin: Hörspiel: eine zufällige Datei aus dem Ordner
    LOG(play_log, s_info, F("Hörspiel"));
    LOG(play_log, s_info, myFolder->special, str_bis(), myFolder->special2);
    mp3.enqueueTrack(myFolder->folder, random(myFolder->special, myFolder->special2 + 1));
    break;

  case pmode_t::album:
    // Album Modus: kompletten Ordner spielen
    myFolder->special = 1;
    myFolder->special2 = numTracksInFolder;
    __attribute__ ((fallthrough));
    /* no break */
  case pmode_t::album_vb:
    // Spezialmodus Von-Bis: Album: alle Dateien zwischen Start und Ende spielen
    LOG(play_log, s_info, F("Album"));
    LOG(play_log, s_info, myFolder->special, str_bis() , myFolder->special2);
    mp3.enqueueTrack(myFolder->folder, myFolder->special, myFolder->special2);
    break;

  case pmode_t::party:
    // Party Modus: Ordner in zufälliger Reihenfolge
    myFolder->special = 1;
    myFolder->special2 = numTracksInFolder;
    __attribute__ ((fallthrough));
    /* no break */
  case pmode_t::party_vb:
    // Spezialmodus Von-Bis: Party Ordner in zufälliger Reihenfolge
    LOG(play_log, s_info, F("Party"));
    LOG(play_log, s_info, myFolder->special, str_bis(), myFolder->special2);
    mp3.enqueueTrack(myFolder->folder, myFolder->special, myFolder->special2);
    mp3.shuffleQueue();
    mp3.setEndless();
    break;

  case pmode_t::einzel:
    // Einzel Modus: eine Datei aus dem Ordner abspielen
    LOG(play_log, s_info, F("Einzel"));
    mp3.enqueueTrack(myFolder->folder, myFolder->special);
    break;

  case pmode_t::hoerbuch:
  case pmode_t::hoerbuch_1:
  {
    // Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken (oder nur eine Datei)
    LOG(play_log, s_info, F("Hörbuch"));
    uint16_t startTrack = settings.readFolderSettingFromFlash(myFolder->folder);
    if ((startTrack == 0) || (startTrack > numTracksInFolder))
      startTrack = 1;
    mp3.enqueueTrack(myFolder->folder, 1, numTracksInFolder, startTrack-1);
  }
    break;

  default:
    break;
  }
}

void Tonuino::playTrackNumber () {
  const uint8_t advertTrack = mp3.getCurrentTrack();
  if (advertTrack != 0)
    mp3.playAdvertisement(advertTrack);
}


// Leider kann das Modul selbst keine Queue abspielen, daher müssen wir selbst die Queue verwalten
void Tonuino::nextTrack(uint8_t tracks, bool fromOnPlayFinished) {
  LOG(play_log, s_info, F("nextTrack"));
  if (activeModifier->handleNext())
    return;
  if (mp3.isPlayingFolder() && (myFolder->mode == pmode_t::hoerbuch || myFolder->mode == pmode_t::hoerbuch_1)) {
    const uint8_t trackToSave = (mp3.getCurrentTrack() < numTracksInFolder) ? mp3.getCurrentTrack()+1 : 1;
    settings.writeFolderSettingToFlash(myFolder->folder, trackToSave);
    if (fromOnPlayFinished && myFolder->mode == pmode_t::hoerbuch_1)
      mp3.clearFolderQueue();
  }
  mp3.playNext(tracks);
}

void Tonuino::previousTrack(uint8_t tracks) {
  LOG(play_log, s_info, F("previousTrack"));
  if (activeModifier->handlePrevious())
    return;
  if (mp3.isPlayingFolder() && (myFolder->mode == pmode_t::hoerbuch || myFolder->mode == pmode_t::hoerbuch_1)) {
    const uint8_t trackToSave = (mp3.getCurrentTrack() > numTracksInFolder) ? mp3.getCurrentTrack()-1 : 1;
    settings.writeFolderSettingToFlash(myFolder->folder, trackToSave);
  }
  mp3.playPrevious(tracks);
}

// Funktionen für den Standby Timer (z.B. über Pololu-Switch oder Mosfet)
void Tonuino::setStandbyTimer() {
  LOG(standby_log, s_info, F("setStandbyTimer"));
  if (settings.standbyTimer != 0 && not standbyTimer.isActive()) {
    standbyTimer.start(settings.standbyTimer * 60 * 1000);
    LOG(standby_log, s_info, F("timer started"));
  }
}

void Tonuino::disableStandbyTimer() {
  LOG(standby_log, s_info, F("disableStandbyTimer"));
  if (settings.standbyTimer != 0) {
    standbyTimer.stop();
    LOG(standby_log, s_info, F("timer stopped"));
  }
}

void Tonuino::checkStandby() {
  if (standbyTimer.isActive() && standbyTimer.isExpired()) {
    shutdown();
  }
}

void Tonuino::shutdown() {
  LOG(standby_log, s_info, F("power off!"));

#ifdef NEO_RING
  ring.call_on_sleep();
#endif

#if defined ALLinONE || defined ALLinONE_Plus || defined SPKONOFF
  digitalWrite(ampEnablePin, getLevel(ampEnablePinType, level::inactive));
  delay(1000);
#endif

  // enter sleep state
  digitalWrite(shutdownPin, getLevel(shutdownPinType, level::active));
  delay(500);

  // http://discourse.voss.earth/t/intenso-s10000-powerbank-automatische-abschaltung-software-only/805
  // powerdown to 27mA (powerbank switches off after 30-60s)
  chip_card.sleepCard();
  mp3.sleep();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();  // Disable interrupts
  sleep_mode();
}

bool Tonuino::specialCard(const nfcTagObject &nfcTag) {
  LOG(card_log, s_debug, F("special card, mode = "), static_cast<uint8_t>(nfcTag.nfcFolderSettings.mode));
  if (activeModifier->getActive() == nfcTag.nfcFolderSettings.mode) {
    resetActiveModifier();
    LOG(card_log, s_info, F("modifier removed"));
    mp3.playAdvertisement(advertTracks::t_261_deactivate_mod_card, false/*olnyIfIsPlaying*/);
    return true;
  }

  switch (nfcTag.nfcFolderSettings.mode) {
  case pmode_t::sleep_timer:  LOG(card_log, s_info, F("act. sleepTimer"));
                              mp3.playAdvertisement(advertTracks::t_302_sleep            , false/*olnyIfIsPlaying*/);
                              activeModifier = &sleepTimer;
                              break;

  case pmode_t::freeze_dance: LOG(card_log, s_info, F("act. freezeDance"));
                              mp3.playAdvertisement(advertTracks::t_300_freeze_into      , false/*olnyIfIsPlaying*/);
                              activeModifier = &freezeDance;
                              break;

  case pmode_t::locked:       LOG(card_log, s_info, F("act. locked"));
                              mp3.playAdvertisement(advertTracks::t_303_locked           , false/*olnyIfIsPlaying*/);
                              activeModifier = &locked;
                              break;

  case pmode_t::toddler:      LOG(card_log, s_info, F("act. toddlerMode"));
                              mp3.playAdvertisement(advertTracks::t_304_buttonslocked    , false/*olnyIfIsPlaying*/);
                              activeModifier = &toddlerMode;
                              break;

  case pmode_t::kindergarden: LOG(card_log, s_info, F("act. kindergardenMode"));
                              mp3.playAdvertisement(advertTracks::t_305_kindergarden     , false/*olnyIfIsPlaying*/);
                              activeModifier = &kindergardenMode;
                              break;

  case pmode_t::repeat_single:LOG(card_log, s_info, F("act. repeatSingleModifier"));
                              mp3.playAdvertisement(advertTracks::t_260_activate_mod_card, false/*olnyIfIsPlaying*/);
                              activeModifier = &repeatSingleModifier;
                              break;

  default:                    return false;
  }
  activeModifier->init(nfcTag.nfcFolderSettings.special);
  return true;
}

// generates a seed for our pseudo random number generator
// adapted from https://rheingoldheavy.com/better-arduino-random-values
uint32_t Tonuino::generateRamdomSeed()
{
  uint32_t seedLongValue = 0;

  for (uint8_t bitShift = 0; bitShift < 32; bitShift++) { // 32 bits in a uint32_t
    uint32_t seedBitValue  = 0;
    for (uint8_t bitSum = 0; bitSum <= 8; bitSum++) {     // 8 samples of analog pin
      seedBitValue += analogRead(openAnalogPin);          // Flip the coin eight times, adding the results together
    }
    delay(1);                                             // Delay a single millisecond to allow the pin to fluctuate
    seedLongValue |= ((seedBitValue & 0x01) << bitShift); // Build a stack of 32 flipped coins
  }
  LOG(init_log, s_debug, F("RamdonSeed: "), seedLongValue);
  return (seedLongValue);
}
