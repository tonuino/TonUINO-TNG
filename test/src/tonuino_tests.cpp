
#include <gtest/gtest.h>

#include <tonuino.hpp>
#include <state_machine.hpp>

class tonuino_test_fixture: public ::testing::Test {
public:
  tonuino_test_fixture()
  : tonuino(Tonuino::getTonuino())
  {
    tonuino.setup();
  }

  void execute_cycle() {
    tonuino.loop();
  }

  Tonuino& tonuino;
};


TEST_F(tonuino_test_fixture, initial_state) {
  EXPECT_TRUE(SM_tonuino::is_in_state<Idle>());
  //EXPECT_TRUE(false) << "log: " << Print::get_output();
}

TEST_F(tonuino_test_fixture, loop_takes_cycleTime) {
  unsigned long t = current_time;
  execute_cycle();
  EXPECT_EQ(current_time - t, cycleTime);
}
