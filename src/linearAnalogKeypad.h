#ifndef linearAnalogKeypad_h
#define linearAnalogKeypad_h

#include <Arduino.h>

// AiO : 29860 1766 --> 29512 1727
// AiO+: 27200 1287 --> 27264 1248
// cla : 27330 1762 --> 27094 1723

class linearAnalogKeypad
{
	private:

    const int8_t   keypadPin;
		const uint16_t keyNum;
		const int16_t  maxLevel;
    const unsigned long longPressTime;

		unsigned long lastPressTime    = 0;
		unsigned char lastKeyId        = 0xff;
		bool suppressRelease           = false;

	public:
		linearAnalogKeypad(uint8_t pin, uint16_t keyNum, int16_t maxLevel, unsigned long longPressTime);
		unsigned char getKey();
};

#endif
