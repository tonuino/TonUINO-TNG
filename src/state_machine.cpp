#include "state_machine.hpp"

#include "tonuino.hpp"
#include "logger.hpp"

class Admin_Entry;
class Idle;
//using Admin_End = Idle;      // use this if you want end admin menu after one setting
using Admin_End = Admin_Entry; // use this if you want continue admin menu after one setting

namespace {

Tonuino        &tonuino   = Tonuino::getTonuino();
Mp3            &mp3       = tonuino.getMp3();
Commands       &commands  = tonuino.getCommands();
Settings       &settings  = tonuino.getSettings();
Chip_card      &chip_card = tonuino.getChipCard();

const __FlashStringHelper* str_ChMode                  () { return F("ChMode") ; }
const __FlashStringHelper* str_ChFolder                () { return F("ChFold") ; }
const __FlashStringHelper* str_ChTrack                 () { return F("ChTr") ; }
const __FlashStringHelper* str_ChFirstTrack            () { return F("ChFTr") ; }
const __FlashStringHelper* str_ChLastTrack             () { return F("ChLTr") ; }
const __FlashStringHelper* str_ChNumAnswer             () { return F("ChNumAnsw") ; }
const __FlashStringHelper* str_ChNumTracks             () { return F("ChNumTr") ; }
const __FlashStringHelper* str_WriteCard               () { return F("WriteC") ; }
const __FlashStringHelper* str_Base                    () { return F("Base") ; }
const __FlashStringHelper* str_Idle                    () { return F("Idle") ; }
const __FlashStringHelper* str_StartPlay               () { return F("StartPlay") ; }
const __FlashStringHelper* str_Play                    () { return F("Play") ; }
const __FlashStringHelper* str_Pause                   () { return F("Pause") ; }
const __FlashStringHelper* str_Quiz                    () { return F("Quiz") ; }
const __FlashStringHelper* str_Memory                  () { return F("Memory") ; }
const __FlashStringHelper* str_Admin_BaseSetting       () { return F("AdmBaseSet") ; }
const __FlashStringHelper* str_Admin_BaseWriteCard     () { return F("AdmBaseWC") ; }
const __FlashStringHelper* str_Admin_Allow             () { return F("AdmAllow") ; }
const __FlashStringHelper* str_Admin_Entry             () { return F("AdmEntry") ; }
const __FlashStringHelper* str_Admin_NewCard           () { return F("AdmNCard") ; }
const __FlashStringHelper* str_Admin_SimpleSetting     () { return F("AdmSimpleSet") ; }
const __FlashStringHelper* str_Admin_ModCard           () { return F("AdmModC") ; }
const __FlashStringHelper* str_Admin_ShortCut          () { return F("AdmShortCut") ; }
const __FlashStringHelper* str_Admin_StandbyTimer      () { return F("AdmStbT") ; }
const __FlashStringHelper* str_Admin_CardsForFolder    () { return F("AdmCardsFFold") ; }
const __FlashStringHelper* str_Admin_InvButtons        () { return F("AdmInvBut") ; }
const __FlashStringHelper* str_Admin_ResetEeprom       () { return F("AdmREeprom") ; }
const __FlashStringHelper* str_Admin_LockAdmin         () { return F("AdmLAdm") ; }
const __FlashStringHelper* str_Admin_PauseIfCardRemoved() { return F("AdmPIfCRem") ; }
//#ifdef MEMORY_GAME
const __FlashStringHelper* str_Admin_MemoryGameCards   () { return F("AdmMemoryGameCards") ; }
//#endif
const __FlashStringHelper* str_VoiceMenu               () { return F("VMenu") ; }
const __FlashStringHelper* str_to                      () { return F(" -> ") ; }
const __FlashStringHelper* str_enter                   () { return F("enter ") ; }
const __FlashStringHelper* str_abort                   () { return F(" abort") ; }

}

// #######################################################

template<SM_type SMT>
bool SM<SMT>::isAbort(command cmd) {
  if (cmd == command::adm_end) {
    mp3.enqueueMp3FolderTrack(mp3Tracks::t_802_reset_aborted);
    LOG(state_log, s_info, F("SM"), str_abort());
    this->template transit<finished_abort>();
    return true;
  }
  return false;
}

// #######################################################

template<SM_type SMT>
void VoiceMenu<SMT>::entry(bool entryPlayAfter) {
  LOG(state_log, s_debug, str_VoiceMenu(), F("::entry() "), static_cast<int>(startMessage));
  if (startMessage != mp3Tracks::t_0)
    mp3.enqueueMp3FolderTrack(startMessage, entryPlayAfter);

  currentValue      = 0;
}

template<SM_type SMT>
void VoiceMenu<SMT>::playCurrentValue() {
  mp3.enqueueMp3FolderTrack(messageOffset + currentValue);
  previewStarted = false;
}

template<SM_type SMT>
void VoiceMenu<SMT>::react(command cmd) {
  if (   currentValue != 0
      && preview
      && not previewStarted
      && not mp3.isPlayingMp3())
  {
    LOG(state_log, s_debug, str_VoiceMenu(), F("::react() start preview "), currentValue);
    if (previewFromFolder == 0)
      mp3.enqueueTrack(currentValue, 1);
    else
      mp3.enqueueTrack(previewFromFolder, currentValue);
    previewStarted = true;
  }

  switch(cmd) {
  case command::next10:
    currentValue = min(currentValue + 10, static_cast<int>(numberOfOptions));
    playCurrentValue();
    break;

  case command::next:
    currentValue = min(currentValue + 1, static_cast<int>(numberOfOptions));
    playCurrentValue();
    break;

  case command::previous10:
    currentValue = max(currentValue - 10, 1);
    playCurrentValue();
    break;

  case command::previous:
    currentValue = max(currentValue - 1, 1);
    playCurrentValue();
    break;

#ifdef SerialInputAsCommand
  case command::menu_jump:
    currentValue = min(max(tonuino.getMenuJump(), static_cast<uint8_t>(1)),numberOfOptions);
    playCurrentValue();
    break;
#endif

  default:
    break;
  }
}

// #######################################################

void ChMode::entry() {
  LOG(state_log, s_info, str_enter(), str_ChMode());

  folder = folderSettings{};

  numberOfOptions   = 14;
  startMessage      = mp3Tracks::t_310_select_mode;
  messageOffset     = mp3Tracks::t_310_select_mode;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();
}

void ChMode::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw == commandRaw::none) {
    LOG(state_log, s_debug, str_ChMode(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    folder.mode = static_cast<pmode_t>(currentValue);
    LOG(state_log, s_debug, str_ChMode(), F(": "), currentValue);
    if (folder.mode == pmode_t::admin) {
      folder.folder = 0;
      folder.mode = pmode_t::admin_card;
      transit<finished>();
      return;
    }
    if (folder.mode == pmode_t::repeat_last || folder.mode == pmode_t::switch_bt) {
      folder.folder = 0xff; // dummy value > 0 to make readCard() returning true
      transit<finished>();
      return;
    }
#ifndef QUIZ_GAME
    if (folder.mode == pmode_t::quiz_game) {
      transit<finished_abort>();
      return;
    }
#endif
#ifndef MEMORY_GAME
    if (folder.mode == pmode_t::memory_game) {
      transit<finished_abort>();
      return;
    }
#endif
    transit<ChFolder>();
    return;
  }
}

// #######################################################

void ChFolder::entry() {
  LOG(state_log, s_info, str_enter(), str_ChFolder());

  numberOfOptions   = 99;
  startMessage      = mp3Tracks::t_301_select_folder;
  messageOffset     = mp3Tracks::t_0;
  preview           = true;
  previewFromFolder = 0;

  VoiceMenu::entry();
}

void ChFolder::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_ChFolder(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    folder.folder = currentValue;
    LOG(state_log, s_debug, str_ChFolder(), F(": "), currentValue);
#ifdef QUIZ_GAME
    if (folder.mode == pmode_t::quiz_game) {
      transit<ChNumAnswer>();
      return;
    }
#endif
    if (folder.mode == pmode_t::einzel) {
      transit<ChTrack>();
      return;
    }
    if (folder.mode == pmode_t::hoerbuch_1) {
      transit<ChNumTracks>();
      return;
    }
    if (  ( folder.mode == pmode_t::hoerspiel_vb)
        ||( folder.mode == pmode_t::album_vb    )
        ||( folder.mode == pmode_t::party_vb    )) {
      transit<ChFirstTrack>();
      return;
    }
    transit<finished>();
    return;
  }
}

