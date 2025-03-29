
#include <gtest/gtest.h>

#include <commands.hpp>
#include <buttons.hpp>

class buttons_test_fixture: public ::testing::Test {
public:
  buttons_test_fixture()
  : initializer{}
  , settings{}
  , buttons{}
  , commands{settings, &buttons}
  {
    buttons.begin();
  }

  struct Initializer {
    Initializer() {
      reset_all_pin_values();
    }
  };

  commandRaw execute_cycle(unsigned long t = cycleTime) {
    current_time += t;
    return commands.getCommandRaw();
  }

  Initializer initializer;

  Settings settings;
  Buttons  buttons;
  Commands commands;
};

TEST_F(buttons_test_fixture, initial_state) {

  EXPECT_EQ(pin_mode [buttonPausePin], (buttonPinType == levelType::activeLow)?INPUT_PULLUP:INPUT);
  EXPECT_EQ(pin_value[buttonPausePin], (buttonPinType == levelType::activeLow)?HIGH:LOW);

  EXPECT_EQ(pin_mode [buttonUpPin], (buttonPinType == levelType::activeLow)?INPUT_PULLUP:INPUT);
  EXPECT_EQ(pin_value[buttonUpPin], (buttonPinType == levelType::activeLow)?HIGH:LOW);

  EXPECT_EQ(pin_mode [buttonDownPin], (buttonPinType == levelType::activeLow)?INPUT_PULLUP:INPUT);
  EXPECT_EQ(pin_value[buttonDownPin], (buttonPinType == levelType::activeLow)?HIGH:LOW);

#ifdef FIVEBUTTONS
  EXPECT_EQ(pin_mode [buttonFourPin], (buttonPinType == levelType::activeLow)?INPUT_PULLUP:INPUT);
  EXPECT_EQ(pin_value[buttonFourPin], (buttonPinType == levelType::activeLow)?HIGH:LOW);

  EXPECT_EQ(pin_mode [buttonFivePin], (buttonPinType == levelType::activeLow)?INPUT_PULLUP:INPUT);
  EXPECT_EQ(pin_value[buttonFivePin], (buttonPinType == levelType::activeLow)?HIGH:LOW);
#endif


  commandRaw cmd_raw = commands.getCommandRaw();
  EXPECT_EQ(cmd_raw, commandRaw::none);
}

