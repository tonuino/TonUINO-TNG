
#include <gtest/gtest.h>

#include <settings.hpp>

class settings_test_fixture: public ::testing::Test {
public:
  settings_test_fixture()
  : settings()
  {}

  Settings  settings;
};

TEST_F(settings_test_fixture, initial_state) {
  settings.loadSettingsFromFlash();
  EXPECT_EQ(settings.cookie, cardCookie);
  EXPECT_EQ(settings.version, 2);
}

