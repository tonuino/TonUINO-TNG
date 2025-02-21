#include "buttons.hpp"

#include "constants.hpp"
#include "logger.hpp"

namespace {
constexpr bool buttonPinIsActiveLow = (buttonPinType == levelType::activeLow);
}

Buttons::Buttons()
: CommandSource()
//            pin             dbTime        puEnable              invert
, buttonPause(buttonPausePin, buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
, buttonUp   (buttonUpPin   , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
, buttonDown (buttonDownPin , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
#ifdef FIVEBUTTONS
, buttonFour (buttonFourPin , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
, buttonFive (buttonFivePin , buttonDbTime, buttonPinIsActiveLow, buttonPinIsActiveLow)
#endif
{
}

void Buttons::begin() {
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
    longPressFactor = 0;
    return commandRaw::none;
  }

  if (ignoreAll) {
    return commandRaw::none;
  }

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

#ifdef FIVEBUTTONS
  else if ((  buttonPause.pressedFor(buttonLongPress)
           || buttonFour .pressedFor(buttonLongPress)
           || buttonFive .pressedFor(buttonLongPress)
      )
     && buttonPause.isPressed()
     && buttonFour .isPressed()
     && buttonFive .isPressed()) {
    ret = commandRaw::allLong;
    ignoreAll = true;
  }
#endif

  else if ((  buttonUp   .pressedFor(buttonLongPress)
           || buttonDown .pressedFor(buttonLongPress)
      )
     && buttonUp   .isPressed()
     && buttonDown .isPressed()) {
    ret = commandRaw::updownLong;
    ignoreAll = true;
  }

  else if (buttonPause.wasReleased() && not ignoreRelease) {
    ret = commandRaw::pause;
  }

  else if (buttonPause.pressedFor(buttonLongPress + longPressFactor * buttonLongPressRepeat)) {
    if (longPressFactor == 0)
      ret = commandRaw::pauseLong;
  }

  else if (buttonUp.wasReleased() && not ignoreRelease) {
    ret = commandRaw::up;
  }

  else if (buttonUp.pressedFor(buttonLongPress + longPressFactor * buttonLongPressRepeat)) {
    if (longPressFactor == 0)
      ret = commandRaw::upLong;
    else
      ret = commandRaw::upLongRepeat;
  }

  else if (buttonDown.wasReleased() && not ignoreRelease) {
    ret = commandRaw::down;
  }

  else if (buttonDown.pressedFor(buttonLongPress + longPressFactor * buttonLongPressRepeat)) {
    if (longPressFactor == 0)
      ret = commandRaw::downLong;
    else
      ret = commandRaw::downLongRepeat;
  }

#ifdef FIVEBUTTONS
  else if (buttonFour.wasReleased() && not ignoreRelease) {
    ret = commandRaw::four;
  }

  else if (buttonFour.pressedFor(buttonLongPress + longPressFactor * buttonLongPressRepeat)) {
    if (longPressFactor == 0)
      ret = commandRaw::fourLong;
    else
      ret = commandRaw::fourLongRepeat;
  }

  else if (buttonFive.wasReleased() && not ignoreRelease) {
    ret = commandRaw::five;
  }

  else if (buttonFive.pressedFor(buttonLongPress + longPressFactor * buttonLongPressRepeat)) {
    if (longPressFactor == 0)
      ret = commandRaw::fiveLong;
    else
      ret = commandRaw::fiveLongRepeat;
  }
#endif

  switch (ret) {
  case commandRaw::pauseLong     :
  case commandRaw::upLong        :
  case commandRaw::upLongRepeat  :
  case commandRaw::downLong      :
  case commandRaw::downLongRepeat:
#ifdef FIVEBUTTONS
  case commandRaw::fourLong      :
  case commandRaw::fourLongRepeat:
  case commandRaw::fiveLong      :
  case commandRaw::fiveLongRepeat:
#endif
                                   ++longPressFactor;
                                   ignoreRelease = true;
                                   break;
  default                        : break;
  }

  if (ret != commandRaw::none) {
    LOG(button_log, s_debug, F("Button raw: "), static_cast<uint8_t>(ret));
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