// #######################################################

void ChTrack::entry() {
  LOG(state_log, s_info, str_enter(), str_ChTrack());

  numberOfOptions   = mp3.getFolderTrackCount(folder.folder);
  startMessage      = mp3Tracks::t_327_select_file;
  messageOffset     = mp3Tracks::t_0;
  preview           = true;
  previewFromFolder = folder.folder;

  VoiceMenu::entry();
}

void ChTrack::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_ChTrack(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    folder.special = currentValue;
    LOG(state_log, s_debug, str_ChTrack(), F(": "), currentValue);
    transit<finished>();
    return;
  }
}

// #######################################################

void ChFirstTrack::entry() {
  LOG(state_log, s_info, str_enter(), str_ChFirstTrack());

  numberOfOptions   = mp3.getFolderTrackCount(folder.folder);
  startMessage      = mp3Tracks::t_328_select_first_file;
  messageOffset     = mp3Tracks::t_0;
  preview           = true;
  previewFromFolder = folder.folder;

  VoiceMenu::entry();
}

void ChFirstTrack::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_ChFirstTrack(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    folder.special = currentValue;
    LOG(state_log, s_debug, str_ChFirstTrack(), F(": "), currentValue);
    transit<ChLastTrack>();
    return;
  }
}

// #######################################################

void ChLastTrack::entry() {
  LOG(state_log, s_info, str_enter(), str_ChLastTrack());

  numberOfOptions   = mp3.getFolderTrackCount(folder.folder);
  startMessage      = mp3Tracks::t_329_select_last_file;
  messageOffset     = mp3Tracks::t_0;
  preview           = true;
  previewFromFolder = folder.folder;

  VoiceMenu::entry();

  currentValue      = folder.special;
}

void ChLastTrack::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_ChLastTrack(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    folder.special2 = currentValue;
    LOG(state_log, s_debug, str_ChLastTrack(), F(": "), currentValue);
    transit<finished>();
    return;
  }
}

// #######################################################

void ChNumAnswer::entry() {
  LOG(state_log, s_info, str_enter(), str_ChNumAnswer());

  numberOfOptions   = 5;
  startMessage      = mp3Tracks::t_333_num_answer;
  messageOffset     = mp3Tracks::t_333_num_answer;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();

  currentValue      = 0;
}

void ChNumAnswer::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_ChNumAnswer(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    if (currentValue == 5) {
      folder.special  = 0;
      folder.special2 = 1;
    }
    else {
      folder.special  = ((currentValue-1)%2+1)*2;
      folder.special2 = (currentValue-1) / 2;
    }
    LOG(state_log, s_debug, str_ChNumAnswer(), F(": "), currentValue);
    transit<finished>();
    return;
  }
}

// #######################################################

void ChNumTracks::entry() {
  LOG(state_log, s_info, str_enter(), str_ChNumTracks());

  numberOfOptions   = 30;
  startMessage      = mp3Tracks::t_340_num_tracks;
  messageOffset     = mp3Tracks::t_0;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();

  currentValue      = 0;
}

void ChNumTracks::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_ChNumTracks(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    folder.special  = currentValue-1;
    LOG(state_log, s_debug, str_ChNumTracks(), F(": "), currentValue);
    transit<finished>();
    return;
  }
}

// #######################################################

void WriteCard::entry() {
  LOG(state_log, s_info, str_enter(), str_WriteCard());
  current_subState = start_waitCardInserted;
}

void WriteCard::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_WriteCard(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (isAbort(cmd))
    return;

  switch (current_subState) {
  case start_waitCardInserted:
    if (chip_card.isCardRemoved())
      mp3.enqueueMp3FolderTrack(mp3Tracks::t_800_waiting_for_card/*, true*//*playAfter*/); // TODO
    current_subState = run_writeCard;
    break;
  case run_writeCard:
    if (not chip_card.isCardRemoved()) {
      folderSettings newCard;
      newCard = folder;
      if (chip_card.writeCard(newCard))
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_400_ok);
      else
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_401_error);
      timer.start(dfPlayer_timeUntilStarts);
      current_subState = end_writeCard;
    }
    break;
  case end_writeCard:
    if (timer.isExpired() && not mp3.isPlaying())
      current_subState = run_waitCardRemoved;
    break;
  case run_waitCardRemoved:
    if (chip_card.isCardRemoved()) {
      LOG(state_log, s_info, str_WriteCard(), str_to(), F("finished"));
      transit<finished>();
    }
    break;
  default:
    break;
  }
}

// #######################################################

bool Base::readCard() {
  lastCardRead.mode = pmode_t::none;
  switch(chip_card.readCard(lastCardRead)) {
  case Chip_card::readCardEvent::none : return false;

  case Chip_card::readCardEvent::known:
#ifdef BT_MODULE
    if (lastCardRead.mode == pmode_t::switch_bt) {
      tonuino.switchBtModuleOnOff();
      return false;
    }
#endif
    if (lastCardRead.folder == 0) {
      if (lastCardRead.mode == pmode_t::admin_card) {
        LOG(state_log, s_debug, str_Base(), str_to(), str_Admin_Entry());
        Admin_Entry::lastCurrentValue = 0;
        transit<Admin_Entry>();
        return false;
      }

      if (tonuino.specialCard(lastCardRead))
        return false;
    }
    break;

  case Chip_card::readCardEvent::empty:
    LOG(state_log, s_debug, str_Base(), str_to(), str_Admin_NewCard());
    Admin_NewCard::return_to_idle = true;
    mp3.enqueueMp3FolderTrack(mp3Tracks::t_300_new_tag);
    Admin_NewCard::wait_track_finished = true;
    transit<Admin_NewCard>();
    return false;
  }

  if (tonuino.getActiveModifier().handleRFID(lastCardRead))
    return false;

  if (lastCardRead.folder != 0) {
    return true;
  }

  return false;
}

bool Base::handleShortcut(uint8_t shortCut) {
  folderSettings sc_folderSettings = settings.getShortCut(shortCut);
  if (sc_folderSettings.folder != 0 && sc_folderSettings.special != 0xff) {
    LOG(state_log, s_debug, F("shortcut, folder: "), sc_folderSettings.folder, F(", mode: "), static_cast<uint8_t>(sc_folderSettings.mode));
#ifdef BT_MODULE
    if (sc_folderSettings.mode == pmode_t::switch_bt) {
      tonuino.switchBtModuleOnOff();
      return false; // do not end the current play
    }
#endif // BT_MODULE
    if (sc_folderSettings.mode != pmode_t::repeat_last)
      tonuino.setMyFolder(sc_folderSettings, false /*myFolderIsCard*/);
    if (tonuino.getFolder() != 0) {
#ifdef QUIZ_GAME
      if (tonuino.getMyFolder().mode == pmode_t::quiz_game) {
        LOG(state_log, s_debug, str_Base(), str_to(), str_Quiz());
        transit<StartPlay<Quiz>>();
        return true;
      }
#endif // QUIZ_GAME
#ifdef MEMORY_GAME
      if (tonuino.getMyFolder().mode == pmode_t::memory_game) {
        LOG(state_log, s_debug, str_Base(), str_to(), str_Memory());
        transit<StartPlay<Memory>>();
        return true;
      }
#endif // MEMORY_GAME
      LOG(state_log, s_debug, str_Base(), str_to(), str_StartPlay());
      transit<StartPlay<Play>>();
      return true;
    }
  }
  return false;
}

