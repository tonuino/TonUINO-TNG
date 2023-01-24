
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>
#include <chip_card.hpp>
#include <commands.hpp>

class tonuino_test_fixture: public ::testing::Test {
public:
  tonuino_test_fixture()
  : initializer{}
  , tonuino{Tonuino::getTonuino()}
  {
    getSettings().resetSettings();
#ifdef BUTTONS3X3
    pin_value[button3x3Pin] = Buttons3x3::levels[Buttons3x3::numLevels];
#endif
    tonuino.setup();
  }

  struct Initializer {
    Initializer() {
      reset_all_pin_values();
    }
  };

  Mp3&       getMp3     () { return tonuino.getMp3     (); }
  Commands&  getCommands() { return tonuino.getCommands(); }
  Settings&  getSettings() { return tonuino.getSettings(); }
  Chip_card& getChipCard() { return tonuino.getChipCard(); }
  MFRC522&   getMFRC522 () { return getChipCard().mfrc522; }

  Initializer initializer;

  Tonuino& tonuino;
  void execute_cycle() {
    tonuino.loop();
  }

  void button_for_raw_command(commandRaw c) {
    switch(c) {

    case commandRaw::allLong:
      press_button(buttonPausePin);
      press_button(buttonUpPin);
      press_button(buttonDownPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonPausePin);
      release_button(buttonUpPin);
      release_button(buttonDownPin);
      execute_cycle();
      break;

    case commandRaw::updownLong:
      press_button(buttonUpPin);
      press_button(buttonDownPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonUpPin);
      release_button(buttonDownPin);
      execute_cycle();
      break;

    case commandRaw::pause:
      press_button(buttonPausePin);
      execute_cycle();
      release_button(buttonPausePin);
      execute_cycle();
      break;

    case commandRaw::pauseLong:
      press_button(buttonPausePin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonPausePin);
      execute_cycle();
      break;

    case commandRaw::up:
      press_button(buttonUpPin);
      execute_cycle();
      release_button(buttonUpPin);
      execute_cycle();
      break;

    case commandRaw::upLong:
      press_button(buttonUpPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonUpPin);
      execute_cycle();
      break;

    case commandRaw::upLongRepeat:
      press_button(buttonUpPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonUpPin);
      execute_cycle();
      break;

    case commandRaw::down:
      press_button(buttonDownPin);
      execute_cycle();
      release_button(buttonDownPin);
      execute_cycle();
      break;

    case commandRaw::downLong:
      press_button(buttonDownPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonDownPin);
      execute_cycle();
      break;

    case commandRaw::downLongRepeat:
      press_button(buttonDownPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonDownPin);
      execute_cycle();
      break;

#ifdef FIVEBUTTONS
    case commandRaw::four:
      press_button(buttonFourPin);
      execute_cycle();
      release_button(buttonFourPin);
      execute_cycle();
      break;

    case commandRaw::fourLong:
      press_button(buttonFourPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonFourPin);
      execute_cycle();
      break;

    case commandRaw::fourLongRepeat:
      press_button(buttonFourPin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonFourPin);
      execute_cycle();
      break;

    case commandRaw::five:
      press_button(buttonFivePin);
      execute_cycle();
      release_button(buttonFivePin);
      execute_cycle();
      break;

    case commandRaw::fiveLong:
      press_button(buttonFivePin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonFivePin);
      execute_cycle();
      break;

    case commandRaw::fiveLongRepeat:
      press_button(buttonFivePin);
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      current_time += buttonLongPress;
      execute_cycle();
      release_button(buttonFivePin);
      execute_cycle();
      break;
#endif
    default: EXPECT_TRUE(false) << "not yet implementred"  ;break;

    }
  }

