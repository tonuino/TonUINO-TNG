#include "tonuino.hpp"

#include <Arduino.h>
#ifndef TonUINO_Esp32
#include <avr/sleep.h>
#else
#include <esp_bt.h>
#include <esp_sleep.h>
#include <esp_bt_main.h>
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#endif

#include "array.hpp"
#include "chip_card.hpp"

#include "constants.hpp"
#include "logger.hpp"
#include "state_machine.hpp"

namespace {

const __FlashStringHelper* str_bis      () { return F(" bis "); }

} // anonymous namespace

#ifdef USE_TIMER
#if defined(ALLinONE_Plus) or defined(TonUINO_Every) or defined(TonUINO_Every_4808)
ISR(TCA0_OVF_vect) {
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
#else // classic with nano, AiO
ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;
#endif

#ifdef ROTARY_ENCODER_USES_TIMER
  RotaryEncoder::timer_loop();
#endif
}
#endif

void Tonuino::setup() {
#ifdef USE_LED_BUTTONS
  ledManager.begin();
  ledManager.setState(ledState::startup);
#endif

#ifdef USE_TIMER
  setup_timer();
#endif

#if defined(BUTTONS3X3) or defined(BAT_VOLTAGE_MEASUREMENT) or defined(POTI)
  setup_adc();
#endif

  buttons.begin();
#ifdef BUTTONS3X3
  buttons3x3.begin();
#endif

  pinMode(shutdownPin  , OUTPUT);
  digitalWrite(shutdownPin, getLevel(shutdownPinType, level::inactive));

  randomSeed(generateRamdomSeed());

  // speaker switch off
#if defined SPKONOFF
  pinMode(ampEnablePin, OUTPUT);
  digitalWrite(ampEnablePin, getLevel(ampEnablePinType, level::inactive));
#endif

#if defined ALLinONE || defined ALLinONE_Plus
  pinMode(usbAccessPin, OUTPUT);
  digitalWrite(usbAccessPin, getLevel(usbAccessPinType, level::inactive));
#endif

#ifdef SPECIAL_START_SHORTCUT
  pinMode(specialStartShortcutPin, INPUT);
#endif

#ifdef BT_MODULE
  pinMode(btModuleOnPin          , OUTPUT);
  pinMode(btModulePairingPin     , OUTPUT);
  digitalWrite(btModuleOnPin     , getLevel(btModuleOnPinType     , level::inactive));
  digitalWrite(btModulePairingPin, getLevel(btModulePairingPinType, level::inactive));
#endif // BT_MODULE

#ifdef ROTARY_ENCODER
  rotaryEncoder.init();
#endif

#ifdef NEO_RING
  ring.init();
  ring.call_on_startup();
#endif

  // load Settings from EEPROM
  settings.init();
  settings.loadSettingsFromFlash();
#ifdef STORE_LAST_CARD
  settings.readExtShortCutFromFlash(lastSortCut, myFolder);
  LOG(init_log, s_debug, F("get last, folder: "), myFolder.folder, F(", mode: "), static_cast<uint8_t>(myFolder.mode));
#endif

#ifdef TonUINO_Esp32
  esp_task_wdt_init(120, true); // increase the default wd timeout
  // init webservice
  webservice.init();
#endif

  // init NFC reader
  chip_card.initCard();

  // RESET flash if all buttons pressed
  if (buttons.isReset()) {
    settings.clearEEPROM();
    settings.loadSettingsFromFlash();
  }

  // init DFPlayer Mini
  mp3.init();

  // init state machine
  SM_tonuino::start();

  // ignore commands, if buttons already pressed during startup
  commands.getCommandRaw();

  // speaker switch on
#if defined SPKONOFF
  digitalWrite(ampEnablePin, getLevel(ampEnablePinType, level::active));
#endif

  // Start Shortcut "at Startup" - e.g. Welcome Sound
#ifdef SPECIAL_START_SHORTCUT

#ifdef TonUINO_Classic
  if (getLevel(specialStartShortcutPinType, (analogRead(specialStartShortcutPin)<512)?0:1) == level::active) {
#else // TonUINO_Classic
  if (getLevel(specialStartShortcutPinType, digitalRead(specialStartShortcutPin)) == level::active) {
#endif // TonUINO_Classic

#ifdef HPJACKDETECT
    mp3.setTempSpkOn();
#endif // HPJACKDETECT
    SM_tonuino::dispatch(command_e(commandRaw::specialStart));
  } else

#endif // SPECIAL_START_SHORTCUT

    // play start track
    SM_tonuino::dispatch(command_e(commandRaw::start));
}

#ifdef USE_TIMER
void Tonuino::setup_timer() {
  // enable timer with 200 Hz (5 ms)
#if defined(ALLinONE_Plus) or defined(TonUINO_Every) or defined(TonUINO_Every_4808)
  //ATMega4809 runs at 16MHz without Main Clock Prescaller and default is TCA_SINGLE_CLKSEL_DIV64_gc
  //4us per tick

  TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm; //enable overflow interrupt
  TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;//timer mode may break pwm

  //5ms = 1250 x .000004  set H and L period registers
  TCA0.SINGLE.PERL = lowByte(1250);
  TCA0.SINGLE.PERH = highByte(1250);

  TCA0.SINGLE.CTRLA  |= TCA_SINGLE_ENABLE_bm;
#else // classic with nano, AiO
  cli();//stop interrupts

  TCCR1A = 0;              // set entire TCCR1A register to 0
  TCCR1B = 0;              // same for TCCR1B
  TCNT1  = 0;              // initialize counter value to 0;
  OCR1A  = 10000U;         // set timer count for frequency = (16*10^6) / (F*8) - 1, F=200Hz
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);   // Set CS11 bit for 8 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  sei();//allow interrupts
#endif
}
#endif // USE_TIMER