void Base::handleReadCard() {
  if (lastCardRead.mode != pmode_t::repeat_last)
    tonuino.setMyFolder(lastCardRead, true /*myFolderIsCard*/);
  if (tonuino.getFolder() != 0) {
    LOG(state_log, s_debug, F("mode: "), static_cast<int>(tonuino.getMyFolder().mode));
#ifdef QUIZ_GAME
    if (tonuino.getMyFolder().mode == pmode_t::quiz_game) {
      LOG(state_log, s_debug, str_Base(), str_to(), str_Quiz());
      transit<StartPlay<Quiz>>();
      return;
    }
#endif // QUIZ_GAME
#ifdef MEMORY_GAME
      if (tonuino.getMyFolder().mode == pmode_t::memory_game) {
        LOG(state_log, s_debug, str_Base(), str_to(), str_Memory());
        transit<StartPlay<Memory>>();
        return;
      }
#endif // MEMORY_GAME
    LOG(state_log, s_debug, str_Base(), str_to(), str_StartPlay());
    transit<StartPlay<Play>>();
  }
}

bool Base::checkForShortcutAndShutdown(command cmd) {
  uint8_t shortCut = 0xff;
  switch(cmd) {
  case command::shortcut1    : shortCut = 1      ; break;
  case command::shortcut2    : shortCut = 2      ; break;
  case command::shortcut3    : shortCut = 3      ; break;
  case command::start        : shortCut = 4      ; break;
#ifdef TonUINO_Esp32
  case command::card_from_web: shortCut = 0      ; break;
#endif
#ifndef DISABLE_SHUTDOWN_VIA_BUTTON
  case command::shutdown : if (tonuino.getActiveModifier().handleButton(command::shutdown))
                             return false;
                           mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
                           mp3.loop();
                           delay(1000);
                           tonuino.shutdown();
                                               break;
#endif
  default                :                     break;
  }
#ifdef BUTTONS3X3
  if (Commands::isExtButton(cmd))
    shortCut = static_cast<uint8_t>(cmd);
#endif
  if (shortCut != 0xff) {
    if (tonuino.getActiveModifier().handleButton(cmd))
      return false;
    if (handleShortcut(shortCut))
      return true;
    else if (shortCut == 4)
      mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
  }
  return false;
}

#ifdef TonUINO_Esp32
bool Base::checkForWritingCard(command cmd, command_e const &cmd_e) {

  if (cmd == command::write_card_from_web) {
    if (chip_card.isCardRemoved()) {
      SM_writeCard::folder = settings.getShortCut(0);
      SM_writeCard::start();
      writingCard = true;
      return true;
    }
  }
  if (writingCard) {
    SM_writeCard::dispatch(cmd_e);
    if (SM_writeCard::is_in_state<finished_writeCard>() or SM_writeCard::is_in_state<finished_abort_writeCard>()) {
      writingCard = false;
    }
    return true;
  }
  return false;
}
#endif

#ifdef NEO_RING
void Base::handleBrightness(command cmd) {
  switch (cmd) {
  case command::bright_up  : tonuino.getRing().brightness_up  (); break;
  case command::bright_down: tonuino.getRing().brightness_down(); break;
  default                  :                                      break;
  }
}
#endif


// #######################################################

void Idle::entry() {
  LOG(state_log, s_info, str_enter(), str_Idle());
  state_str = str_Idle();
  tonuino.setStandbyTimer();
}

void Idle::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Idle(), F("::react(cmd_e) "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::idle_pause);

  if (tonuino.getActiveModifier().handleButton(cmd))
    return;

  if (checkForShortcutAndShutdown(cmd))
    return;

#ifdef TonUINO_Esp32
  if (checkForWritingCard(cmd, cmd_e))
    return;
#endif

#ifdef NEO_RING
  handleBrightness(cmd);
#endif

  switch (cmd) {
  case command::admin:
    LOG(state_log, s_debug, str_Idle(), str_to(), str_Admin_Allow());
    transit<Admin_Allow>();
    return;
#ifdef REPLAY_ON_PLAY_BUTTON
  case command::pause:
    if (tonuino.getFolder() != 0) {
#ifdef QUIZ_GAME
      if (tonuino.getMyFolder().mode == pmode_t::quiz_game) {
        LOG(state_log, s_debug, str_Base(), str_to(), str_Quiz());
        transit<StartPlay<Quiz>>();
        return;
      }
#endif // QUIZ_GAME
#ifdef MEMORY_GAME
      if (tonuino.getMyFolder().mode == pmode_t::memory_game) {
        LOG(state_log, s_debug, str_Base(), str_to(), str_Memory());
        transit<StartPlay<Memory>>();
        return;
      }
#endif // MEMORY_GAME
      LOG(state_log, s_debug, str_Idle(), str_to(), str_StartPlay());
      transit<StartPlay<Play>>();
      return;
    }
    break;
#endif
#ifdef SPECIAL_START_SHORTCUT
    case command::specialStart:
      tonuino.setMyFolder({specialStartShortcutFolder, pmode_t::einzel, specialStartShortcutTrack, 0}, true /*myFolderIsCard*/);
      LOG(state_log, s_debug, str_Idle(), str_to(), str_StartPlay());
      transit<StartPlay<Play>>();
      break;
#endif
  default:
    break;
  }
}

void Idle::react(card_e const &c_e) {
  if (c_e.card_ev != cardEvent::none) {
    LOG(state_log, s_debug, str_Idle(), F("::react(c) "), static_cast<int>(c_e.card_ev));
  }
#ifdef TonUINO_Esp32
  if (writingCard)
    return;
#endif
  switch (c_e.card_ev) {
  case cardEvent::inserted:
    if (readCard())
      handleReadCard();
    return;
  case cardEvent::removed:
    break;
  default:
    break;
  }
}

// #######################################################

void Play::entry() {
  LOG(state_log, s_info, str_enter(), str_Play());
  state_str = str_Play();
  tonuino.disableStandbyTimer();
  mp3.start();
}

void Play::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Play(), F("::react(cmd_e) "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::play);

  if (tonuino.getActiveModifier().handleButton(cmd))
    return;

  if (checkForShortcutAndShutdown(cmd))
    return;

  switch (cmd) {
  case command::admin:
    if (settings.adminMenuLocked != 1) { // only card is allowed
      LOG(state_log, s_debug, str_Play(), str_to(), str_Admin_Allow());
      transit<Admin_Allow>();
    }
    return;
  case command::pause:
    LOG(state_log, s_debug, F("Pause Taste"));
    LOG(state_log, s_debug, str_Play(), str_to(), str_Pause());
    transit<Pause>();
    return;
  case command::track:
#ifdef BT_MODULE
    if (tonuino.isBtModuleOn())
      tonuino.btModulePairing();
    else
#endif
    tonuino.playTrackNumber();
    break;
  case command::volume_up:
    mp3.increaseVolume();
    break;
  case command::next:
    tonuino.nextTrack();
    break;
  case command::next10:
    tonuino.nextTrack(10);
    break;
  case command::volume_down:
    mp3.decreaseVolume();
    break;
  case command::previous:
    tonuino.previousTrack();
    break;
  case command::previous10:
    tonuino.previousTrack(10);
    break;
  case command::to_first:
    tonuino.previousTrack(0xff);
    break;
  default:
    break;
  }
  if (not mp3.isPlayingFolder()) {
    if (mp3.isPlaying())
      mp3.stop();
    transit<Idle>();
  }
}

void Play::react(card_e const &c_e) {
  if (c_e.card_ev != cardEvent::none) {
    LOG(state_log, s_debug, str_Play(), F("::react(c) "), static_cast<int>(c_e.card_ev));
  }
  switch (c_e.card_ev) {
  case cardEvent::inserted:
    if (readCard()) {
#ifdef DONT_ACCEPT_SAME_RFID_TWICE
      if (not (tonuino.getMyFolder() == lastCardRead))
#endif
        handleReadCard();
    }
    else {
      if (lastCardRead.mode == pmode_t::memory_game) {
        mp3.jumpTo(lastCardRead.special-1);
      }
    }
    return;
  case cardEvent::removed:
    if ((settings.pauseWhenCardRemoved==1) && not tonuino.getActiveModifier().handleButton(command::pause)) {
      transit<Pause>();
      return;
    }
    break;
  default:
    break;
  }
}

// #######################################################

void Pause::entry() {
  LOG(state_log, s_info, str_enter(), str_Pause());
  state_str = str_Pause();
  tonuino.setStandbyTimer();
  mp3.pause();
}

