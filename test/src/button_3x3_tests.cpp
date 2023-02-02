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

  int levels[Buttons3x3::maxLevel+1] = {
#ifdef TonUINO_Classic
         0,
        92,
       194,
       307,
       405,
       512,
       608,
       698,
       773,
      1023
#endif
#ifdef ALLinONE
         0,
       490,
       945,
      1494,
      1949,
      2480,
      2937,
      3373,
      3691,
      4064
#endif
#ifdef ALLinONE_Plus
         0,
       101,
       195,
       309,
       403,
       514,
       612,
       703,
       780,
      1023,
#endif
  };

  commandRaw execute_cycle(unsigned long t = cycleTime) {
    current_time += t;
    return commands.getCommandRaw();
  }

  void set_value(uint8_t level_index) {
    ASSERT_LE(level_index, Buttons3x3::numLevels);
    pin_value[button3x3Pin] = levels[level_index];
  }

  void reset_value() {
    set_value(Buttons3x3::numLevels);
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
    set_value(level_index);
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    reset_value();
    EXPECT_EQ(execute_cycle(), static_cast<commandRaw>(buttonExtSC_begin+level_index)) << static_cast<int>(level_index);

    // long press
    set_value(level_index);
    EXPECT_EQ(execute_cycle(), commandRaw::none);
    unsigned long start_time = current_time;
    EXPECT_EQ(execute_cycle(  buttonLongPress-1-(current_time-start_time)), commandRaw::none);
    EXPECT_EQ(execute_cycle(1                                            ), static_cast<commandRaw>(buttonExtSC_begin+Buttons3x3::numLevels+level_index)) << static_cast<int>(level_index);
    EXPECT_EQ(execute_cycle(2*buttonLongPress-1-(current_time-start_time)), commandRaw::none);
    EXPECT_EQ(execute_cycle(1                                            ), commandRaw::none);
    reset_value();
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    // short press (again)
    set_value(level_index);
    EXPECT_EQ(execute_cycle(), commandRaw::none);

    reset_value();
    EXPECT_EQ(execute_cycle(), static_cast<commandRaw>(buttonExtSC_begin+level_index)) << static_cast<int>(level_index);

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
