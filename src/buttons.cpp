#include "buttons.hpp"

#include "constants.hpp"
#include "logger.hpp"

namespace {
constexpr bool buttonPinIsActiveLow = (buttonPinType == levelType::activeLow);
}

Buttons::Buttons(const Settings& settings)
: CommandSource()
, settings(settings)
//            pin             dbTime        puEnable              invert
, buttonPause(buttonPausePin, buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
, buttonUp   (buttonUpPin   , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
, buttonDown (buttonDownPin , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
#ifdef FIVEBUTTONS
, buttonFour (buttonFourPin , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
, buttonFive (buttonFivePin , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
#endif
{
  buttonPause.begin();
  buttonUp   .begin();
  buttonDown .begin();
#ifdef FIVEBUTTONS
  buttonFour .begin();
  buttonFive .begin();
#endif
}

commandRaw Buttons::getCommandRaw() {
  commandRaw ret = commandRaw::none;
  readButtons();

  if ((ignoreRelease || ignoreAll) && isNoButton()) {
    ignoreAll     = false;
    ignoreRelease = false;
    longPressFactor = 1;
    return commandRaw::none;
  }

  if (ignoreAll) {
    return commandRaw::none;
  }

  const uint32_t longPressTimout = longPressFactor * buttonLongPress;

  if ((  buttonPause.pressedFor(buttonLongPress)
      || buttonUp   .pressedFor(buttonLongPress)
      || buttonDown .pressedFor(buttonLongPress)
      )
     && buttonPause.isPressed()
     && buttonUp   .isPressed()
     && buttonDown .isPressed()) {
    ret = commandRaw::allLong;
    ignoreAll = true;
  }

  else if (buttonPause.wasReleased() && not ignoreRelease) {
    ret = commandRaw::pause;
  }

  else if (buttonPause.pressedFor(longPressTimout)) {
    ret = commandRaw::pauseLong;
  }

  else if (buttonUp.wasReleased() && not ignoreRelease) {
    ret = commandRaw::up;
  }

  else if (buttonUp.pressedFor(longPressTimout)) {
    ret = commandRaw::upLong;
  }

  else if (buttonDown.wasReleased() && not ignoreRelease) {
    ret = commandRaw::down;
  }

  else if (buttonDown.pressedFor(longPressTimout)) {
    ret = commandRaw::downLong;
  }

#ifdef FIVEBUTTONS
  else if (buttonFour.wasReleased() && not ignoreRelease) {
    ret = commandRaw::four;
  }

  else if (buttonFour.pressedFor(longPressTimout)) {
    ret = commandRaw::fourLong;
  }

  else if (buttonFive.wasReleased() && not ignoreRelease) {
    ret = commandRaw::five;
  }

  else if (buttonFive.pressedFor(longPressTimout)) {
    ret = commandRaw::fiveLong;
  }
#endif

  switch (ret) {
  case commandRaw::pauseLong: longPressFactor = 0xff;
                              ignoreRelease = true;
                              break;
  case commandRaw::upLong   :
  case commandRaw::downLong : longPressFactor = 0xff;
                              ignoreRelease = true;
                              break;
#ifdef FIVEBUTTONS
  case commandRaw::fourLong :
  case commandRaw::fiveLong : ++longPressFactor;
                              ignoreRelease = true;
                              break;
#endif
  default:
    break;
  }

  if (ret != commandRaw::none) {
    LOG(button_log, s_info, F("Button raw: "), static_cast<uint8_t>(ret));
  }
  return ret;
}

bool Buttons::isNoButton() {
  return not buttonPause.isPressed()
      && not buttonUp   .isPressed()
      && not buttonDown .isPressed()
#ifdef FIVEBUTTONS
      && not buttonFour .isPressed()
      && not buttonFive .isPressed()
#endif
      ;
}

bool Buttons::isReset() {
  constexpr int buttonActiveLevel = getLevel(buttonPinType, level::active);
  return (digitalRead(buttonPausePin) == buttonActiveLevel &&
          digitalRead(buttonUpPin   ) == buttonActiveLevel &&
          digitalRead(buttonDownPin ) == buttonActiveLevel );
}

void Buttons::readButtons() {
  buttonPause.read();
  buttonUp   .read();
  buttonDown .read();
#ifdef FIVEBUTTONS
  buttonFour .read();
  buttonFive .read();
#endif
}