void Pause::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Pause(), F("::react(b) "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::idle_pause);

  if (tonuino.getActiveModifier().handleButton(cmd))
    return;

  if (checkForShortcutAndShutdown(cmd))
    return;

#ifdef TonUINO_Esp32
  if (checkForWritingCard(cmd, cmd_e))
    return;
#endif

#ifdef NEO_RING
  handleBrightness(cmd);
#endif

  switch (cmd) {
  case command::admin:
    if (settings.adminMenuLocked != 1) { // only card is allowed
      LOG(state_log, s_debug, str_Pause(), str_to(), str_Admin_Allow());
      transit<Admin_Allow>();
    }
    return;
  case command::pause:
    if ( (settings.pauseWhenCardRemoved!=1) ||
        ((settings.pauseWhenCardRemoved==1) && not chip_card.isCardRemoved())
       ) {
      LOG(state_log, s_debug, str_Pause(), str_to(), str_Play());
      transit<Play>();
    }
    return;
  default:
    break;
  }
}

void Pause::react(card_e const &c_e) {
  if (c_e.card_ev != cardEvent::none) {
    LOG(state_log, s_debug, str_Pause(), F("::react(c) "), static_cast<int>(c_e.card_ev));
  }
#ifdef TonUINO_Esp32
  if (writingCard)
    return;
#endif
  switch (c_e.card_ev) {
  case cardEvent::inserted:
    if (readCard()) {
      bool resume_on_card = settings.pauseWhenCardRemoved==1 ||
#ifdef RESUME_ON_SAME_RFID
                            true ||
#endif
                            false;

      if (resume_on_card && tonuino.getMyFolder() == lastCardRead && not tonuino.getActiveModifier().handleButton(command::pause)) {
        transit<Play>();
        return;
      }
      handleReadCard();
    }
    else {
      if (lastCardRead.mode == pmode_t::memory_game) {
        mp3.jumpTo(lastCardRead.special-1);
        transit<Play>();
        return;
      }
    }
    return;
  case cardEvent::removed:
    break;
  default:
    break;
  }
}

// #######################################################

template<class P> void StartPlay<P>::entry() {
  LOG(state_log, s_info, str_enter(), str_StartPlay());
  state_str = str_StartPlay();
  mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
  timer.stop();
}

template<class P> void StartPlay<P>::react(command_e const &/*cmd_e*/) {
  if (timer.isActive()) {
    if (timer.isExpired()) {
      LOG(state_log, s_debug, str_StartPlay(), str_to(), str_Play());
      if (is_same_type<P, Play>::value && (settings.pauseWhenCardRemoved==1) && chip_card.isCardRemoved() && tonuino.playingCard())
        transit<Pause>();
      else
        transit<P>();
      return;
    }
  }
  else if (not mp3.isPlayingMp3()) {
    tonuino.playFolder();
    timer.start(dfPlayer_timeUntilStarts);
  }
}

// #######################################################

void Quiz::entry() {
  LOG(state_log, s_info, str_enter(), str_Quiz());
  state_str = str_Quiz();
  tonuino.disableStandbyTimer();
  tonuino.resetActiveModifier();
  numAnswer   = tonuino.getMyFolder().special;
  numSolution = tonuino.getMyFolder().special2;
  if (numAnswer != 0 and numAnswer != 2 and numAnswer != 4) {
    LOG(state_log, s_error, F("numA: "), numAnswer);
    finish();
    return;
  }
  if (numSolution > 1) {
    LOG(state_log, s_error, F("numS: "), numSolution);
    finish();
    return;
  }
  quizState = QuizState::playQuestion;
  numQuestion = tonuino.getNumTracksInFolder()/(numAnswer+numSolution+1);

  a.clear();
  if (numAnswer >= 2) {
    a.push(0);
    a.push(1);
  }
  if (numAnswer == 4) {
    a.push(2);
    a.push(3);
  }

  remainingQuestions = 0;

  timer.start(timeout);

  if (numAnswer == 0)
    mp3.enqueueMp3FolderTrack(mp3Tracks::t_509_quiz_game_buzzer_intro);
  else
    mp3.enqueueMp3FolderTrack(mp3Tracks::t_500_quiz_game_intro);
}

void Quiz::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    timer.start(timeout);
    LOG(state_log, s_debug, str_Quiz(), F("::react(cmd_e) "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::play);

  if (tonuino.getActiveModifier().handleButton(cmd))
    return;

  if (checkForShortcutAndShutdown(cmd))
    return;

  if (quizState == QuizState::playWeiter && not mp3.isPlayingFolder()) {
    mp3.enqueueMp3FolderTrack(mp3Tracks::t_510_quiz_game_continue);
    quizState = QuizState::playQuestion;
  }
  if (quizState == QuizState::playSolution && not mp3.isPlayingMp3()) {
    mp3.enqueueTrack(tonuino.getFolder(), trackQuestion+numAnswer+1);
    quizState = QuizState::playWeiter;
  }

  switch (cmd) {
  case command::admin:
    if (settings.adminMenuLocked != 1) { // only card is allowed
      LOG(state_log, s_debug, str_Quiz(), str_to(), str_Admin_Allow());
      transit<Admin_Allow>();
    }
    return;
  case command::pause:
    LOG(state_log, s_debug, F("Pause Taste"));

    switch (quizState) {
    case QuizState::playQuestion:
    case QuizState::playSolution:
    case QuizState::playWeiter:
      if (remainingQuestions == 0) {
        remainingQuestions = numQuestion;
        r.setAll(0xFF);
      }
      question = random(0, remainingQuestions);
      LOG(state_log, s_debug, F("random: "), question, F(", remain: "), remainingQuestions);
      {
        uint8_t i = 0;
        while (true) {
          if (question == 0) {
            while (not r.getBit(i)) ++i;
            question = i;
            r.clearBit(i);
            LOG(state_log, s_debug, F("question: "), question);
            break;
          }
          if (r.getBit(i++)) {
            --question;
          }
        }
      }
      --remainingQuestions;
      LOG(state_log, s_debug, F("r: "), lf_no);
      for (uint8_t i = 0; i<numQuestion; ++i)
        LOG(state_log, s_debug, r.getBit(i), lf_no);
      LOG(state_log, s_debug, F(" "));

      trackQuestion = question*(numAnswer+numSolution+1)+1;
      a.shuffle();
      mp3.enqueueTrack(tonuino.getFolder(), trackQuestion);
      quizState = QuizState::playAnswer;
      actAnswer = 0xff;
      break;
    case QuizState::playAnswer:
      if (numAnswer == 0) {
        // nothing
      }
      else if (actAnswer == 0) {
        LOG(state_log, s_debug, F("richtig"));
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_501_quiz_game_ok+numSolution*2);
      }
      else {
        LOG(state_log, s_debug, F("falsch"));
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_502_quiz_game_bad+numSolution*2);
      }
      if (numSolution == 1)
        quizState = QuizState::playSolution;
      else
        quizState = QuizState::playQuestion;
      break;
    }
    break;
  case command::track:
    if (quizState == QuizState::playAnswer) {
      mp3.enqueueTrack(tonuino.getFolder(), trackQuestion);
    }
    break;
  case command::volume_up:
    if (quizState == QuizState::playAnswer) {
      if (numAnswer == 0) {
        if ((actAnswer == 0xff) || (actAnswer == 1)) {
          LOG(state_log, s_debug, F("Buzzer vol up"));
          mp3.enqueueMp3FolderTrack(mp3Tracks::t_508_quiz_game_buzzer_volu);
          actAnswer = 1;
        }
      }
      else {
        actAnswer = a.get(0);
        mp3.enqueueTrack(tonuino.getFolder(), trackQuestion+actAnswer+1);
      }
    }
    else {
      mp3.increaseVolume();
    }
    break;
  case command::next:
    if (quizState == QuizState::playAnswer) {
      if (numAnswer == 0) {
        if ((actAnswer == 0xff) || (actAnswer == 2)) {
          LOG(state_log, s_debug, F("Buzzer up"));
          mp3.enqueueMp3FolderTrack(mp3Tracks::t_506_quiz_game_buzzer_up);
          actAnswer = 2;
        }
      }
      else {
        actAnswer = a.get(1);
        mp3.enqueueTrack(tonuino.getFolder(), trackQuestion+actAnswer+1);
      }
    }
    break;
  case command::volume_down:
    if (quizState == QuizState::playAnswer) {
      if (numAnswer == 0) {
        if ((actAnswer == 0xff) || (actAnswer == 3)) {
          LOG(state_log, s_debug, F("Buzzer vol down"));
          mp3.enqueueMp3FolderTrack(mp3Tracks::t_507_quiz_game_buzzer_vold);
          actAnswer = 3;
        }
      }
      else {
        actAnswer = a.get(2%numAnswer);
        mp3.enqueueTrack(tonuino.getFolder(), trackQuestion+actAnswer+1);
      }
    }
    else {
      mp3.decreaseVolume();
    }
    break;
  case command::previous:
    if (quizState == QuizState::playAnswer) {
      if (numAnswer == 0) {
        if ((actAnswer == 0xff) || (actAnswer == 4)) {
          LOG(state_log, s_debug, F("Buzzer down"));
          mp3.enqueueMp3FolderTrack(mp3Tracks::t_505_quiz_game_buzzer_down);
          actAnswer = 4;
        }
      }
      else {
        actAnswer = a.get(3%numAnswer);
        mp3.enqueueTrack(tonuino.getFolder(), trackQuestion+actAnswer+1);
      }
    }
    break;
  case command::to_first:
    finish();
    return;
  default:
    break;
  }
  if (not tonuino.isStandbyTimerOff() && timer.isExpired()) {
    finish();
    return;
  }
}

