#include "linearAnalogKeypad.h"

linearAnalogKeypad::linearAnalogKeypad(int pin, int keyNum, int maxLevel, unsigned long longPressTime)
: keypadPin(pin)
, keyNum(keyNum)
, maxLevel(maxLevel)
, longPressTime(longPressTime)
{
}

unsigned char linearAnalogKeypad::getKey() {
	const int analogValue = analogRead(keypadPin);

	unsigned char keyId = min((analogValue+maxLevel/keyNum/2)*keyNum/maxLevel, keyNum);

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
