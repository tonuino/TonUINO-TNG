#ifdef BUTTONS3X3

#include <gtest/gtest.h>

#include <commands.hpp>
#include <buttons3x3.hpp>

class buttons3x3_test_fixture: public ::testing::Test {
public:
  buttons3x3_test_fixture()
  : initializer{}
  , settings{}
  , buttons{}
  , commands{settings, &buttons}
  {
    reset_value();
  }

  struct Initializer {
    Initializer() {
    }
  };

  commandRaw execute_cycle(unsigned long t = cycleTime) {
    current_time += t;
    return commands.getCommandRaw();
  }

  void set_value(int value) {
    pin_value[button3x3Pin] = value;
  }
  void reset_value() {
    set_value(Buttons3x3::levels[Buttons3x3::numLevels]);
  }

  Initializer initializer;

  Settings   settings;
  Buttons3x3 buttons;
  Commands   commands;
};

TEST_F(buttons3x3_test_fixture, initial_state) {
  commandRaw cmd_raw = commands.getCommandRaw();
  EXPECT_EQ(cmd_raw, commandRaw::none);
}

TEST_F(buttons3x3_test_fixture, raw_all_single_buttons) {
  for (uint8_t level_index = 0; level_index < Buttons3x3::numLevels; ++level_index) {
    // short press
    set_value(Buttons3x3::levels[level_index]);
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    reset_value();
    EXPECT_EQ(execute_cycle(), static_cast<commandRaw>(buttonExtSC_begin+level_index)) << level_index;

    // long press
    set_value(Buttons3x3::levels[level_index]);
    EXPECT_EQ(execute_cycle(), commandRaw::none);
    unsigned long start_time = current_time;
    EXPECT_EQ(execute_cycle(  buttonLongPress-1-(current_time-start_time)), commandRaw::none);
    EXPECT_EQ(execute_cycle(1                                            ), static_cast<commandRaw>(buttonExtSC_begin+Buttons3x3::numLevels+level_index)) << level_index;
    EXPECT_EQ(execute_cycle(2*buttonLongPress-1-(current_time-start_time)), commandRaw::none);
    EXPECT_EQ(execute_cycle(1                                            ), commandRaw::none);
    reset_value();
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    // short press (again)
    set_value(Buttons3x3::levels[level_index]);
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    reset_value();
    EXPECT_EQ(execute_cycle(), static_cast<commandRaw>(buttonExtSC_begin+level_index)) << level_index;

  }
}

TEST_F(buttons3x3_test_fixture, all_commands) {
  state_for_command state_list[] = {
      state_for_command::admin     ,
      state_for_command::idle_pause,
      state_for_command::play      ,
  };

  for (state_for_command state: state_list) {
    for (uint8_t index = 0; index < buttonExtSC_buttons; ++index) {
      commandRaw raw_cmd = static_cast<commandRaw>(buttonExtSC_begin+index);
      EXPECT_EQ(commands.getCommand(raw_cmd, state), static_cast<command>(raw_cmd));
    }
  }
}

#endif
