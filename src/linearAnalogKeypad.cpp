#include "linearAnalogKeypad.h"

#include "logger.hpp"

linearAnalogKeypad::linearAnalogKeypad(uint8_t pin, uint16_t keyNum, int16_t maxLevel, unsigned long longPressTime)
: keypadPin(pin)
, keyNum(keyNum)
, maxLevel(maxLevel)
, longPressTime(longPressTime)
{
}

unsigned char linearAnalogKeypad::getKey() {
	const int16_t analogValue = analogRead(keypadPin);

	uint16_t keyId = static_cast<int32_t>(analogValue+maxLevel/keyNum/2)*keyNum/maxLevel;
	keyId = min(keyId, keyNum);

  static uint16_t t = 0;
	LOG_CODE(button_log, s_debug,
  if (t++ % 10 == 0)
    LOG(button_log, s_debug, F("Button3x3 analog value: "), analogValue, F("key: "), keyId);
	)

	unsigned char ret = 0;


  const unsigned long currentTime = millis();
	if (keyId != lastKeyId) {
    // release key
    if (keyId == keyNum) {
      if (suppressRelease)
        suppressRelease = false;
      else
        ret = lastKeyId+1;
    }
    lastPressTime = currentTime;
    lastKeyId = keyId;
	}
	// long press
	if (!suppressRelease && lastKeyId != keyNum && (currentTime - lastPressTime) >= longPressTime) {
	  suppressRelease = true;
    ret = keyId+1+keyNum;
	}

	return ret;
}
