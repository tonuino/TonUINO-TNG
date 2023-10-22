
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>
#include <chip_card.hpp>
#include <commands.hpp>

#include "tonuino_fixture.hpp"

class tonuino_test_fixture: public tonuino_fixture {

};

// =======================================================
// Test initialization
// =======================================================

TEST_F(tonuino_test_fixture, initial_state) {
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  EXPECT_EQ(pin_mode[shutdownPin], OUTPUT);

#ifdef TonUINO_Classic
  EXPECT_EQ(pin_value[shutdownPin], LOW);
#endif

#if defined ALLinONE || defined ALLinONE_Plus
  EXPECT_EQ(pin_value[shutdownPin], HIGH);
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
  EXPECT_EQ(getSettings().dummy               ,       false);
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
  EXPECT_EQ(getSettings().pauseWhenCardRemoved,           0);

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
  folderSettings card = { 11, pmode_t::album, 0, 0 };
  uint16_t track_count = 10;

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

  card_in(card, track_count);
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

  leave_start_play();

  // play 1-1
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, card.folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // button pause --> pause
  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_pause());

  // button pause --> play
  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, card.folder);
  EXPECT_EQ(getMp3().df_folder_track, 1);

  // loop for all tracks
  for (uint8_t i = 1; i < track_count; ++i) {
    getMp3().end_track();
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, card.folder);
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

  goto_play({ 1, pmode_t::album, 0, 0 });
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

  goto_pause({ 1, pmode_t::album, 0, 0 });
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
  EXPECT_EQ(pin_value[shutdownPin], LOW);
#endif
#ifdef TonUINO_Classic
  EXPECT_EQ(pin_value[shutdownPin], HIGH);
#endif
  EXPECT_TRUE(getMp3()    .called_sleep        );
  EXPECT_TRUE(getMFRC522().called_AntennaOff   );
  EXPECT_TRUE(getMFRC522().called_SoftPowerDown);

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, shutdown_in_pause) {

  goto_pause({ 2, pmode_t::album, 0, 0 });
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
  EXPECT_EQ(pin_value[shutdownPin], LOW);
#endif
#ifdef TonUINO_Classic
  EXPECT_EQ(pin_value[shutdownPin], HIGH);
#endif
  EXPECT_TRUE(getMp3()    .called_sleep        );
  EXPECT_TRUE(getMFRC522().called_AntennaOff   );
  EXPECT_TRUE(getMFRC522().called_SoftPowerDown);

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =================== shortcutx (idle, pause)
TEST_F(tonuino_test_fixture, shortcutx_in_idle) {

  folderSettings folder_settings = { 10, pmode_t::einzel, 3, 0 };
  uint16_t track_count = 15;

  command cmd_list[] = { command::shortcut1, command::shortcut2, command::shortcut3 };

  for (command cmd: cmd_list) {
    goto_idle();
    Print::clear_output();

    ++folder_settings.folder;
    getMp3().set_folder_track_count(folder_settings.folder, track_count);

    uint8_t index = static_cast<uint8_t>(cmd) - static_cast<uint8_t>(command::shortcut1);

    getSettings().shortCuts[index] = folder_settings;

    // button shortcutx
    button_for_command(cmd, state_for_command::idle_pause);

    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    leave_start_play();

    // play play_folder-3
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, folder_settings.folder);
    EXPECT_EQ(getMp3().df_folder_track, folder_settings.special);
  }
}

TEST_F(tonuino_test_fixture, shortcutx_in_pause) {

  folderSettings folder_settings = { 4, pmode_t::einzel, 3, 0 };
  uint16_t track_count = 14;

  command cmd_list[] = { command::shortcut1, command::shortcut2, command::shortcut3 };

  for (command cmd: cmd_list) {
    goto_pause({ 3, pmode_t::album, 0, 0 });
    Print::clear_output();

    ++folder_settings.folder;
    getMp3().set_folder_track_count(folder_settings.folder, track_count);

    uint8_t index = static_cast<uint8_t>(cmd) - static_cast<uint8_t>(command::shortcut1);

    getSettings().shortCuts[index] = folder_settings;

    // button shortcut
    button_for_command(cmd, state_for_command::idle_pause);

    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    leave_start_play();

    // play play_folder-3
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, folder_settings.folder);
    EXPECT_EQ(getMp3().df_folder_track, folder_settings.special);

    goto_idle();
  }
}

#ifdef BUTTONS3X3

TEST_F(tonuino_test_fixture, shortcut3x3_in_idle) {

  uint8_t folder = 3;
  uint16_t track_count = 99;
  getMp3().set_folder_track_count(folder, track_count);

  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = index+1;
    folderSettings fs = { folder, pmode_t::album_vb, start_track, 99 };
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
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, fs.folder);
    EXPECT_EQ(getMp3().df_folder_track, index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }

  // long press
  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = Buttons3x3::numLevels+index+1;
    folderSettings fs = { folder, pmode_t::album_vb, start_track, 99 };
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
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, fs.folder);
    EXPECT_EQ(getMp3().df_folder_track, Buttons3x3::numLevels+index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }
}

