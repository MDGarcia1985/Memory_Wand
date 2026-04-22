/*
  DEBUG VERSION

  Adds Serial prints to trace:
  - Game flow
  - Sequence generation
  - Button detection
  - Player correctness
*/

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

// -----------------------------
// Setup
// -----------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("=== Simon Game Debug Start ===");

  for (uint8_t i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT);
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  pinMode(PIEZO, OUTPUT);

  randomSeed(analogRead(A0));

  resetGame();
  startupChime();
}

// -----------------------------
// Main Loop
// -----------------------------
void loop() {
  Serial.println("\n--- New Round ---");

  playSequence();

  if (getUserInput()) {
    Serial.println("Result: SUCCESS");

    successChime();

    if (seqLength < MAX_SEQUENCE) {
      sequence[seqLength] = random(0, 4);
      Serial.print("Added to sequence: ");
      Serial.println(sequence[seqLength]);

      seqLength++;
    } else {
      Serial.println("Player WON");
      winAnimation();
      resetGame();
    }

    delay(300);
  } else {
    Serial.println("Result: FAILURE");

    gameOver();
    resetGame();
    delay(500);
  }
}

// -----------------------------
// Reset Game
// -----------------------------
void resetGame() {
  seqLength = 1;
  sequence[0] = random(0, 4);

  Serial.print("Game reset. First value: ");
  Serial.println(sequence[0]);

  waitForAllButtonsReleased();
}

// -----------------------------
// Play Sequence
// -----------------------------
void playSequence() {
  Serial.print("Playing sequence: ");

  for (uint8_t i = 0; i < seqLength; i++) {
    Serial.print(sequence[i]);
    Serial.print(" ");

    uint8_t color = sequence[i];

    digitalWrite(leds[color], HIGH);
    tone(PIEZO, colorTone(color), 180);
    delay(250);

    digitalWrite(leds[color], LOW);
    noTone(PIEZO);
    delay(150);
  }

  Serial.println();
}

// -----------------------------
// User Input
// -----------------------------
bool getUserInput() {
  Serial.println("Waiting for user input...");

  waitForAllButtonsReleased();

  for (uint8_t i = 0; i < seqLength; i++) {
    uint8_t expected = sequence[i];

    Serial.print("Step ");
    Serial.print(i);
    Serial.print(" | Expected: ");
    Serial.println(expected);

    int pressed = waitForButtonPress();

    Serial.print("User pressed: ");
    Serial.println(pressed);

    if (pressed < 0 || pressed > 3) {
      Serial.println("ERROR: Invalid button index");
      return false;
    }

    flashPressedButton(pressed);

    if (pressed != expected) {
      Serial.println("Mismatch detected!");
      return false;
    } else {
      Serial.println("Correct input");
    }
  }

  return true;
}

// -----------------------------
// Wait for Button Press
// -----------------------------
int waitForButtonPress() {
  while (true) {
    for (uint8_t i = 0; i < 4; i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        Serial.print("Detected HIGH on button index: ");
        Serial.println(i);

        delay(15);

        if (digitalRead(buttons[i]) == HIGH) {
          Serial.println("Confirmed press");

          // Wait for release
          while (digitalRead(buttons[i]) == HIGH) {
            delay(1);
          }

          Serial.println("Button released");

          delay(10);
          return i;
        }
      }
    }

    delay(1);
  }
}

// -----------------------------
// Wait for All Buttons Released
// -----------------------------
void waitForAllButtonsReleased() {
  Serial.println("Ensuring all buttons are released...");

  bool anyPressed;

  do {
    anyPressed = false;

    for (uint8_t i = 0; i < 4; i++) {
      if (digitalRead(buttons[i]) == HIGH) {
        anyPressed = true;
      }
    }

    delay(1);
  } while (anyPressed);

  Serial.println("All buttons released");
  delay(10);
}

// -----------------------------
// Feedback
// -----------------------------
void flashPressedButton(uint8_t index) {
  digitalWrite(leds[index], HIGH);
  tone(PIEZO, colorTone(index), 120);
  delay(150);
  digitalWrite(leds[index], LOW);
  noTone(PIEZO);
}

// -----------------------------
// Sounds & Animations
// -----------------------------
void startupChime() {
  Serial.println("Startup chime");

  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(leds[i], HIGH);
    tone(PIEZO, colorTone(i), 80);
    delay(100);
    digitalWrite(leds[i], LOW);
    noTone(PIEZO);
    delay(40);
  }
}

void successChime() {
  Serial.println("Success chime");

  tone(PIEZO, 880, 80);
  delay(100);
  tone(PIEZO, 1175, 100);
  delay(120);
  noTone(PIEZO);
}

void gameOver() {
  Serial.println("Game Over triggered");

  tone(PIEZO, 250, 350);
  delay(400);
  tone(PIEZO, 180, 500);
  delay(550);
  noTone(PIEZO);
}

void winAnimation() {
  Serial.println("Win animation");

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

// -----------------------------
// Tone Mapping
// -----------------------------
int colorTone(uint8_t color) {
  switch (color) {
    case 0: return 262;
    case 1: return 330;
    case 2: return 392;
    case 3: return 523;
    default: return 440;
  }
}