void Quiz::react(card_e const &c_e) {
  if (c_e.card_ev != cardEvent::none) {
    LOG(state_log, s_debug, str_Quiz(), F("::react(c) "), static_cast<int>(c_e.card_ev));
  }
  switch (c_e.card_ev) {
  case cardEvent::inserted:
    if (readCard()) {
#ifdef DONT_ACCEPT_SAME_RFID_TWICE
      if (not (tonuino.getMyFolder() == lastCardRead))
#endif
        handleReadCard();
    }
    return;
  default:
    break;
  }
}

void Quiz::finish() {
  // todo play end
  if (mp3.isPlaying()) {
    mp3.clearAllQueue();
    mp3.stop();
  }
  transit<Idle>();
}


// #######################################################

void Memory::entry() {
  LOG(state_log, s_info, str_enter(), str_Memory());
  state_str = str_Memory();
  tonuino.disableStandbyTimer();
  tonuino.resetActiveModifier();
  first  = 0;
  second = 0;

  timer.start(timeout);

  mp3.enqueueMp3FolderTrack(mp3Tracks::t_520_memory_game_intro);
}

void Memory::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    timer.start(timeout);
    LOG(state_log, s_debug, str_Memory(), F("::react(cmd_e) "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::play);

  if (tonuino.getActiveModifier().handleButton(cmd))
    return;

  if (checkForShortcutAndShutdown(cmd))
    return;

  switch (cmd) {
  case command::admin:
    if (settings.adminMenuLocked != 1) { // only card is allowed
      LOG(state_log, s_debug, str_Memory(), str_to(), str_Admin_Allow());
      transit<Admin_Allow>();
    }
    return;
  case command::pause:
    LOG(state_log, s_debug, F("Pause Taste"));
    if (first == 0) {
      mp3.enqueueMp3FolderTrack(mp3Tracks::t_523_memory_game_1);
    }
    else if (second == 0) {
      mp3.enqueueMp3FolderTrack(mp3Tracks::t_524_memory_game_2);
    }
    else {
      if (((first+1 == second  ) && (first %2 == 1)) ||
          ((first   == second+1) && (second%2 == 1))   ) {
        // match
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_521_memory_game_ok);
      }
      else {
        // no match
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_522_memory_game_bad);
      }
      first  = 0;
      second = 0;
    }
    break;

  case command::track:
    if (second != 0)
      mp3.enqueueTrack(tonuino.getFolder(), second);
    else if (first != 0)
      mp3.enqueueTrack(tonuino.getFolder(), first);
    else
      mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
    break;

  case command::volume_up:
    mp3.increaseVolume();
    break;
  case command::volume_down:
    mp3.decreaseVolume();
    break;
  case command::to_first:
    finish();
    return;
  default:
    break;

  }

  if (not tonuino.isStandbyTimerOff() && timer.isExpired()) {
    finish();
    return;
  }
}

void Memory::react(card_e const &c_e) {
  if (c_e.card_ev != cardEvent::none) {
    timer.start(timeout);
    LOG(state_log, s_debug, str_Memory(), F("::react(c) "), static_cast<int>(c_e.card_ev));
  }
  switch (c_e.card_ev) {
  case cardEvent::inserted:
    if (readCard()) {
#ifdef DONT_ACCEPT_SAME_RFID_TWICE
      if (not (tonuino.getMyFolder() == lastCardRead))
#endif
        handleReadCard();
    }
    else if (lastCardRead.mode == pmode_t::memory_game && lastCardRead.folder == 0) {
      mp3.enqueueTrack(tonuino.getFolder(), lastCardRead.special);
      if (first == 0) {
        first = lastCardRead.special;
      }
      else if (second == 0) {
        second = lastCardRead.special;
      }
      else {
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
      }
    }
    return;
  default:
    break;
  }
}

void Memory::finish() {
  // todo play end
  if (mp3.isPlaying()) {
    mp3.clearAllQueue();
    mp3.stop();
  }
  transit<Idle>();
}


// #######################################################

void Admin_BaseSetting::saveAndTransit() {
  settings.writeSettingsToFlash();
  mp3.enqueueMp3FolderTrack(mp3Tracks::t_402_ok_settings);
  LOG(state_log, s_debug, str_Admin_BaseSetting(), str_to(), F("Idle or AdmEntry"));
  transit<Admin_End>();
}

bool Amin_BaseWriteCard::handleWriteCard(command_e const &cmd_e, bool return_to_idle) {
  SM_writeCard::dispatch(cmd_e);
  if (SM_writeCard::is_in_state<finished_writeCard>()) {
    LOG(state_log, s_debug, str_Admin_BaseWriteCard(), str_to(), F("Idle or AdmEntry"));
    if (return_to_idle)
      transit<Idle>();
    else
      transit<Admin_End>();
    return true;
  }
  else if (SM_writeCard::is_in_state<finished_abort_writeCard>()) {
    LOG(state_log, s_info, str_Admin_BaseWriteCard(), str_abort());
    transit<finished_abort>();
    return true;
  }
  return false;
}

// #######################################################

void Admin_Allow::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_Allow());
  state_str = str_Admin_Allow();
  current_subState = select_method;
  tonuino.resetActiveModifier();
}

