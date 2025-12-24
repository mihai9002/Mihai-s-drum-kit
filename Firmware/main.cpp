#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TinyUSB.h>
#include <Keyboard.h>
#define SCR_W 128
#define SCR_H 32

Adafruit_SSD1306 display(SCR_W, SCR_H, &Wire);


const int Pins[7] = {1, 2, 3, 4, 7, 8, 11};
const uint8_t Keys[7] = {'A', 'S', 'D', 'F', 'J', 'K', 'L'};
bool buttonState[7];
bool lastButtonState[7];
unsigned long lastDebounceTime[7];
const unsigned long debounceDelay = 20;


const int encPinA = 9;
const int encPinB = 10;
int lastEncoderA; 


String lastAction = "None";

void drawDisplay() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Drum Pad");

  display.setCursor(0, 16);
  display.print("Last: ");
  display.println(lastAction);

  display.display();
}

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonState[i] = HIGH;
  }

  pinMode(encPinA, INPUT_PULLUP);
  pinMode(encPinB, INPUT_PULLUP);
  lastEncoderA = digitalRead(encPinA);

  Keyboard.begin();

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  drawDisplay();
}
void handleButtons() {
  for (int i = 0; i < 7; i++) {
    bool reading = digitalRead(Pins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        // Button pressed
        if (buttonState[i] == LOW) {
          Keyboard.press(Keys[i]);
          delay(5);
          Keyboard.release(Keys[i]);

          lastAction = "BTN " + String(i + 1);
          drawDisplay();
        }
      }
    }
    lastButtonState[i] = reading;
  }
}


void handleEncoder() {
  int currentA = digitalRead(encoderPinA);

  if (currentA != lastEncoderA) {
    if (digitalRead(encPinB) != currentA) {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(5);
      Keyboard.release(KEY_RIGHT_ARROW);
      lastAction = "ENC +";
    } else {
      Keyboard.press(KEY_LEFT_ARROW);
      delay(5);
      Keyboard.release(KEY_LEFT_ARROW);
      lastAction = "ENC -";
    }
    drawDisplay();
  }

  lastEncoderA = currentA;
}

void loop() {
  handleButtons();
  handleEncoder();
}