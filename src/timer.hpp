#ifndef SRC_TIMER_HPP_
#define SRC_TIMER_HPP_

class Timer {
public:
  void start(unsigned long timeout);
  bool isExpired();
  void stop();
  bool isActive() { return active; }
  unsigned long remainingTime();
private:
  unsigned long expireTime{0    };
  bool          active    {false};
};

#endif /* SRC_TIMER_HPP_ */
