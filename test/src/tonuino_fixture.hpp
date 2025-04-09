#ifndef SRC_TONUINO_FIXTURE_HPP_
#define SRC_TONUINO_FIXTURE_HPP_

#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>
#include <chip_card.hpp>
#include <commands.hpp>

class tonuino_fixture: public ::testing::Test {
public:
  tonuino_fixture()
  : initializer{}
  , tonuino{Tonuino::getTonuino()}
  {
    getSettings().resetSettings();
    tonuino.resetActiveModifier();
#ifdef BUTTONS3X3
    pin_value[button3x3Pin] = Buttons3x3::maxLevel;
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
  Modifier&  getModifier() { return tonuino.getActiveModifier(); }

  uint8_t    getVolume  () { return *getMp3().volume; }

  Initializer initializer;

  Tonuino& tonuino;

  const unsigned long time_check_play = 1000; // 1 second

  void execute_cycle() {
    tonuino.loop();
  }
  void execute_cycle_for_ms(unsigned long ms) {
    for (unsigned long i = 0; i < ms; i += cycleTime)
      tonuino.loop();
  }

  void button_for_raw_command(commandRaw c) {
    switch(c) {

    case commandRaw::allLong:
      press_button(buttonPausePin);
      press_button(buttonUpPin);
      press_button(buttonDownPin);
      execute_cycle();
      execute_cycle_for_ms(buttonLongPress);
      release_button(buttonPausePin);
      release_button(buttonUpPin);
      release_button(buttonDownPin);
      execute_cycle();
      break;

    case commandRaw::updownLong:
      press_button(buttonUpPin);
      press_button(buttonDownPin);
      execute_cycle();
      execute_cycle_for_ms(buttonLongPress);
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
      execute_cycle_for_ms(buttonLongPress);
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
      execute_cycle_for_ms(buttonLongPress);
      release_button(buttonUpPin);
      execute_cycle();
      break;

    case commandRaw::upLongRepeat:
      press_button(buttonUpPin);
      execute_cycle();
      execute_cycle_for_ms(buttonLongPress);
      execute_cycle_for_ms(buttonLongPress);
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
      execute_cycle_for_ms(buttonLongPress);
      release_button(buttonDownPin);
      execute_cycle();
      break;

    case commandRaw::downLongRepeat:
      press_button(buttonDownPin);
      execute_cycle();
      execute_cycle_for_ms(buttonLongPress);
      execute_cycle_for_ms(buttonLongPress);
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
      execute_cycle_for_ms(buttonLongPress);
      release_button(buttonFourPin);
      execute_cycle();
      break;

    case commandRaw::fourLongRepeat:
      press_button(buttonFourPin);
      execute_cycle();
      execute_cycle_for_ms(buttonLongPress);
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
      execute_cycle_for_ms(buttonLongPress);
      release_button(buttonFivePin);
      execute_cycle();
      break;

    case commandRaw::fiveLongRepeat:
      press_button(buttonFivePin);
      execute_cycle();
      execute_cycle_for_ms(buttonLongPress);
      execute_cycle_for_ms(buttonLongPress);
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
    if (SM_tonuino::is_in_state<StartPlay<Play>>()) {
      execute_cycle(); // start pling
      getMp3().end_track();
      execute_cycle(); // start timer
      current_time += dfPlayer_timeUntilStarts+1;
      execute_cycle();
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
      execute_cycle(); // maybe we start a sub state machine right now
      button_for_command(command::adm_end, state_for_command::admin);
      int counter = 10;
      while (not SM_tonuino::is_in_state<Idle>() && --counter) {
        getMp3().end_track();
        execute_cycle();
      }
      EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
      counter = 10;
      while (not getMp3().is_stopped() && --counter) {
        getMp3().end_track();
        execute_cycle();
      }
      EXPECT_TRUE(getMp3().is_stopped());
    }
  }

  void goto_play(const folderSettings& card, uint16_t track_count = 99) {
    goto_idle();
    card_in(card, track_count);
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay<Play>>());

    // play t_262_pling
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay<Play>>());
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

    // end t_262_pling
    getMp3().end_track();
    execute_cycle();
    execute_cycle_for_ms(dfPlayer_timeUntilStarts);
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

    // play 1-1
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, card.folder);
    //EXPECT_EQ(getMp3().df_folder_track, 1); // TODO

    getMFRC522().card_out();
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  }
  void goto_pause(const folderSettings& card) {
    goto_play(card);

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

  void card_in(uint32_t cookie, uint8_t version, uint8_t folder, uint8_t mode, uint8_t special, uint8_t special2, uint16_t track_count = 99) {
    // set sd-card and insert card
    getMp3().set_folder_track_count(folder, track_count);
    getMFRC522().card_in(cookie, version, folder, mode, special, special2);
    execute_cycle();
  }

  void card_in(const folderSettings& card, uint16_t track_count = 99) {
    card_in(cardCookie, cardVersion, card.folder, static_cast<uint8_t>(card.mode), card.special, card.special2, track_count);
  }

  void card_out() {
    getMFRC522().card_out();
    // three cycles for card removed
    execute_cycle();
    execute_cycle();
    execute_cycle();
  }
  folderSettings card_decode() {
    folderSettings card;
    uint32_t cookie;
    uint8_t  version;
    uint8_t  mode;
    getMFRC522().card_decode(cookie, version, card.folder, mode, card.special, card.special2);
    card.mode = static_cast<pmode_t>(mode);
    if (cookie == cardCookie && version == cardVersion)
      return card;
    else
      return folderSettings{};
  }

  void leave_start_play() {
    // play t_262_pling
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay<Play>>());
    EXPECT_TRUE(getMp3().is_playing_mp3());
    EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

    // end t_262_pling
    getMp3().end_track();
    execute_cycle();
    execute_cycle_for_ms(dfPlayer_timeUntilStarts);
    execute_cycle();
    EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  }
#ifdef BUTTONS3X3
  void set_value_for_3x3(uint8_t button) {
    ASSERT_LE(button, buttonExtSC_buttons);
    pin_value[button3x3Pin] = button*Buttons3x3::maxLevel/Buttons3x3::numLevels;
  }
  void reset_value_for_3x3() {
    set_value_for_3x3(Buttons3x3::numLevels);
  }
  static constexpr uint16_t startAddressExtraShortcuts = 156;
#endif
};

#endif // SRC_TONUINO_FIXTURE_HPP_