  void button_for_command(command c, state_for_command s) {
    switch(s) {
    case state_for_command::admin:
      switch(c) {

      case command::adm_end   : button_for_raw_command(commandRaw::pauseLong ); break;
      case command::select    : button_for_raw_command(commandRaw::pause     ); break;
      case command::next      : button_for_raw_command(commandRaw::up        ); break;
      case command::next10    : button_for_raw_command(commandRaw::upLong    ); break;
      case command::previous  : button_for_raw_command(commandRaw::down      ); break;
      case command::previous10: button_for_raw_command(commandRaw::downLong  ); break;
      default                 : EXPECT_TRUE(false) << "not yet implementred"  ;break;
      }
      break;

    case state_for_command::idle_pause:
      switch(c) {

      case command::admin    : button_for_raw_command(commandRaw::allLong   ); break;
      case command::pause    : button_for_raw_command(commandRaw::pause     ); break;
      case command::shutdown : button_for_raw_command(commandRaw::pauseLong ); break;
      case command::shortcut1: button_for_raw_command(commandRaw::updownLong); break;
      case command::shortcut2: button_for_raw_command(commandRaw::upLong    ); break;
      case command::shortcut3: button_for_raw_command(commandRaw::downLong  ); break;
      default                : EXPECT_TRUE(false) << "not yet implementred"  ;break;
      }
      break;

      case state_for_command::play:
        switch(c) {

        case command::admin : button_for_raw_command(commandRaw::allLong  ); break;
        case command::pause : button_for_raw_command(commandRaw::pause    ); break;
        case command::track : button_for_raw_command(commandRaw::pauseLong); break;
        case command::next  :
  #ifdef FIVEBUTTONS
                              button_for_raw_command(commandRaw::up       ); break;
  #else
          if (getSettings().invertVolumeButtons)
                              button_for_raw_command(commandRaw::upLong   );
          else
                              button_for_raw_command(commandRaw::up       );
                                                                             break;
  #endif

        case command::previous:
  #ifdef FIVEBUTTONS
                              button_for_raw_command(commandRaw::down     ); break;
  #else
          if (getSettings().invertVolumeButtons)
                              button_for_raw_command(commandRaw::downLong );
          else
                              button_for_raw_command(commandRaw::down     );
                                                                             break;
  #endif

  #ifdef FIVEBUTTONS
        case command::next10    :button_for_raw_command(commandRaw::upLong   ); break;
        case command::previous10:button_for_raw_command(commandRaw::downLong ); break;
  #endif

        case command::volume_up:
  #ifdef FIVEBUTTONS
                              button_for_raw_command(commandRaw::four     ); break;
  #else
          if (getSettings().invertVolumeButtons)
                              button_for_raw_command(commandRaw::up       );
          else
                              button_for_raw_command(commandRaw::upLong   );
                                                                              break;
  #endif

        case command::volume_down:
  #ifdef FIVEBUTTONS
                              button_for_raw_command(commandRaw::five     ); break;
  #else
          if (getSettings().invertVolumeButtons)
                              button_for_raw_command(commandRaw::down     );
          else
                              button_for_raw_command(commandRaw::downLong );
                                                                            break;
  #endif

        default:
        EXPECT_TRUE(false) << "not yet implementred";
        break;
        }
        break;
    }
  }

  void goto_idle() {
    if (SM_tonuino::is_in_state<Idle>()) {
      // end start
      getMp3().end_track();
      execute_cycle();
      EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
      EXPECT_TRUE(getMp3().is_stopped());
      return;
    }
    if (SM_tonuino::is_in_state<Pause>()) {
      button_for_command(command::pause, state_for_command::idle_pause);
      EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
      execute_cycle();
    }
    if (SM_tonuino::is_in_state<Play>()) {
      getMp3().endless = false;
      // loop for all tracks
      while (not SM_tonuino::is_in_state<Idle>()) {
        getMp3().end_track();
        execute_cycle();
        execute_cycle();
      }
    }
    else {
      // admin
      button_for_command(command::adm_end, state_for_command::admin);
      int counter = 10;
      while (not SM_tonuino::is_in_state<Idle>() && --counter) {
        getMp3().end_track();
        execute_cycle();
      }
      EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
    }
  }

  uint8_t play_folder = 1;
  uint8_t play_folder_track_count = 20;

  void goto_play() {
    goto_idle();
    card_in();
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    // play t_262_pling
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

    // end t_262_pling
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
    EXPECT_TRUE(getMp3().is_stopped());

    // play 1-1
    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 1);