TEST_F(tonuino_test_fixture, shortcut3x3_in_pause) {
  uint8_t folder = 3;
  uint16_t track_count = 99;
  getMp3().set_folder_track_count(folder, track_count);

  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = index+1;
    folderSettings fs = { folder, pmode_t::album_vb, start_track, 99 };
    goto_pause({ 4, pmode_t::album, 0, 0 });
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
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, fs.folder);
    EXPECT_EQ(getMp3().df_folder_track, index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }

  // long press
  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = Buttons3x3::numLevels+index+1;
    folderSettings fs = { folder, pmode_t::album_vb, start_track, 99 };
    goto_pause({ 4, pmode_t::album, 0, 0 });
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
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, fs.folder);
    EXPECT_EQ(getMp3().df_folder_track, Buttons3x3::numLevels+index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }
}

TEST_F(tonuino_test_fixture, shortcut3x3_in_play) {
  uint8_t folder = 3;
  uint16_t track_count = 99;
  getMp3().set_folder_track_count(folder, track_count);

  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = index+1;
    folderSettings fs = { folder, pmode_t::album_vb, start_track, 99 };
    goto_play({ 4, pmode_t::album, 0, 0 });
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
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, fs.folder);
    EXPECT_EQ(getMp3().df_folder_track, index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }

  // long press
  for (uint8_t index = 0; index < Buttons3x3::numLevels; ++index) {
    uint8_t start_track = Buttons3x3::numLevels+index+1;
    folderSettings fs = { folder, pmode_t::album_vb, start_track, 99 };
    goto_play({ 4, pmode_t::album, 0, 0 });
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
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, fs.folder);
    EXPECT_EQ(getMp3().df_folder_track, Buttons3x3::numLevels+index+1);

    goto_idle();
//    EXPECT_TRUE(false) << "log: " << Print::get_output();
  }
}
#endif

// =================== pause (pause, play)
TEST_F(tonuino_test_fixture, pause_in_pause) {
  goto_pause({ 4, pmode_t::album, 0, 0 });
  Print::clear_output();

  button_for_command(command::pause, state_for_command::idle_pause);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_playing_folder());
}

TEST_F(tonuino_test_fixture, pause_in_play) {
  goto_play({ 2, pmode_t::album, 0, 0 });
  Print::clear_output();

  button_for_command(command::pause, state_for_command::play);
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_pause());
}

// =================== track (play)
TEST_F(tonuino_test_fixture, track_in_play) {
  goto_play({ 3, pmode_t::album, 0, 0 });
  Print::clear_output();

  button_for_command(command::track, state_for_command::play);
  EXPECT_TRUE(getMp3().df_playing_adv);
  EXPECT_EQ(getMp3().df_adv_track, getMp3().getCurrentTrack());
}

// =================== volume_up/down (play)
TEST_F(tonuino_test_fixture, volume_up_down_in_play) {
  goto_play({ 4, pmode_t::album, 0, 0 });
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
  folderSettings card = { 5, pmode_t::album, 0, 0 };
  goto_play(card);
  Print::clear_output();

  for (bool i: {false, true}) {
    getSettings().invertVolumeButtons = i;

    button_for_command(command::next, state_for_command::play);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, card.folder);
    EXPECT_EQ(getMp3().df_folder_track, 2);

    button_for_command(command::previous, state_for_command::play);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, card.folder);
    EXPECT_EQ(getMp3().df_folder_track, 1);

#ifdef FIVEBUTTONS
    button_for_command(command::next10, state_for_command::play);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, card.folder);
    EXPECT_EQ(getMp3().df_folder_track, 11);

    button_for_command(command::previous10, state_for_command::play);
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, card.folder);
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
  execute_cycle_for_ms(time_check_play);
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
  execute_cycle_for_ms(time_check_play);
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
  execute_cycle_for_ms(time_check_play);
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
  execute_cycle_for_ms(time_check_play);
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
  execute_cycle_for_ms(time_check_play);
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
  execute_cycle_for_ms(time_check_play);
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
  execute_cycle_for_ms(time_check_play);
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
// Test settings
// =======================================================

// =================== volume
TEST_F(tonuino_test_fixture, volume_settings) {

  getSettings().maxVolume  = 22;
  getSettings().minVolume  = 11;
  getSettings().initVolume = 13;

  getSettings().writeSettingsToFlash();

  getSettings().maxVolume  = 20;
  getSettings().minVolume  = 10;
  getSettings().initVolume = 12;

  tonuino.setup();

  EXPECT_EQ(getSettings().maxVolume , 22);
  EXPECT_EQ(getSettings().minVolume , 11);
  EXPECT_EQ(getSettings().initVolume, 13);

  EXPECT_EQ(getVolume(), 13);
}

// =================== standbyTimer
TEST_F(tonuino_test_fixture, standbyTimer_settings) {

  getSettings().standbyTimer = 0;
  getSettings().writeSettingsToFlash();
  getSettings().standbyTimer = 5;
  tonuino.setup();
  EXPECT_EQ(getSettings().standbyTimer, 0);

  getSettings().standbyTimer = 60;
  getSettings().writeSettingsToFlash();
  getSettings().standbyTimer = 5;
  tonuino.setup();
  EXPECT_EQ(getSettings().standbyTimer, 60);

}