TEST_F(buttons_test_fixture, raw_allLong) {
  press_button(buttonPausePin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  unsigned long start_time = current_time;

  press_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  press_button(buttonDownPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  EXPECT_EQ(execute_cycle(buttonLongPress-1-(current_time-start_time)), commandRaw::none);
  EXPECT_EQ(execute_cycle(1                                          ), commandRaw::allLong);

  // no new button command until all released
  release_button(buttonPausePin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  release_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  press_button(buttonPausePin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  press_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  release_button(buttonPausePin);
  release_button(buttonUpPin);
  release_button(buttonDownPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  // all released --> new command possible
  press_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  release_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::up);
}

TEST_F(buttons_test_fixture, raw_updownLong) {
  press_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  unsigned long start_time = current_time;

  press_button(buttonDownPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  EXPECT_EQ(execute_cycle(buttonLongPress-1-(current_time-start_time)), commandRaw::none);
  EXPECT_EQ(execute_cycle(1                                          ), commandRaw::updownLong);

  // no new button command until all released
  release_button(buttonUpPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  press_button(buttonPausePin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  release_button(buttonPausePin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  release_button(buttonUpPin);
  release_button(buttonDownPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);

  // all released --> new command possible
  press_button(buttonDownPin);
  EXPECT_EQ(execute_cycle(), commandRaw::none);
  release_button(buttonDownPin);
  EXPECT_EQ(execute_cycle(), commandRaw::down);
}

commandRaw get_command_normal(uint8_t pin) {
  switch(pin) {
  case buttonPausePin: return commandRaw::pause;
  case buttonUpPin   : return commandRaw::up   ;
  case buttonDownPin : return commandRaw::down ;
#ifdef FIVEBUTTONS
  case buttonFourPin : return commandRaw::four ;
  case buttonFivePin : return commandRaw::five ;
#endif
  default:             return commandRaw::none;
  }
}
commandRaw get_command_long(uint8_t pin) {
  switch(pin) {
  case buttonPausePin: return commandRaw::pauseLong;
  case buttonUpPin   : return commandRaw::upLong   ;
  case buttonDownPin : return commandRaw::downLong ;
#ifdef FIVEBUTTONS
  case buttonFourPin : return commandRaw::fourLong ;
  case buttonFivePin : return commandRaw::fiveLong ;
#endif
  default:             return commandRaw::none;
  }
}

commandRaw get_command_repeat(uint8_t pin) {
  switch(pin) {
  case buttonPausePin: return commandRaw::none          ;
  case buttonUpPin   : return commandRaw::upLongRepeat  ;
  case buttonDownPin : return commandRaw::downLongRepeat;
#ifdef FIVEBUTTONS
  case buttonFourPin : return commandRaw::fourLongRepeat;
  case buttonFivePin : return commandRaw::fiveLongRepeat;
#endif
  default:             return commandRaw::none;
  }
}

TEST_F(buttons_test_fixture, raw_all_single_pins) {
  uint8_t pin_list[] = {
      buttonPausePin
    , buttonUpPin
    , buttonDownPin
#ifdef FIVEBUTTONS
    , buttonFourPin
    , buttonFivePin
#endif
  };
  for (uint8_t pin : pin_list) {
    press_button(pin);
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    release_button(pin);
    EXPECT_EQ(execute_cycle(), get_command_normal(pin)) << static_cast<int>(pin);

    press_button(pin);
    EXPECT_EQ(execute_cycle(), commandRaw::none);
    unsigned long start_time = current_time;
    EXPECT_EQ(execute_cycle(buttonLongPress      -1), commandRaw::none);
    EXPECT_EQ(execute_cycle(1                      ), get_command_long  (pin)) << static_cast<int>(pin);
    EXPECT_EQ(execute_cycle(buttonLongPressRepeat-1), commandRaw::none);
    EXPECT_EQ(execute_cycle(1                      ), get_command_repeat(pin)) << static_cast<int>(pin);
    release_button(pin);
    EXPECT_EQ(execute_cycle(), commandRaw::none);
  }
}

TEST_F(buttons_test_fixture, all_commands) {
  commandRaw raw_cmd_list[] = {
      commandRaw::none,
      commandRaw::start,
      commandRaw::allLong,
      commandRaw::pause,
      commandRaw::pauseLong,
      commandRaw::up,
      commandRaw::upLong,
      commandRaw::upLongRepeat,
      commandRaw::down,
      commandRaw::downLong,
      commandRaw::downLongRepeat,
      commandRaw::updownLong,
    #ifdef FIVEBUTTONS
      commandRaw::four,
      commandRaw::fourLong,
      commandRaw::fourLongRepeat,
      commandRaw::five,
      commandRaw::fiveLong,
      commandRaw::fiveLongRepeat,
    #endif
  };
  state_for_command state_list[] = {
      state_for_command::admin     ,
      state_for_command::idle_pause,
      state_for_command::play      ,
  };

  settings.invertVolumeButtons = false;

  for (state_for_command state: state_list) {
    for (commandRaw raw_cmd: raw_cmd_list) {
      EXPECT_EQ(commands.getCommand(raw_cmd, state), cmd_table[static_cast<int>(raw_cmd)][static_cast<int>(state)]);
    }
  }

  settings.invertVolumeButtons = true;

  for (state_for_command state: state_list) {
    state_for_command compare_state = state;
#ifndef FIVEBUTTONS
    if (compare_state == state_for_command::play)
      compare_state = state_for_command::play_invert;
#endif
    for (commandRaw raw_cmd: raw_cmd_list) {
      EXPECT_EQ(commands.getCommand(raw_cmd, state), cmd_table[static_cast<int>(raw_cmd)][static_cast<int>(compare_state)]);
    }
  }
}