void Tonuino::setup_adc() {
#if defined(ALLinONE_Plus) or defined(TonUINO_Every) or defined(TonUINO_Every_4808)
  analogReference(INTERNAL2V5);
#endif
#ifdef ALLinONE
  analogReference(INTERNAL2V048);
  analogReadResolution(12);
#endif
}

void Tonuino::loop() {

  unsigned long  start_cycle = millis();
  checkStandby();

  if (request_shutdown) {
    delay(100);
    shutdown();
    delay(1000);
  }

#ifdef BAT_VOLTAGE_MEASUREMENT
  if (batVoltage.check())
    shutdown();
#endif

#ifdef HPJACKDETECT
  mp3.hpjackdetect();
#endif
  mp3.loop();

  activeModifier->loop();

  SM_tonuino::dispatch(command_e(commands.getCommandRaw()));
  SM_tonuino::dispatch(card_e(chip_card.getCardEvent()));

#ifdef NEO_RING
#ifdef NEO_RING_EXT
  if (mp3.volumeChanged())
    ring.call_on_volume(mp3.getVolumeRel());
  else if (standbyTimer.remainingTime() < 60*1000ul)
    ring.call_before_sleep(standbyTimer.remainingTime() / 1000 * 255 / 60);
  else if (activeModifier->getActive() == pmode_t::sleep_timer)
    ring.call_on_sleep_timer();
  else
#endif // NEO_RING_EXT
  if (SM_tonuino::is_in_state<Idle>())
    ring.call_on_idle();
  else if (SM_tonuino::is_in_state<StartPlay<Play>>()
#ifdef QUIZ_GAME
        || SM_tonuino::is_in_state<StartPlay<Quiz>>()
#endif
#ifdef MEMORY_GAME
        || SM_tonuino::is_in_state<StartPlay<Memory>>()
#endif
          )
    ring.call_on_startPlay();
  else if (SM_tonuino::is_in_state<Play>())
    ring.call_on_play();
  else if (SM_tonuino::is_in_state<Pause>())
    ring.call_on_pause();
#ifdef QUIZ_GAME
  else if (SM_tonuino::is_in_state<Quiz>())
    ring.call_on_game();
#endif // QUIZ_GAME
#ifdef MEMORY_GAME
  else if (SM_tonuino::is_in_state<Memory>())
    ring.call_on_game();
#endif // MEMORY_GAME
  else // admin menu
    ring.call_on_admin();
#endif // NEO_RING

#ifdef USE_LED_BUTTONS
  if (commands.getLastCommand() != command::none)
    Tonuino::getTonuino().getLedManager().handleBlinkOnce();

  if (SM_tonuino::is_in_state<Idle>())
    ledManager.setState(ledState::await_input);
  else if (SM_tonuino::is_in_state<StartPlay<Play>>()
#ifdef QUIZ_GAME
        || SM_tonuino::is_in_state<StartPlay<Quiz>>()
#endif
#ifdef MEMORY_GAME
        || SM_tonuino::is_in_state<StartPlay<Memory>>()
#endif
  )
    ledManager.setState(ledState::startup);
  else if (SM_tonuino::is_in_state<Play>())
    ledManager.setState(ledState::playing);
  else if (SM_tonuino::is_in_state<Pause>())
    ledManager.setState(ledState::paused);
#ifdef QUIZ_GAME
  else if (SM_tonuino::is_in_state<Quiz>())
    ledManager.setState(ledState::playing); // TODO Should be changed to another state.
#endif // QUIZ_GAME
#ifdef MEMORY_GAME
  else if (SM_tonuino::is_in_state<Memory>())
    ledManager.setState(ledState::playing); // TODO should be changed to another state
#endif // MEMORY_GAME
  else // admin menu
    ledManager.setState(ledState::await_input);

  ledManager.update();
#endif // USE_LED_BUTTONS

#ifdef BT_MODULE
  if (btModulePairingTimer.isActive() && btModulePairingTimer.isExpired())
    digitalWrite(btModulePairingPin, getLevel(btModulePairingPinType, level::inactive));
#endif // BT_MODULE

#ifdef TonUINO_Esp32
  webservice.loop();
#endif

  unsigned long  stop_cycle = millis();

  if (stop_cycle-start_cycle < cycleTime)
    delay(cycleTime - (stop_cycle - start_cycle));
}