    getMFRC522().card_out();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  }
  void goto_pause() {
    goto_play();

    // button pause --> pause
    button_for_command(command::pause, state_for_command::play);
    EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());
  }
  void goto_admin() {
    goto_idle();

    // button admin
    button_for_command(command::admin, state_for_command::idle_pause);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
    execute_cycle(); // select_method
    execute_cycle(); // allow
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

    // play t_900_admin
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_900_admin));

    // end t_900_admin
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    EXPECT_TRUE(getMp3().is_stopped());
  }

  void card_in(uint32_t cookie, uint8_t version, uint8_t folder, uint8_t mode, uint8_t special, uint8_t special2) {
    // set sd-card and insert card
    getMp3().set_folder_track_count(folder, play_folder_track_count);
    getMFRC522().card_in(cookie, version, folder, mode, special, special2);
    execute_cycle();
  }

  void card_in() {
    card_in(cardCookie, cardVersion, play_folder, static_cast<uint8_t>(pmode_t::album), 0, 0);
  }

  void card_in(folderSettings card) {
    card_in(cardCookie, cardVersion, card.folder, static_cast<uint8_t>(card.mode), card.special, card.special2);
  }

  void card_out() {
    getMFRC522().card_out();
    // three cycles for card removed
    execute_cycle();
    execute_cycle();
    execute_cycle();
  }

  void leave_start_play() {
    // play t_262_pling
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

    // end t_262_pling
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
    EXPECT_TRUE(getMp3().is_stopped());

    execute_cycle();
  }
};

// =======================================================
// Test initialization
// =======================================================

TEST_F(tonuino_test_fixture, initial_state) {
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_EQ(pin_mode[shutdownPin], OUTPUT);
  EXPECT_EQ(pin_value[shutdownPin], HIGH);

#if defined ALLinONE || defined ALLinONE_Plus
  EXPECT_EQ(pin_mode[ampEnablePin], OUTPUT);
  EXPECT_EQ(pin_value[ampEnablePin], LOW);

  EXPECT_EQ(pin_mode[usbAccessPin], OUTPUT);
  EXPECT_EQ(pin_value[usbAccessPin], LOW);
#endif

  EXPECT_EQ(getSettings().cookie              ,  cardCookie);
  EXPECT_EQ(getSettings().version             , cardVersion);
  EXPECT_EQ(getSettings().maxVolume           ,          25);
  EXPECT_EQ(getSettings().minVolume           ,           5);
  EXPECT_EQ(getSettings().initVolume          ,          15);
  EXPECT_EQ(getSettings().eq                  ,           1);
  EXPECT_EQ(getSettings().locked              ,       false);
  EXPECT_EQ(getSettings().standbyTimer        ,           0);
  EXPECT_EQ(getSettings().invertVolumeButtons ,           1);
  EXPECT_EQ(getSettings().shortCuts[0].folder ,           0);
  EXPECT_EQ(getSettings().shortCuts[1].folder ,           0);
  EXPECT_EQ(getSettings().shortCuts[2].folder ,           0);
  EXPECT_EQ(getSettings().shortCuts[3].folder ,           0);
  EXPECT_EQ(getSettings().adminMenuLocked     ,           0);
  EXPECT_EQ(getSettings().adminMenuPin[0]     ,           1);
  EXPECT_EQ(getSettings().adminMenuPin[1]     ,           1);
  EXPECT_EQ(getSettings().adminMenuPin[2]     ,           1);
  EXPECT_EQ(getSettings().adminMenuPin[3]     ,           1);
  EXPECT_EQ(getSettings().pauseWhenCardRemoved,       false);

  EXPECT_TRUE(getMp3().called_begin);
  EXPECT_EQ(getMp3().current_volume, getSettings().initVolume);
  EXPECT_EQ(getMp3().current_eq    , getSettings().eq-1      );

  EXPECT_TRUE(getMFRC522().called_Init);


  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, loop_takes_cycleTime) {
  unsigned long t = current_time;
  execute_cycle();
  EXPECT_EQ(current_time - t, cycleTime);
}

// =======================================================
// Test sunny day scenarios
// =======================================================

TEST_F(tonuino_test_fixture, sunny_day_play) {
  play_folder = 1;
  play_folder_track_count = 10;

  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  // play start
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));
  // end start
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_stopped());

  Print::clear_output();

  card_in();
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

  leave_start_play();

  // play 1-1
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // button pause --> pause
  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_pause());

  // button pause --> play
  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // loop for all tracks
  for (uint8_t i = 1; i < play_folder_track_count; ++i) {
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, i+1);
  }

  // end --> Idle
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(getMp3().is_stopped());
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());

  card_out();
