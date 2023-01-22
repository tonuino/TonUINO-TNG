/*
 * Copyright (c) 2019, ArduinoGetStarted.com. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the ArduinoGetStarted.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARDUINOGETSTARTED.COM "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARDUINOGETSTARTED.COM BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
// Adapted for memory and code optimization and add long press feature

#include "ezAnalogKeypad.h"

ezAnalogKeypad::ezAnalogKeypad(int pin)
: keypadPin(pin)
{
	for(int i = 0; i <= ezAnalogKeypad_MAX_KEY; i++) {
		keys[i]   = 0;
    keysLong[i]   = 0;
		values[i] = 0;
	}
}

void ezAnalogKeypad::setLongPressTime(unsigned long time) {
  longPressTime = time;
}

void ezAnalogKeypad::setNoPressValue(int analogValue) {
	registerKey(0, analogValue); // 0 is key for non-press
}

void ezAnalogKeypad::registerKey(unsigned char key, int analogValue, unsigned char keyLong) {
	if(keyNum <= ezAnalogKeypad_MAX_KEY) {
    if (keyNum > 0 && values[keyNum-1] >= analogValue) {
      //Serial.println(F("bad order"));
      return;
    }
    keys[keyNum]   = key;
    keysLong[keyNum]   = keyLong;
		values[keyNum] = analogValue;
		keyNum++;
		
	} else {
		//Serial.println(F("too much keys"));
	}
}

unsigned char ezAnalogKeypad::getKey() {
	const int analogValue = analogRead(keypadPin);

	int keyId = -1;
	unsigned char ret = 0;

	int lower_bound;
	int upper_bound;
	for(int i = 0; i < keyNum; i++) {
		if(i == 0)
			lower_bound = values[i] - 100; // 100 is tolerance 
		else
			lower_bound = (values[i] + values[i-1]) / 2;

		if(i == (keyNum - 1))
			upper_bound = values[i] + 100; // 100 is tolerance 
		else
			upper_bound = (values[i] + values[i+1]) / 2;

		if(analogValue >= lower_bound && analogValue < upper_bound) {
			keyId = i;
			break;
		}
	}

	if (keyId != -1 && keyId != lastKeyId) {
    // release key
    if (keys[keyId] == 0) {
      if (suppressRelease)
        suppressRelease = false;
      else
        ret = keys[lastKeyId];
    }
    lastKeyId = keyId;
	}
	// long press
	if (!suppressRelease && keys[lastKeyId] != 0) {
	  suppressRelease = true;
    ret = keysLong[keyId];
	}

	return ret;
}
