#include <Arduino.h>

#define USE_NO_SEND_PWM
#define IR_SEND_PIN 9
#include <IRremote.hpp>

const int numInputs = 5;

const int inputPins[numInputs] = {8, 7, 6, 5, 4};
const int idlePin = 3;

const int necInputCommands[numInputs] = {0x1b, 0x1e, 0x0d, 0x12, 0x10};
const int necAddress = 0x01;
const int necWakeCommand = 0x5c;
const int necSleepCommand = 0x1f;
const int necNextInputCommand = 0x0c;
const int necPrevInputCommand = 0x05;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < numInputs; i++) {
    pinMode(inputPins[i], INPUT);
  }
  Serial.begin(9600);
  IrSender.begin();
}

void sendCommand(int command)
{
  Serial.print("Sending NEC: ");
  char buffer[32];
  sprintf(buffer, "address=%02X, function=%02X", necAddress, command);
  Serial.println(buffer);
  IrSender.sendNEC(necAddress, command, 0);
}

int checkState()
{
  for (int i = 0; i < numInputs; i++) {
    if (digitalRead(inputPins[i]) == LOW) {
      return i + 1;
    }
  }
  if (digitalRead(idlePin) == LOW) {
    return 0;
  }
  return -1;
}

void loop()
{
  if (Serial.available()) {
    char c = Serial.read();

    if (c >= '1' && c < '1' + numInputs) {
      sendCommand(necInputCommands[c - '1']);
    } else if (c == 'w') {
      sendCommand(necWakeCommand);
    } else if (c == 's') {
      sendCommand(necSleepCommand);
    } else if (c == 'n') {
      sendCommand(necNextInputCommand);
    } else if (c == 'p') {
      sendCommand(necPrevInputCommand);
    } else if (c == 'q') {
      int currentInput = checkState();

      Serial.print("Current input: ");
      if (currentInput == 0) {
        Serial.println("None");
      } else if (currentInput == -1) {
        Serial.println("Unknown");
      } else {
        Serial.println(currentInput);
      }
    } else if (c == '?') {
      Serial.println("Commands:");
      Serial.println("1-5: Select input");
      Serial.println("w: Wake");
      Serial.println("s: Sleep");
      Serial.println("n: Next input");
      Serial.println("p: Previous input");
      Serial.println("q: Query current input");
      Serial.println("?: Print this help");
    } else {
      Serial.print("Unknown command: ");
      Serial.println(c);
    }
  }
}