//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, sunny_day_adm) {

  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  Print::clear_output();
  goto_idle();

  // button admin
  button_for_command(command::admin, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
  execute_cycle(); // select_method
  execute_cycle(); // allow
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

  // play t_900_admin
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_900_admin));

  // end t_900_admin
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  uint16_t start_track = static_cast<uint16_t>(mp3Tracks::t_900_admin)+1;
  uint16_t end_track   = static_cast<uint16_t>(mp3Tracks::t_900_admin)+1+13;
  for (uint16_t track = start_track; track < end_track; ++track) {
    // button up --> play t_900++
    button_for_command(command::next, state_for_command::admin);
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, track);

    // end t_900++
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
    EXPECT_TRUE(getMp3().is_stopped());
  }
  // button down --> play t_912_admin_lock
  button_for_command(command::previous, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_912_admin_lock));

  // end t_912_admin_lock
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button select --> play t_980_admin_lock_intro
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_980_admin_lock_intro));

  // end t_980_admin_lock_intro
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button up --> play t_981_admin_lock_disabled
  button_for_command(command::next, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_981_admin_lock_disabled));

  // end t_981_admin_lock_disabled
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button select --> play t_402_ok_settings
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_LockAdmin>());
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_402_ok_settings));

  // end t_402_ok_settings
  getMp3().end_track();
  execute_cycle();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_919_continue_admin));

  // end t_919_continue_admin
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button up --> play t_913_pause_on_card_removed
  button_for_command(command::next, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_913_pause_on_card_removed));

  // end t_913_pause_on_card_removed
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button admin_end --> play t_802_reset_aborted
  button_for_command(command::adm_end, state_for_command::admin);
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_802_reset_aborted));

  // end t_802_reset_aborted
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_stopped());

}

// =======================================================
// Test Buttons
// =======================================================

// =================== admin (idle, play, pause)
TEST_F(tonuino_test_fixture, admin_in_idle) {

  goto_idle();
  Print::clear_output();

  // button admin
  button_for_command(command::admin, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
  execute_cycle(); // select_method
  execute_cycle(); // allow
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

  goto_idle();
}

TEST_F(tonuino_test_fixture, admin_in_play) {

  goto_play();
  Print::clear_output();

  // button admin
  button_for_command(command::admin, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
  execute_cycle(); // select_method
  execute_cycle(); // allow
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

  goto_idle();
}

TEST_F(tonuino_test_fixture, admin_in_pause) {

  goto_pause();
  Print::clear_output();

  // button admin
  button_for_command(command::admin, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Allow>());
  execute_cycle(); // select_method
  execute_cycle(); // allow
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());

  goto_idle();
}

// =================== shutdown (idle, pause)
TEST_F(tonuino_test_fixture, shutdown_in_idle) {

  goto_idle();
  Print::clear_output();

#if defined ALLinONE || defined ALLinONE_Plus
  pin_value[ampEnablePin] = LOW;
#endif

  pin_value[shutdownPin]            = HIGH;
  getMp3()    .called_sleep         = false;
  getMFRC522().called_AntennaOff    = false;
  getMFRC522().called_SoftPowerDown = false;

  // button shutdown
  button_for_command(command::shutdown, state_for_command::idle_pause);

#if defined ALLinONE || defined ALLinONE_Plus
  EXPECT_EQ(pin_value[ampEnablePin], HIGH);
#endif

  EXPECT_EQ(pin_value[shutdownPin], LOW);
  EXPECT_TRUE(getMp3()    .called_sleep        );
  EXPECT_TRUE(getMFRC522().called_AntennaOff   );
  EXPECT_TRUE(getMFRC522().called_SoftPowerDown);

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, shutdown_in_pause) {

  goto_pause();
  Print::clear_output();

#if defined ALLinONE || defined ALLinONE_Plus
  pin_value[ampEnablePin] = LOW;
#endif

  pin_value[shutdownPin]            = HIGH;
  getMp3()    .called_sleep         = false;
  getMFRC522().called_AntennaOff    = false;
  getMFRC522().called_SoftPowerDown = false;

  // button shutdown
  button_for_command(command::shutdown, state_for_command::idle_pause);

#if defined ALLinONE || defined ALLinONE_Plus
  EXPECT_EQ(pin_value[ampEnablePin], HIGH);
#endif

  EXPECT_EQ(pin_value[shutdownPin], LOW);
  EXPECT_TRUE(getMp3()    .called_sleep        );
  EXPECT_TRUE(getMFRC522().called_AntennaOff   );
  EXPECT_TRUE(getMFRC522().called_SoftPowerDown);

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =================== shortcutx (idle, pause)
TEST_F(tonuino_test_fixture, shortcutx_in_idle) {

  play_folder = 3;
  play_folder_track_count = 15;
  getMp3().set_folder_track_count(play_folder, play_folder_track_count);

  command cmd_list[] = { command::shortcut1, command::shortcut2, command::shortcut3 };

  for (command cmd: cmd_list) {
    goto_idle();
    Print::clear_output();

    uint8_t index = static_cast<uint8_t>(cmd) - static_cast<uint8_t>(command::shortcut1);

    getSettings().shortCuts[index].folder  = play_folder;
    getSettings().shortCuts[index].mode    = pmode_t::einzel;
    getSettings().shortCuts[index].special = 3;

    // button shutdown
    button_for_command(cmd, state_for_command::idle_pause);

    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    leave_start_play();

    // play play_folder-3
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 3);
  }
}

TEST_F(tonuino_test_fixture, shortcutx_in_pause) {

  play_folder = 3;
  play_folder_track_count = 15;
  getMp3().set_folder_track_count(play_folder, play_folder_track_count);

  command cmd_list[] = { command::shortcut1, command::shortcut2, command::shortcut3 };

  for (command cmd: cmd_list) {
    goto_pause();
    Print::clear_output();

    uint8_t index = static_cast<uint8_t>(cmd) - static_cast<uint8_t>(command::shortcut1);

    getSettings().shortCuts[index].folder  = play_folder;
    getSettings().shortCuts[index].mode    = pmode_t::einzel;
    getSettings().shortCuts[index].special = 3;

    // button shutdown
    button_for_command(command::shortcut1, state_for_command::idle_pause);

    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    leave_start_play();

    // play play_folder-3
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 3);

    goto_idle();
  }
}

