
#include <gtest/gtest.h>

#include <commands.hpp>
#include <buttons.hpp>

class buttons_test_fixture: public ::testing::Test {
public:
  buttons_test_fixture()
  : settings()
  , buttons()
  , commands(settings, &buttons)
  {}

  Settings settings;
  Buttons  buttons;
  Commands commands;
};

TEST_F(buttons_test_fixture, initial_state) {
  commandRaw cmd_raw = commands.getCommandRaw();
  EXPECT_EQ(cmd_raw, commandRaw::none);
}