void Admin_Allow::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_Allow(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }

  switch (current_subState) {
  case select_method       :
    switch (settings.adminMenuLocked) {
    case 0 : current_subState = allow;       break;
    case 1 : current_subState = not_allow;   break;
    case 2 : mp3.enqueueMp3FolderTrack(mp3Tracks::t_991_admin_pin);
             pin_number = 0;
             current_subState = get_pin;     break;
//    case 3 : current_subState = start_match; break;
    default: current_subState = not_allow;   break;
    }
    break;
  case get_pin             :
  {
    const uint8_t c = Commands::getButtonCode(cmd_e.cmd_raw);
    if (c != 0) {
      LOG(state_log, s_debug, F("pin: "), c);
      pin[pin_number++] = c;
    }
    if (pin_number == 4) {
      if (pin == settings.adminMenuPin) {
        current_subState = allow;
      }
      else {
        current_subState = not_allow;
      }
    }
    break;
  }
//  case start_match         :
//    av = random(10, 20);
//    bv = random(1, av);
//    mp3.enqueueMp3FolderTrack(mp3Tracks::t_992_admin_calc);
//    timer.start(dfPlayer_timeUntilStarts);
//    current_subState = play_match_intro;
//    break;
//  case play_match_intro    :
//    if (timer.isExpired() && not mp3.isPlaying()) {
//      mp3.enqueueMp3FolderTrack(av);
//      timer.start(dfPlayer_timeUntilStarts);
//      current_subState = play_match_a;
//    }
//    break;
//  case play_match_a        :
//    if (timer.isExpired() && not mp3.isPlaying()) {
//      if (random(1, 3) == 2) {
//        cv = av + bv;
//        mp3.enqueueMp3FolderTrack(mp3Tracks::t_993_admin_calc);
//      } else {
//        cv = av - bv;
//        mp3.enqueueMp3FolderTrack(mp3Tracks::t_994_admin_calc);
//      }
//      timer.start(dfPlayer_timeUntilStarts);
//      LOG(admin_log, s_info, F("Result: "), cv);
//      current_subState = play_match_operation;
//    }
//    break;
//  case play_match_operation:
//    if (timer.isExpired() && not mp3.isPlaying()) {
//      mp3.enqueueMp3FolderTrack(bv);
//      timer.start(dfPlayer_timeUntilStarts);
//      current_subState = play_match_b;
//    }
//    break;
//  case play_match_b        :
//    if (timer.isExpired() && not mp3.isPlaying()) {
//      numberOfOptions   = 255;
//      startMessage      = mp3Tracks::t_0;
//      messageOffset     = mp3Tracks::t_0;
//      preview           = false;
//      previewFromFolder = 0;
//
//      mp3.clearFolderQueue();
//
//      VoiceMenu::entry();
//      current_subState = get_match_c;
//    }
//    break;
//  case get_match_c         :
//    VoiceMenu::react(b);
//    const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);
//    if (Commands::isSelect(cmd) && (currentValue != 0)) {
//      if (current_subState == cv)
//        current_subState = allow;
//      else
//        current_subState = not_allow;
//    }
//    break;
  case allow:
    LOG(state_log, s_debug, str_Admin_Allow(), str_to(), str_Admin_Entry());
    Admin_Entry::lastCurrentValue = 0;
    transit<Admin_Entry>();
    return;
  case not_allow:
    LOG(state_log, s_debug, str_Admin_Allow(), str_abort());
    transit<finished_abort>();
    return;
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (isAbort(cmd))
    return;
}

// #######################################################

void Admin_Entry::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_Entry());
  state_str = str_Admin_Entry();
  tonuino.disableStandbyTimer();
  tonuino.resetActiveModifier();

  numberOfOptions   = 14;
  startMessage      = lastCurrentValue == 0 ? mp3Tracks::t_900_admin : mp3Tracks::t_919_continue_admin;
  messageOffset     = mp3Tracks::t_900_admin;
  preview           = false;
  previewFromFolder = 0;

  if (mp3.isPlayingFolder()) {
    mp3.clearFolderQueue();
    mp3.stop();
  }

  VoiceMenu::entry(startMessage == mp3Tracks::t_919_continue_admin);

  currentValue      = lastCurrentValue;
}

void Admin_Entry::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_Entry(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (not chip_card.isCardRemoved()) {
    if (cmd != command::none)
      mp3.enqueueMp3FolderTrack(mp3Tracks::t_801_remove_card);
    return;
  }

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    lastCurrentValue = currentValue;
    switch (currentValue) {
    case 0:  break;
    case 1:  // create new card
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_NewCard());
             Admin_NewCard::wait_track_finished = false;
             transit<Admin_NewCard>();
             return;
    case 2:  // Maximum Volume
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_SimpleSetting());
             Admin_SimpleSetting::type = Admin_SimpleSetting::maxVolume;
             transit<Admin_SimpleSetting>();
             return;
    case 3:  // Minimum Volume
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_SimpleSetting());
             Admin_SimpleSetting::type = Admin_SimpleSetting::minVolume;
             transit<Admin_SimpleSetting>();
             return;
    case 4:  // Initial Volume
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_SimpleSetting());
             Admin_SimpleSetting::type = Admin_SimpleSetting::initVolume;
             transit<Admin_SimpleSetting>();
             return;
    case 5:  // EQ
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_SimpleSetting());
             Admin_SimpleSetting::type = Admin_SimpleSetting::eq;
             transit<Admin_SimpleSetting>();
             return;
    case 6:  // create modifier card
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_ModCard());
             transit<Admin_ModCard>();
             return;
    case 7:  // shortcut
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_ShortCut());
             transit<Admin_ShortCut>();
             return;
    case 8:  // standby timer
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_StandbyTimer());
             transit<Admin_StandbyTimer>();
             return;
    case 9:  // Create Cards for Folder
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_CardsForFolder());
             transit<Admin_CardsForFolder>();
             return;
    case 10: // Invert Functions for Up/Down Buttons
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_InvButtons());
             transit<Admin_InvButtons>();
             return;
    case 11: // reset EEPROM
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_ResetEeprom());
             transit<Admin_ResetEeprom>();
             return;
    case 12: // lock admin menu
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_LockAdmin());
             transit<Admin_LockAdmin>();
             return;
    case 13: // Pause, wenn Karte entfernt wird
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_PauseIfCardRemoved());
             transit<Admin_PauseIfCardRemoved>();
             return;
    case 14: // Memory Spiel Karten
//#ifdef MEMORY_GAME
             LOG(state_log, s_debug, str_Admin_Entry(), str_to(), str_Admin_MemoryGameCards());
             transit<Admin_MemoryGameCards>();
//#else
//             mp3.enqueueMp3FolderTrack(mp3Tracks::t_262_pling);
//#endif
             return;
    }
  }
}

// #######################################################

void Admin_NewCard::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_NewCard());
  state_str = str_Admin_NewCard();
  if (wait_track_finished) {
    current_subState = wait_track;
    timer.start(dfPlayer_timeUntilStarts);
  }
  else {
    current_subState = start_setupCard;
  }
}

void Admin_NewCard::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_NewCard(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  switch (current_subState) {
  case wait_track:
    if (timer.isExpired() and not mp3.isPlaying())
      current_subState = start_setupCard;
    break;
  case start_setupCard:
    SM_setupCard::start();
    current_subState = run_setupCard;
    break;
  case run_setupCard:
    SM_setupCard::dispatch(cmd_e);
    if (SM_setupCard::is_in_state<finished_setupCard>())
      current_subState = end_setupCard;
    if (SM_setupCard::is_in_state<finished_abort_setupCard>()) {
      LOG(state_log, s_info, str_Admin_NewCard(), str_abort());
      transit<finished_abort>();
      return;
    }
    break;
  case end_setupCard:
    SM_writeCard::folder = SM_setupCard::folder;
    current_subState = start_writeCard;
    break;
  case start_writeCard:
    SM_writeCard::start();
    current_subState = run_writeCard;
    break;
  case run_writeCard:
    if (handleWriteCard(cmd_e, return_to_idle)) {
      return_to_idle = false;
      return;
    }
    break;
  default:
    break;
  }
}

// #######################################################

void Admin_SimpleSetting::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_SimpleSetting(), type);
  state_str = str_Admin_SimpleSetting();

  numberOfOptions   = type == maxVolume  ? 30 - mp3.getMinVolume()                        :
                      type == minVolume  ? mp3.getMaxVolume() - 1                         :
                      type == initVolume ? mp3.getMaxVolume() - mp3.getMinVolume() + 1    :
                      type == eq         ? 6                                              : 0;
  startMessage      = type == maxVolume  ? mp3Tracks::t_930_max_volume_intro              :
                      type == minVolume  ? mp3Tracks::t_931_min_volume_into               :
                      type == initVolume ? mp3Tracks::t_932_init_volume_into              :
                      type == eq         ? mp3Tracks::t_920_eq_intro                      : mp3Tracks::t_0;
  messageOffset     = type == maxVolume  ? static_cast<mp3Tracks>(mp3.getMinVolume())     :
                      type == minVolume  ? mp3Tracks::t_0                                 :
                      type == initVolume ? static_cast<mp3Tracks>(mp3.getMinVolume() - 1) :
                      type == eq         ? mp3Tracks::t_920_eq_intro                      : mp3Tracks::t_0;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();

  currentValue      = type == maxVolume  ? mp3.getMaxVolume()  - mp3.getMinVolume()        :
                      type == minVolume  ? mp3.getMinVolume()                             :
                      type == initVolume ? mp3.getInitVolume() - mp3.getMinVolume() + 1   :
                      type == eq         ? settings.eq                                    : 0;
}