#ifdef BUTTONS3X3
void set_value_for_3x3(uint8_t button) {
  ASSERT_LE(button, buttonExtSC_buttons);
  pin_value[button3x3Pin] = Buttons3x3::levels[button];
}
void reset_value_for_3x3() {
  set_value_for_3x3(Buttons3x3::numLevels);
}
constexpr uint16_t startAddressExtraShortcuts = 156;

TEST_F(tonuino_test_fixture, shortcut3x3_in_idle) {

  play_folder = 3;
  play_folder_track_count = 99;
  getMp3().set_folder_track_count(play_folder, play_folder_track_count);

  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = index+1;
    folderSettings fs = { play_folder, pmode_t::album_vb, start_track, 99 };
    goto_idle();
    Print::clear_output();

    const int address = startAddressExtraShortcuts + index * sizeof(folderSettings);
    EEPROM_put(address, fs);

    set_value_for_3x3(index);
    execute_cycle();
    reset_value_for_3x3();
    execute_cycle();

    ASSERT_TRUE(SM_tonuino::is_in_state<StartPlay>()) << index;

    leave_start_play();

    // play play_folder-3
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }

  // long press
  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = Buttons3x3::numLevels+index+1;
    folderSettings fs = { play_folder, pmode_t::album_vb, start_track, 99 };
    goto_idle();
    Print::clear_output();

    const int address = startAddressExtraShortcuts + (Buttons3x3::numLevels+index) * sizeof(folderSettings);
    EEPROM_put(address, fs);

    set_value_for_3x3(index);
    execute_cycle();
    current_time += buttonLongPress;
    execute_cycle();
    reset_value_for_3x3();
    execute_cycle();

    ASSERT_TRUE(SM_tonuino::is_in_state<StartPlay>()) << index;

    leave_start_play();

    // play play_folder-3
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, Buttons3x3::numLevels+index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }
}
#endif

// =================== pause (pause, play)
TEST_F(tonuino_test_fixture, pause_in_pause) {
  goto_pause();
  Print::clear_output();

  button_for_command(command::pause, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
}

TEST_F(tonuino_test_fixture, pause_in_play) {
  goto_play();
  Print::clear_output();

  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_pause());
}