// =================== adminMenuL
TEST_F(tonuino_test_fixture, adminMenu_settings) {

  Settings::pin_t pin = { 1, 2, 3, 4 };
  getSettings().adminMenuLocked = 2;
  getSettings().adminMenuPin    = pin;
  getSettings().writeSettingsToFlash();
  getSettings().adminMenuLocked = 0;
  getSettings().adminMenuPin = { 4, 3, 2, 1 };
  tonuino.setup();
  EXPECT_EQ(getSettings().adminMenuLocked, 2);
  EXPECT_EQ(getSettings().adminMenuPin, pin);
}

// =================== pauseWhenCardRemoved
TEST_F(tonuino_test_fixture, pauseWhenCardRemoved_settings) {

  getSettings().pauseWhenCardRemoved = 0;
  getSettings().writeSettingsToFlash();
  getSettings().pauseWhenCardRemoved = 1;
  tonuino.setup();
  EXPECT_EQ(getSettings().pauseWhenCardRemoved, 0);

  getSettings().pauseWhenCardRemoved = 1;
  getSettings().writeSettingsToFlash();
  getSettings().pauseWhenCardRemoved = 0;
  tonuino.setup();
  EXPECT_EQ(getSettings().pauseWhenCardRemoved, 1);

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

  uint16_t track_count = 10;

  getSettings().pauseWhenCardRemoved = 1;

  int ind = 0;
  for (folderSettings card: test_data) {
    goto_idle();
    Print::clear_output();

    card_in(card, track_count);
    EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

    leave_start_play();

    // play 1-1
    execute_cycle_for_ms(time_check_play);
    ASSERT_TRUE(getMp3().is_playing_folder()) << "Index: " << ind;
    EXPECT_EQ(getMp3().df_folder, card.folder);

    // card out --> pause
    card_out();
    ASSERT_TRUE(SM_tonuino::is_in_state<Pause>()) << "Index: " << ind;
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    // card in --> play
    card_in(card, track_count);
    ASSERT_TRUE(SM_tonuino::is_in_state<Play>()) << "Index: " << ind;
    execute_cycle_for_ms(time_check_play);
    ASSERT_TRUE(getMp3().is_playing_folder()) << "Index: " << ind;
    EXPECT_EQ(getMp3().df_folder, card.folder);

    card_out();
    ++ind;
  }
//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}

// =================== card out early in StartPlay
TEST_F(tonuino_test_fixture, pause_if_card_removed_card_out_early) {
  folderSettings card = { 1, pmode_t::album        , 0, 0 };
  uint16_t track_count = 10;

  getSettings().pauseWhenCardRemoved = 1;

  goto_idle();
  Print::clear_output();

  card_in(card, track_count);
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());

  // card out
  card_out();

  // play t_262_pling
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(SM_tonuino::is_in_state<StartPlay>());
  EXPECT_TRUE(getMp3().is_playing_mp3());
  EXPECT_EQ(getMp3().df_mp3_track, static_cast<uint16_t>(mp3Tracks::t_262_pling));

  // end t_262_pling
  getMp3().end_track();
  execute_cycle();
  execute_cycle_for_ms(dfPlayer_timeUntilStarts);
  execute_cycle();
  // should go to pause
  EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
  execute_cycle();
  EXPECT_TRUE(getMp3().is_pause());

  card_in(card, track_count);
  EXPECT_TRUE(SM_tonuino::is_in_state<Play>());

  // play 1-1
  execute_cycle_for_ms(time_check_play);
  EXPECT_TRUE(getMp3().is_playing_folder());
  EXPECT_EQ(getMp3().df_folder, card.folder);
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

  uint16_t track_count = 10;

  getSettings().pauseWhenCardRemoved = 1;

  int ind = 0;
  for (card_data_2 data: test_data) {
    goto_idle();
    Print::clear_output();

    card_in(data.card1, track_count);
    ASSERT_TRUE(SM_tonuino::is_in_state<StartPlay>()) << "Index: " << ind;

    leave_start_play();

    // play 1-1
    execute_cycle_for_ms(time_check_play);
    ASSERT_TRUE(getMp3().is_playing_folder()) << "Index: " << ind;
    EXPECT_EQ(getMp3().df_folder, data.card1.folder);

    // card out --> pause
    card_out();
    EXPECT_TRUE(SM_tonuino::is_in_state<Pause>());
    execute_cycle();
    EXPECT_TRUE(getMp3().is_pause());

    // card in other card --> StartPlay
    card_in(data.card2, track_count);
    ASSERT_TRUE(SM_tonuino::is_in_state<StartPlay>()) << "Index: " << ind;

    leave_start_play();

    // play 1-1
    execute_cycle_for_ms(time_check_play);
    EXPECT_TRUE(getMp3().is_playing_folder());
    EXPECT_EQ(getMp3().df_folder, data.card2.folder);

    card_out();
    ++ind;
  }

//  EXPECT_TRUE(false) << "log: " << Print::get_output();
}