void Tonuino::playFolder() {
  LOG(play_log, s_debug, F("playFolder"));
  numTracksInFolder = mp3.getFolderTrackCount(myFolder.folder);
  LOG(play_log, s_warning, numTracksInFolder, F(" tr in folder "), myFolder.folder);
  numTracksInFolder = min(numTracksInFolder, static_cast<uint16_t>(0xffu));
  mp3.clearAllQueue();
  if (numTracksInFolder == 0)
    return;

  switch (myFolder.mode) {

  case pmode_t::hoerspiel:
    // Hörspielmodus: eine zufällige Datei aus dem Ordner
    myFolder.special = 1;
    myFolder.special2 = numTracksInFolder;
    __attribute__ ((fallthrough));
    /* no break */
  case pmode_t::hoerspiel_vb:
    // Spezialmodus Von-Bin: Hörspiel: eine zufällige Datei aus dem Ordner
    LOG(play_log, s_debug, F("Hörspiel"));
    LOG(play_log, s_debug, myFolder.special, str_bis(), myFolder.special2);
    mp3.enqueueTrack(myFolder.folder, random(myFolder.special, myFolder.special2 + 1));
    break;

  case pmode_t::album:
    // Album Modus: kompletten Ordner spielen
    myFolder.special = 1;
    myFolder.special2 = numTracksInFolder;
    __attribute__ ((fallthrough));
    /* no break */
  case pmode_t::album_vb:
    // Spezialmodus Von-Bis: Album: alle Dateien zwischen Start und Ende spielen
    LOG(play_log, s_debug, F("Album"));
    LOG(play_log, s_debug, myFolder.special, str_bis() , myFolder.special2);
    mp3.enqueueTrack(myFolder.folder, myFolder.special, myFolder.special2);
    break;

  case pmode_t::party:
    // Party Modus: Ordner in zufälliger Reihenfolge
    myFolder.special = 1;
    myFolder.special2 = numTracksInFolder;
    __attribute__ ((fallthrough));
    /* no break */
  case pmode_t::party_vb:
    // Spezialmodus Von-Bis: Party Ordner in zufälliger Reihenfolge
    LOG(play_log, s_debug, F("Party"));
    LOG(play_log, s_debug, myFolder.special, str_bis(), myFolder.special2);
    mp3.enqueueTrack(myFolder.folder, myFolder.special, myFolder.special2);
    mp3.shuffleQueue();
    mp3.setEndless();
    break;

  case pmode_t::einzel:
    // Einzel Modus: eine Datei aus dem Ordner abspielen
    LOG(play_log, s_debug, F("Einzel"));
    mp3.enqueueTrack(myFolder.folder, myFolder.special);
    break;

  case pmode_t::hoerbuch:
  case pmode_t::hoerbuch_1:
  {
    // Hörbuch Modus: kompletten Ordner spielen und Fortschritt merken (oder nur eine Datei)
    LOG(play_log, s_debug, F("Hörbuch"));
    uint16_t startTrack = settings.readFolderSettingFromFlash(myFolder.folder);
    if ((startTrack == 0) || (startTrack > numTracksInFolder))
      startTrack = 1;
    mp3.enqueueTrack(myFolder.folder, 1, numTracksInFolder, startTrack-1);
  }
    break;

  case pmode_t::quiz_game:
  case pmode_t::memory_game:
    // Nothing to enqueue here
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
  LOG(play_log, s_debug, F("nextTrack"));
  if (fromOnPlayFinished && mp3.isPlayingFolder() && (myFolder.mode == pmode_t::hoerbuch || myFolder.mode == pmode_t::hoerbuch_1)) {
    const uint8_t trackToSave = (mp3.getCurrentTrack() < numTracksInFolder) ? mp3.getCurrentTrack()+1 : 1;
    settings.writeFolderSettingToFlash(myFolder.folder, trackToSave);
    if (myFolder.mode == pmode_t::hoerbuch_1) {
      if (myFolder.special > 0)
        --myFolder.special;
      else
        mp3.clearFolderQueue();
    }
  }
  if (mp3.isPlayingFolder() && activeModifier->handleNext())
    return;
  mp3.playNext(tracks, fromOnPlayFinished);
  if (not fromOnPlayFinished && mp3.isPlayingFolder() && (myFolder.mode == pmode_t::hoerbuch || myFolder.mode == pmode_t::hoerbuch_1)) {
    settings.writeFolderSettingToFlash(myFolder.folder, mp3.getCurrentTrack());
  }
}

void Tonuino::previousTrack(uint8_t tracks) {
  LOG(play_log, s_debug, F("previousTrack"));
  if (activeModifier->handlePrevious())
    return;
  mp3.playPrevious(tracks);
  if (mp3.isPlayingFolder() && (myFolder.mode == pmode_t::hoerbuch || myFolder.mode == pmode_t::hoerbuch_1)) {
    settings.writeFolderSettingToFlash(myFolder.folder, mp3.getCurrentTrack());
  }
}

void Tonuino::jumpToTrack(uint8_t track) {
  LOG(play_log, s_debug, F("jumpToTrack"));
  mp3.jumpTo(track);
  if (mp3.isPlayingFolder() && (myFolder.mode == pmode_t::hoerbuch || myFolder.mode == pmode_t::hoerbuch_1)) {
    settings.writeFolderSettingToFlash(myFolder.folder, mp3.getCurrentTrack());
  }
}

// functions for the standby timer (e.g.. with Pololu-Switch or Mosfet)
void Tonuino::setStandbyTimer() {
  if (isStandbyTimerOff())
    return;
  LOG(standby_log, s_debug, F("setStandbyTimer"));
  if (settings.standbyTimer != 0 && not standbyTimer.isActive()) {
    standbyTimer.start(settings.standbyTimer * 60 * 1000);
    LOG(standby_log, s_info, F("timer started"));
  }
}

void Tonuino::disableStandbyTimer() {
  LOG(standby_log, s_debug, F("disableStandbyTimer"));
  standbyTimer.stop();
  if (settings.standbyTimer != 0) {
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

#ifdef ESP32
  webservice.push_shutdown();
#endif

#ifdef NEO_RING
  ring.call_on_sleep();
#endif

#if defined SPKONOFF
  digitalWrite(ampEnablePin, getLevel(ampEnablePinType, level::inactive));
  delay(1000);
#endif

  // enter sleep state
  digitalWrite(shutdownPin, getLevel(shutdownPinType, level::active));
  delay(500);

#if defined(USE_POLOLU_SHUTDOWN) or defined(USE_TRAEGER_PLATINE_SHUTDOWN)
  return;
#endif

  // http://discourse.voss.earth/t/intenso-s10000-powerbank-automatische-abschaltung-software-only/805
  // powerdown to 27mA (powerbank switches off after 30-60s)
  chip_card.sleepCard();
  mp3.sleep();

#ifndef TonUINO_Esp32
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();  // Disable interrupts
  sleep_mode();
#else
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_deep_sleep_start();
#endif
}

#ifdef BT_MODULE
void Tonuino::switchBtModuleOnOff() {
  btModuleOn = not btModuleOn;
  if (btModuleOn)
    mp3.playAdvertisement(advertTracks::t_320_bt_on , false/*olnyIfIsPlaying*/);
  else
    mp3.playAdvertisement(advertTracks::t_321_bt_off, false/*olnyIfIsPlaying*/);
  digitalWrite(btModuleOnPin, getLevel(btModuleOnPinType, btModuleOn ? level::active : level::inactive));
}

void Tonuino::btModulePairing() {
  if (not btModulePairingTimer.isActive()) {
    mp3.playAdvertisement(advertTracks::t_322_bt_pairing, false/*olnyIfIsPlaying*/);
    btModulePairingTimer.start(btModulePairingPulse);
    digitalWrite(btModulePairingPin, getLevel(btModulePairingPinType, level::active));
  }
}
#endif // BT_MODULE

void Tonuino::switchStandbyTimerOnOff() {
  standbyTimerOff = not standbyTimerOff;
  if (standbyTimerOff) {
    disableStandbyTimer();
    mp3.playAdvertisement(advertTracks::t_323_standby_timer_off , false/*olnyIfIsPlaying*/);
  }
  else {
    if (SM_tonuino::is_in_state<Pause>() || SM_tonuino::is_in_state<Idle>())
      setStandbyTimer();
    mp3.playAdvertisement(advertTracks::t_324_standby_timer_on, false/*olnyIfIsPlaying*/);
  }

}

bool Tonuino::specialCard(const folderSettings &nfcTag) {
  LOG(card_log, s_debug, F("special card, mode = "), static_cast<uint8_t>(nfcTag.mode));
  if (activeModifier->getActive() == nfcTag.mode) {
    resetActiveModifier();
    LOG(card_log, s_info, F("modifier removed"));
    mp3.playAdvertisement(advertTracks::t_261_deactivate_mod_card, false/*olnyIfIsPlaying*/);
    return true;
  }

#ifdef QUIZ_GAME
  if (SM_tonuino::is_in_state<Quiz>() && nfcTag.mode != pmode_t::bt_module && nfcTag.mode != pmode_t::stdb_timer_sw)
    return false;
#endif // QUIZ_GAME
#ifdef MEMORY_GAME
  if (SM_tonuino::is_in_state<Memory>() && nfcTag.mode != pmode_t::bt_module && nfcTag.mode != pmode_t::stdb_timer_sw)
    return false;
#endif // MEMORY_GAME


  switch (nfcTag.mode) {
  case pmode_t::sleep_timer:  LOG(card_log, s_info, F("act. sleepTimer"));
                              mp3.playAdvertisement(advertTracks::t_302_sleep            , false/*olnyIfIsPlaying*/);
                              activeModifier = &sleepTimer;
                              break;

  case pmode_t::freeze_dance: LOG(card_log, s_info, F("act. freezeDance"));
                              mp3.playAdvertisement(advertTracks::t_300_freeze_into      , false/*olnyIfIsPlaying*/);
                              activeModifier = &danceGame;
                              break;

  case pmode_t::fi_wa_ai:     LOG(card_log, s_info, F("act. FeWaLu"));
                              mp3.playAdvertisement(advertTracks::t_303_fi_wa_ai         , false/*olnyIfIsPlaying*/);
                              activeModifier = &danceGame;
                              break;

  case pmode_t::toddler:      LOG(card_log, s_info, F("act. toddlerMode"));
                              mp3.playAdvertisement(advertTracks::t_304_buttonslocked    , false/*olnyIfIsPlaying*/);
                              activeModifier = &toddlerMode;
                              break;

  case pmode_t::kindergarden: LOG(card_log, s_info, F("act. kindergarden"));
                              mp3.playAdvertisement(advertTracks::t_305_kindergarden     , false/*olnyIfIsPlaying*/);
                              activeModifier = &kindergardenMode;
                              break;

  case pmode_t::repeat_single:LOG(card_log, s_info, F("act. repeatSingle"));
                              mp3.playAdvertisement(advertTracks::t_260_activate_mod_card, false/*olnyIfIsPlaying*/);
                              activeModifier = &repeatSingleModifier;
                              break;

#ifdef MODIFICATION_CARD_PAUSE_AFTER_TRACK
  case pmode_t::pause_aft_tr: LOG(card_log, s_info, F("act. pauseAftTr"));
                              mp3.playAdvertisement(advertTracks::t_260_activate_mod_card, false/*olnyIfIsPlaying*/);
                              activeModifier = &pauseAfterTrack;
                              break;
#endif

#ifdef MODIFICATION_CARD_JUKEBOX
  case pmode_t::jukebox:      LOG(card_log, s_info, F("act. jukebox"));
                              mp3.playAdvertisement(advertTracks::t_309_jukebox          , false/*olnyIfIsPlaying*/);
                              activeModifier = &jukeboxModifier;
                              break;
#endif

#ifdef BT_MODULE
  case pmode_t::bt_module:    LOG(card_log, s_info, F("toggle bt module from "), btModuleOn);
                              switchBtModuleOnOff();
                              return true;
#endif // BT_MODULE

  case pmode_t::stdb_timer_sw:LOG(card_log, s_info, F("toggle std timer from "), standbyTimerOff);
                              switchStandbyTimerOnOff();
                              return true;

  default:                    return false;
  }
  activeModifier->init(nfcTag.mode, nfcTag.special);
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
      delay(1);                                           // Delay a single millisecond to allow the pin to fluctuate
    }
    seedLongValue |= ((seedBitValue & 0x01) << bitShift); // Build a stack of 32 flipped coins
  }
  LOG(init_log, s_debug, F("RamdonSeed: "), seedLongValue);
  return (seedLongValue);
}