// =================== track (play)
TEST_F(tonuino_test_fixture, track_in_play) {
  goto_play();
  Print::clear_output();

  button_for_command(command::track, state_for_command::play);
  EXPECT_TRUE(getMp3().df_playing_adv);
  EXPECT_EQ(getMp3().df_adv_track, getMp3().getCurrentTrack());
}

// =================== volume_up/down (play)
TEST_F(tonuino_test_fixture, volume_up_down_in_play) {
  goto_play();
  Print::clear_output();
  uint8_t volume = getMp3().current_volume;

  for (bool i: {false, true}) {
    getSettings().invertVolumeButtons = i;

    button_for_command(command::volume_up, state_for_command::play);
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
    EXPECT_EQ(getMp3().current_volume, volume+1);

    button_for_command(command::volume_down, state_for_command::play);
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
    EXPECT_EQ(getMp3().current_volume, volume);
  }
}

// =================== next/previous (play)
TEST_F(tonuino_test_fixture, next_previous_in_play) {
  goto_play();
  Print::clear_output();

  for (bool i: {false, true}) {
    getSettings().invertVolumeButtons = i;

    button_for_command(command::next, state_for_command::play);
    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 2);

    button_for_command(command::previous, state_for_command::play);
    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 1);

#ifdef FIVEBUTTONS
    button_for_command(command::next10, state_for_command::play);
    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 11);

    button_for_command(command::previous10, state_for_command::play);
    execute_cycle();
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, play_folder);
    EXPECT_EQ(getMp3().df_folder_track, 1);
#endif
  }
}

// =================== next/previous (play)
TEST_F(tonuino_test_fixture, next_previous_in_admin) {
  goto_admin();
  Print::clear_output();

  uint16_t start_track = static_cast<uint16_t>(mp3Tracks::t_900_admin)+1;

  // button next --> start_track
  button_for_command(command::next, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, start_track);

  // end start_track
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button next10 --> start_track+10
  button_for_command(command::next10, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, start_track+10);

  // end start_track+10
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button previous --> play start_track+9
  button_for_command(command::previous, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, start_track+9);

  // end start_track+9
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button previous10 --> play start_track
  button_for_command(command::previous10, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, start_track);

  // end start_track
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

}
// =================== next/previous (play)
TEST_F(tonuino_test_fixture, select_in_admin) {
  goto_admin();
  Print::clear_output();

  // button next --> t_910_switch_volume
  button_for_command(command::next10, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_910_switch_volume));

  // end t_910_switch_volume
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_Entry>());
  EXPECT_TRUE(getMp3().is_stopped());

  // button select --> play t_933_switch_volume_intro
  button_for_command(command::select, state_for_command::admin);
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_InvButtons>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_933_switch_volume_intro));

  // end t_933_switch_volume_intro
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Admin_InvButtons>());
  EXPECT_TRUE(getMp3().is_stopped());

}

// =================== next/previous (play)
TEST_F(tonuino_test_fixture, adm_end_in_admin) {
  goto_admin();
  Print::clear_output();

  // button admin_end --> play t_802_reset_aborted
  button_for_command(command::adm_end, state_for_command::admin);
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_802_reset_aborted));

  // end t_802_reset_aborted
  getMp3().end_track();
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_TRUE(getMp3().is_stopped());
}

// =======================================================
// Test Pause if card is removed
// =======================================================

