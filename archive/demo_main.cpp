/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include <Arduino.h>
#include <stdint.h>


#define RED_BTN 2
#define YEL_BTN 3
#define GRN_BTN 4
#define BLU_BTN 5

#define RED_LED 6
#define YEL_LED 7
#define GRN_LED 8
#define BLU_LED 9

#define PIEZO 10

const uint8_t buttons[4] = {RED_BTN, YEL_BTN, GRN_BTN, BLU_BTN};
const uint8_t leds[4]    = {RED_LED, YEL_LED, GRN_LED, BLU_LED};

const uint8_t MAX_SEQUENCE = 30;
uint8_t sequence[MAX_SEQUENCE];
uint8_t seqLength = 1;

void setup() {
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT);   // External debounce + Schmitt trigger drives logic
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  pinMode(PIEZO, OUTPUT);

  randomSeed(analogRead(A0));
  sequence[0] = random(0, 4);

  startupChime();
}

void loop() {
  playSequence();

  if (getUserInput()) {
    successChime();

    if (seqLength < MAX_SEQUENCE) {
      sequence[seqLength] = random(0, 4);
      seqLength++;
    } else {
      winAnimation();
      resetGame();
    }

    delay(300);
  } else {
    gameOver();
    resetGame();
    delay(500);
  }
}

void resetGame() {
  seqLength = 1;
  sequence[0] = random(0, 4);
}

void playSequence() {
  delay(250);

  for (uint8_t i = 0; i < seqLength; i++) {
    uint8_t color = sequence[i];

    digitalWrite(leds[color], HIGH);
    tone(PIEZO, colorTone(color), 180);
    delay(250);

    digitalWrite(leds[color], LOW);
    noTone(PIEZO);
    delay(150);
  }
}

bool getUserInput() {
  for (uint8_t i = 0; i < seqLength; i++) {
    uint8_t expected = sequence[i];
    int pressed = waitForButtonPress();

    if (pressed < 0) {
      return false;
    }

    digitalWrite(leds[pressed], HIGH);
    tone(PIEZO, colorTone(pressed), 120);
    delay(150);
    digitalWrite(leds[pressed], LOW);
    noTone(PIEZO);

    if (pressed != expected) {
      return false;
    }
  }

  return true;
}

int waitForButtonPress() {
  while (true) {
    for (uint8_t i = 0; i < 4; i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        delay(5); // small guard only; hardware debounce already exists

        if (digitalRead(buttons[i]) == HIGH) {
          unsigned long tStart = millis();

          while (digitalRead(buttons[i]) == HIGH) {
            if (millis() - tStart > 2000) {
              break; // prevent lockup on stuck button
            }
          }

          return i;
        }
      }
    }
  }
}

void startupChime() {
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
    tone(PIEZO, colorTone(i), 80);
    delay(100);
    digitalWrite(leds[i], LOW);
    noTone(PIEZO);
    delay(40);
  }

  digitalWrite(GRN_LED, HIGH);
  tone(PIEZO, 784, 160);
  delay(180);
  digitalWrite(GRN_LED, LOW);
  noTone(PIEZO);
}

void successChime() {
  tone(PIEZO, 880, 80);
  delay(100);
  tone(PIEZO, 1175, 100);
  delay(120);
  noTone(PIEZO);
}

void gameOver() {
  tone(PIEZO, 250, 350);
  delay(400);
  tone(PIEZO, 180, 500);
  delay(550);
  noTone(PIEZO);

  for (uint8_t flash = 0; flash < 4; flash++) {
    for (uint8_t i = 0; i < 4; i++) {
      digitalWrite(leds[i], HIGH);
    }
    delay(120);

    for (uint8_t i = 0; i < 4; i++) {
      digitalWrite(leds[i], LOW);
    }
    delay(120);
  }
}

void winAnimation() {
  for (uint8_t pass = 0; pass < 3; pass++) {
    for (uint8_t i = 0; i < 4; i++) {
      digitalWrite(leds[i], HIGH);
      tone(PIEZO, colorTone(i), 70);
      delay(90);
      digitalWrite(leds[i], LOW);
    }
  }
  noTone(PIEZO);
}

int colorTone(uint8_t color) {
  switch (color) {
    case 0: return 262; // Red   -> C4
    case 1: return 330; // Yellow-> E4
    case 2: return 392; // Green -> G4
    case 3: return 523; // Blue  -> C5
    default: return 440;
  }
}