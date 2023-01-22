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

#ifndef ezAnalogKeypad_h
#define ezAnalogKeypad_h

#include <Arduino.h>

#ifndef ezAnalogKeypad_MAX_KEY
#define ezAnalogKeypad_MAX_KEY 9
#endif

class ezAnalogKeypad
{
	private:
    unsigned long longPressTime = 1000;

    int keypadPin;
    unsigned long lastPressTime    = 0;
		int keyNum                     = 0;
		int lastKeyId                  = -1;
		bool suppressRelease           = false;
		unsigned char keys    [ezAnalogKeypad_MAX_KEY + 1]; // one for non-press state
    unsigned char keysLong[ezAnalogKeypad_MAX_KEY + 1]; // one for non-press state
		int values            [ezAnalogKeypad_MAX_KEY + 1];

	public:
		ezAnalogKeypad(int pin);
		void setLongPressTime(unsigned long time);
		void setNoPressValue(int analogValue);
		void registerKey(unsigned char key, int analogValue, unsigned char keyLong=0);
		unsigned char getKey();
};

#endif
