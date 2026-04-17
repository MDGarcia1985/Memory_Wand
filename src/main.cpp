/*
 * @file    main.cpp
 * @brief   Main file for the project. It handles the main runtime opperations of the program.
 *
 * This project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.* org/licenses/>.
 *
 * Author:  Michael Garcia
 * Email:   michael@mandedesign.studio
 * Webiste: https://mandedesign.studio
 * Date:    2026-04-11
 * Version: 1.0
 */

#include <Arduino.h>
#include <headers.h>

// put function declarations here:
int myFunction(int, int);
Led statusLed;

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  statusLed.begin(3);   // example pin
  statusLed.blink(200, 800);
}

void loop() {
  bool raw_pin = digitalRead(BUTTON_PIN);
      btn.updateActiveLow(raw_pin);  // For pull-up buttons
      
      if (btn.pressed()) {
          // Button was just pressed
      }
      if (btn.released()) {
          // Button was just released
      }
      if (btn.down()) {
          // Button is currently held down
      }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}