void Admin_SimpleSetting::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_SimpleSetting(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    switch (type) {
    case maxVolume : mp3.getMaxVolume () = currentValue + mp3.getMinVolume()    ; break;
    case minVolume : mp3.getMinVolume () = currentValue                         ; break;
    case initVolume: mp3.getInitVolume() = currentValue + mp3.getMinVolume() - 1; break;
    case eq        : settings.eq = currentValue;
                     mp3.setEq(static_cast<DfMp3_Eq>(settings.eq - 1))          ; break;

    }
    saveAndTransit();
    return;
  }
}

// #######################################################

void Admin_ModCard::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_ModCard());
  state_str = str_Admin_ModCard();

  numberOfOptions   = 10;
  startMessage      = mp3Tracks::t_970_modifier_Intro;
  messageOffset     = mp3Tracks::t_970_modifier_Intro;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();

  current_subState  = get_mode;

  folder.mode     = pmode_t::none;
  folder.folder   = 0;
  folder.special  = 0;
  folder.special2 = 0;
}

void Admin_ModCard::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_ModCard(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (current_subState != start_writeCard && current_subState != run_writeCard)
    VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  switch (current_subState) {
  case get_mode           :
    if (Commands::isSelect(cmd) && (currentValue != 0)) {
      folder.mode = static_cast<pmode_t>(currentValue);

      if (folder.mode == pmode_t::sleep_timer) {
        numberOfOptions   = 4;
        startMessage      = mp3Tracks::t_960_timer_intro;
        messageOffset     = mp3Tracks::t_960_timer_intro;
        VoiceMenu::entry();
        current_subState = get_sleeptime_timer;
      }
      else if (folder.mode == pmode_t::freeze_dance || folder.mode == pmode_t::fi_wa_ai) {
        numberOfOptions   = 3;
        startMessage      = mp3Tracks::t_966_dance_pause_intro;
        messageOffset     = mp3Tracks::t_966_dance_pause_intro;
        VoiceMenu::entry();
        current_subState = get_play_time;
      }
      else {
        mp3.clearMp3Queue();
        current_subState = start_writeCard;
      }
    }
    break;
  case get_sleeptime_timer:
    if (Commands::isSelect(cmd) && (currentValue != 0)) {
      switch (currentValue) {
      case 1:
        folder.special = 5;
        break;
      case 2:
        folder.special = 15;
        break;
      case 3:
        folder.special = 30;
        break;
      case 4:
        folder.special = 60;
        break;
      }
      numberOfOptions   = 2;
      startMessage      = mp3Tracks::t_938_modifier_sleep_mode;
      messageOffset     = mp3Tracks::t_933_switch_volume_intro;
      VoiceMenu::entry();
      current_subState = get_sleeptime_mode;
    }
    break;
  case get_sleeptime_mode :
    if (Commands::isSelect(cmd) && (currentValue != 0)) {
      if (currentValue == 2)
        folder.special += 0x80;
      current_subState = start_writeCard;
    }
    break;
  case get_play_time:
    if (Commands::isSelect(cmd) && (currentValue != 0)) {
      folder.special = currentValue-1;
      current_subState = start_writeCard;
    }
    break;
  case start_writeCard    :
    SM_writeCard::folder = folder;
    SM_writeCard::start();
    current_subState = run_writeCard;
    break;
  case run_writeCard      :
    if (handleWriteCard(cmd_e))
      return;
    break;
  }
}

// #######################################################

void Admin_ShortCut::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_ShortCut());
  state_str = str_Admin_ShortCut();

  numberOfOptions   = 4;
  startMessage      = mp3Tracks::t_940_shortcut_into;
  messageOffset     = mp3Tracks::t_940_shortcut_into;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();

  shortcut = 0;
}

void Admin_ShortCut::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_ShortCut(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (shortcut == 0) {
    if (Commands::isSelect(cmd) && (currentValue != 0)) {
      shortcut = currentValue;
      current_subState = start_setupCard;
    }
#ifdef BUTTONS3X3
    else if (Commands::isExtButton(cmd)) {
      shortcut = static_cast<uint8_t>(cmd);
      current_subState = start_setupCard;
      LOG(state_log, s_info, F("shortcut set"), static_cast<int>(shortcut));
    }
#endif
  }
  else {
    switch (current_subState) {
    case start_setupCard:
      settings.setShortCut(shortcut, folderSettings{0,pmode_t::none,0,0});
      settings.writeSettingsToFlash();
      SM_setupCard::start();
      current_subState = run_setupCard;
      break;
    case run_setupCard:
      SM_setupCard::dispatch(cmd_e);
      if (SM_setupCard::is_in_state<finished_setupCard>())
        current_subState = end_setupCard;
      if (SM_setupCard::is_in_state<finished_abort_setupCard>()) {
        LOG(state_log, s_info, str_Admin_ShortCut(), str_abort());
        transit<finished_abort>();
        return;
      }
      break;
    case end_setupCard:
      settings.setShortCut(shortcut, SM_setupCard::folder);
      saveAndTransit();
      return;
    default:
      break;
    }
    return;
  }
}

// #######################################################

void Admin_StandbyTimer::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_StandbyTimer());
  state_str = str_Admin_StandbyTimer();

  numberOfOptions   = 5;
  startMessage      = mp3Tracks::t_960_timer_intro;
  messageOffset     = mp3Tracks::t_960_timer_intro;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();
}

void Admin_StandbyTimer::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_StandbyTimer(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    switch (currentValue) {
    case 1: settings.standbyTimer =  5; break;
    case 2: settings.standbyTimer = 15; break;
    case 3: settings.standbyTimer = 30; break;
    case 4: settings.standbyTimer = 60; break;
    case 5: settings.standbyTimer =  0; break;
    }
    saveAndTransit();
    return;
  }
}

// #######################################################

void Admin_CardsForFolder::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_CardsForFolder());
  state_str = str_Admin_CardsForFolder();

  folder.mode = pmode_t::einzel;

  current_subState = start_setupCard;
}

void Admin_CardsForFolder::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_CardsForFolder(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (isAbort(cmd))
    return;

  switch (current_subState) {
  case start_setupCard:
    SM_setupCard::current_state_ptr = &tinyfsm::_state_instance< ChFolder >::value;
    SM_setupCard::folder.mode = pmode_t::album_vb;
    SM_setupCard::enter();
    current_subState = run_setupCard;
    break;
  case run_setupCard:
    SM_setupCard::dispatch(cmd_e);
    if (SM_setupCard::is_in_state<finished_setupCard>())
      current_subState = end_setupCard;
    if (SM_setupCard::is_in_state<finished_abort_setupCard>()) {
      LOG(state_log, s_info, str_Admin_CardsForFolder(), str_abort());
      transit<finished_abort>();
      return;
    }
    break;
  case end_setupCard:
    folder.folder = SM_setupCard::folder.folder;
    special       = SM_setupCard::folder.special;
    special2      = SM_setupCard::folder.special2;
    mp3.enqueueMp3FolderTrack(mp3Tracks::t_936_batch_cards_intro);
    timer.start(dfPlayer_timeUntilStarts);
    current_subState = prepare_writeCard;
    break;

  case prepare_writeCard:
    if (timer.isExpired() && not mp3.isPlaying() && chip_card.isCardRemoved()) {
      if (special > special2) {
        LOG(state_log, s_debug, str_Admin_CardsForFolder(), str_to(), str_Idle());
        transit<Admin_End>();
        return;
      }
      folder.special = special;
      mp3.enqueueMp3FolderTrack(special, true/*playAfter*/);
      timer.start(dfPlayer_timeUntilStarts);
      LOG(card_log, s_info, special, F("-te Karte auflegen"));
      current_subState = start_writeCard;
    }
    break;
  case start_writeCard:
    if (timer.isExpired() && not mp3.isPlaying() && chip_card.isCardRemoved()) {
      SM_writeCard::folder = folder;
      SM_writeCard::start();
      current_subState = run_writeCard;
    }
    break;
  case run_writeCard:
    SM_writeCard::dispatch(cmd_e);
    if (SM_writeCard::is_in_state<finished_writeCard>()) {
      ++special;
      current_subState = prepare_writeCard;
    }
    else if (SM_writeCard::is_in_state<finished_abort_writeCard>()) {
      LOG(state_log, s_info, str_Admin_CardsForFolder(), str_abort());
      transit<finished_abort>();
      return;
    }
    break;
  default:
    break;
  }
}