// =================== card out in play
TEST_F(tonuino_test_fixture, pause_if_card_removed_works) {
  folderSettings test_data[] = {
      { 1, pmode_t::hoerspiel    , 0, 0 },
      { 1, pmode_t::album        , 0, 0 },
      { 1, pmode_t::party        , 0, 0 },
      { 1, pmode_t::einzel       , 1, 0 },
      { 1, pmode_t::hoerbuch     , 0, 0 },
      { 1, pmode_t::hoerspiel_vb , 1, 3 },
      { 1, pmode_t::album_vb     , 2, 4 },
      { 1, pmode_t::party_vb     , 5, 9 },
      { 1, pmode_t::hoerbuch_1   , 0, 0 },
  };

  play_folder_track_count = 10;

  getSettings().pauseWhenCardRemoved = true;

  int ind = 0;
  for (folderSettings card: test_data) {
    goto_idle();
    Print::clear_output();

    card_in(card);
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    leave_start_play();

    // play 1-1
    ASSERT_TRUE(getMp3().is_playing_folder()) << "Index: " << ind;
    EXPECT_EQ(getMp3().df_folder, card.folder);

    // card out --> pause
    card_out();
    ASSERT_TRUE(SM_tonuino::is_in_state<Pause>()) << "Index: " << ind;
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    // card in --> play
    card_in(card);
    ASSERT_TRUE(SM_tonuino::is_in_state<Play>()) << "Index: " << ind;
    execute_cycle();
    ASSERT_TRUE(getMp3().is_playing_folder()) << "Index: " << ind;
    EXPECT_EQ(getMp3().df_folder, card.folder);

    card_out();
    ++ind;
  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =================== card out early in StartPlay
TEST_F(tonuino_test_fixture, pause_if_card_removed_card_out_early) {
  play_folder = 1;
  play_folder_track_count = 10;

  getSettings().pauseWhenCardRemoved = true;

  goto_idle();
  Print::clear_output();

  card_in();
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

  // card out
  card_out();

  // play t_262_pling
  execute_cycle();
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

  // end t_262_pling
  getMp3().end_track();
  execute_cycle();
  // should go to pause
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  EXPECT_TRUE(getMp3().is_stopped());

  card_in();
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  // play 1-1
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, play_folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  card_out();

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =================== card in with other card
struct card_data_2 {
  folderSettings card1;
  folderSettings card2;
};
TEST_F(tonuino_test_fixture, pause_if_card_removed_card_in_with_other) {
  card_data_2 test_data[] = {
      // folder
      {
          { 1, pmode_t::hoerspiel   , 0, 0 },
          { 2, pmode_t::hoerspiel   , 0, 0 }
      },
      {
          { 1, pmode_t::album       , 0, 0 },
          { 2, pmode_t::album       , 0, 0 }
      },
      {
          { 1, pmode_t::party       , 0, 0 },
          { 2, pmode_t::party       , 0, 0 }
      },
      //... for other modes

      // mode
      {
          { 1, pmode_t::hoerspiel   , 0, 0 },
          { 1, pmode_t::album       , 0, 0 }
      },
      {
          { 1, pmode_t::einzel      , 1, 0 },
          { 1, pmode_t::hoerbuch    , 1, 1 }
      },

      // special
      {
          { 1, pmode_t::einzel      , 1, 0 },
          { 1, pmode_t::einzel      , 2, 0 }
      },
      {
          { 1, pmode_t::hoerspiel_vb, 1, 5 },
          { 1, pmode_t::hoerspiel_vb, 3, 5 }
      },
      {
          { 1, pmode_t::album_vb    , 3, 5 },
          { 1, pmode_t::album_vb    , 4, 5 }
      },
      {
          { 1, pmode_t::party_vb    , 3, 5 },
          { 1, pmode_t::party_vb    , 5, 5 }
      },

      // special2
      {
          { 1, pmode_t::hoerspiel_vb, 1, 2 },
          { 1, pmode_t::hoerspiel_vb, 1, 3 }
      },
      {
          { 1, pmode_t::album_vb    , 1, 3 },
          { 1, pmode_t::album_vb    , 1, 4 }
      },
      {
          { 1, pmode_t::party_vb    , 1, 2 },
          { 1, pmode_t::party_vb    , 1, 5 }
      }
  };

  play_folder_track_count = 10;

  getSettings().pauseWhenCardRemoved = true;

  int ind = 0;
  for (card_data_2 data: test_data) {
    goto_idle();
    Print::clear_output();

    card_in(data.card1);
    ASSERT_TRUE(SM_tonuino::is_in_state<StartPlay>()) << "Index: " << ind;

    leave_start_play();

    // play 1-1
    ASSERT_TRUE(getMp3().is_playing_folder()) << "Index: " << ind;
    EXPECT_EQ(getMp3().df_folder, data.card1.folder);

    // card out --> pause
    card_out();
    EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    // card in other card --> StartPlay
    card_in(data.card2);
    ASSERT_TRUE(SM_tonuino::is_in_state<StartPlay>()) << "Index: " << ind;

    leave_start_play();

    // play 1-1
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, data.card2.folder);

    card_out();
    ++ind;
  }

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}