// #######################################################

void Admin_InvButtons::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_InvButtons());
  state_str = str_Admin_InvButtons();

  numberOfOptions   = 2;
  startMessage      = mp3Tracks::t_933_switch_volume_intro;
  messageOffset     = mp3Tracks::t_933_switch_volume_intro;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();
}

void Admin_InvButtons::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_InvButtons(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    switch (currentValue) {
    case 1: settings.invertVolumeButtons = 0; break;
    case 2: settings.invertVolumeButtons = 1; break;
    }
    saveAndTransit();
    return;
  }
}

// #######################################################

void Admin_ResetEeprom::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_ResetEeprom());
  state_str = str_Admin_ResetEeprom();
  settings.clearEEPROM();
  settings.resetSettings();
  mp3.enqueueMp3FolderTrack(mp3Tracks::t_999_reset_ok);
}

void Admin_ResetEeprom::react(command_e const &/*cmd_e*/) {
  LOG(state_log, s_debug, str_Admin_ResetEeprom(), str_to(), str_Idle());
  transit<Admin_End>();
  return;
}

// #######################################################

void Admin_LockAdmin::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_LockAdmin());
  state_str = str_Admin_LockAdmin();

  numberOfOptions   = 3;
  startMessage      = mp3Tracks::t_985_admin_lock_intro;
  messageOffset     = mp3Tracks::t_985_admin_lock_intro;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();

  current_subState = get_mode;
}

void Admin_LockAdmin::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_LockAdmin(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (isAbort(cmd))
    return;

  switch(current_subState) {
  case get_mode:
    VoiceMenu::react(cmd);
    if (Commands::isSelect(cmd) && (currentValue != 0)) {
      settings.adminMenuLocked = currentValue-1;
      if (settings.adminMenuLocked == 2) {
        current_subState = get_pin;
        pin_number = 0;
        mp3.enqueueMp3FolderTrack(mp3Tracks::t_991_admin_pin);
      }
      else
        current_subState = finished;
    }
    break;
  case get_pin             :
  {
    const uint8_t c = Commands::getButtonCode(cmd_e.cmd_raw);
    if (c != 0) {
      LOG(state_log, s_debug, F("pin: "), c);
      pin[pin_number++] = c;
    }
    if (pin_number == 4) {
      settings.adminMenuPin = pin;
      current_subState = finished;
    }
    break;
  }
  case finished:
    saveAndTransit();
    return;
  }
}

// #######################################################

void Admin_PauseIfCardRemoved::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_PauseIfCardRemoved());
  state_str = str_Admin_PauseIfCardRemoved();

  numberOfOptions   = 2;
  startMessage      = mp3Tracks::t_913_pause_on_card_removed;
  messageOffset     = mp3Tracks::t_933_switch_volume_intro;
  preview           = false;
  previewFromFolder = 0;

  VoiceMenu::entry();
}

void Admin_PauseIfCardRemoved::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_PauseIfCardRemoved(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }
  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  VoiceMenu::react(cmd);

  if (isAbort(cmd))
    return;

  if (Commands::isSelect(cmd) && (currentValue != 0)) {
    switch (currentValue) {
    case 1: settings.pauseWhenCardRemoved = 0; break;
    case 2: settings.pauseWhenCardRemoved = 1; break;
    }
    saveAndTransit();
    return;
  }
}

// #######################################################

//#ifdef MEMORY_GAME
void Admin_MemoryGameCards::entry() {
  LOG(state_log, s_info, str_enter(), str_Admin_MemoryGameCards());
  state_str = str_Admin_MemoryGameCards();

  folder.mode     = pmode_t::memory_game;
  folder.special  = 1; // start with card 1
  folder.special2 = 0;

  mp3.enqueueMp3FolderTrack(mp3Tracks::t_937_memory_game_cards_intro);
  timer.start(dfPlayer_timeUntilStarts);

  current_subState = prepare_writeCard;
}

void Admin_MemoryGameCards::react(command_e const &cmd_e) {
  if (cmd_e.cmd_raw != commandRaw::none) {
    LOG(state_log, s_debug, str_Admin_MemoryGameCards(), F("::react() "), static_cast<int>(cmd_e.cmd_raw));
  }

  const command cmd = commands.getCommand(cmd_e.cmd_raw, state_for_command::admin);

  if (isAbort(cmd))
    return;

  switch (cmd) {
  case command::select:
    LOG(state_log, s_debug, str_Admin_MemoryGameCards(), str_to(), str_Idle());
    transit<Admin_End>();
    return;
  case command::next:
    ++folder.special;
    current_subState = prepare_writeCard;
    break;
  case command::next10:
    if (folder.special <= 254-10)
      folder.special += 10;
    else
      folder.special = 254;
    current_subState = prepare_writeCard;
    break;
  case command::previous:
    if (folder.special > 1)
      --folder.special;
    current_subState = prepare_writeCard;
    break;
  case command::previous10:
    if (folder.special >= 1+10)
      folder.special -= 10;
    else
      folder.special = 1;
    current_subState = prepare_writeCard;
    break;
  default:
    break;
  }

  switch (current_subState) {
  case prepare_writeCard:
    if (timer.isExpired() && not mp3.isPlaying() && chip_card.isCardRemoved()) {
      if (folder.special == 255) {
        LOG(state_log, s_debug, str_Admin_MemoryGameCards(), str_to(), str_Idle());
        transit<Admin_End>();
        return;
      }
      mp3.enqueueMp3FolderTrack(folder.special, true/*playAfter*/);
      timer.start(dfPlayer_timeUntilStarts);
      LOG(card_log, s_info, folder.special, F("-te Karte auflegen"));
      current_subState = start_writeCard;
    }
    break;
  case start_writeCard:
    if (timer.isExpired() && not mp3.isPlaying() && chip_card.isCardRemoved()) {
      SM_writeCard::folder = folder;
      SM_writeCard::start();
      current_subState = run_writeCard;
    }
    break;
  case run_writeCard:
    SM_writeCard::dispatch(cmd_e);
    if (SM_writeCard::is_in_state<finished_writeCard>()) {
      ++folder.special;
      current_subState = prepare_writeCard;
    }
    else if (SM_writeCard::is_in_state<finished_abort_writeCard>()) {
      LOG(state_log, s_info, str_Admin_MemoryGameCards(), str_abort());
      transit<finished_abort>();
      return;
    }
    break;
  default:
    break;
  }
}
//#endif

// #######################################################

FSM_INITIAL_STATE(SM_setupCard, ChMode)
FSM_INITIAL_STATE(SM_writeCard, WriteCard)
FSM_INITIAL_STATE(SM_tonuino  , Idle)

template<SM_type SMT>
folderSettings  SM<SMT>::folder{};
template<SM_type SMT>
Timer           SM<SMT>::timer{};
template<SM_type SMT>
bool            SM<SMT>::waitForPlayFinish{};
template<SM_type SMT>
const __FlashStringHelper* SM<SMT>::state_str{};

template<SM_type SMT>
uint8_t   VoiceMenu<SMT>::numberOfOptions  ;
template<SM_type SMT>
mp3Tracks VoiceMenu<SMT>::startMessage     ;
template<SM_type SMT>
mp3Tracks VoiceMenu<SMT>::messageOffset    ;
template<SM_type SMT>
bool      VoiceMenu<SMT>::preview          ;
template<SM_type SMT>
uint8_t   VoiceMenu<SMT>::previewFromFolder;
template<SM_type SMT>
uint8_t   VoiceMenu<SMT>::currentValue     ;

template<SM_type SMT>
bool      VoiceMenu<SMT>::previewStarted   ;

folderSettings Base::lastCardRead{};
uint8_t Admin_Entry::lastCurrentValue{};
Admin_SimpleSetting::Type Admin_SimpleSetting::type{};
bool Admin_NewCard::return_to_idle{false};
bool Admin_NewCard::wait_track_finished{false};
