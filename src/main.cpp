#include <Arduino.h>

#define USE_NO_SEND_PWM
#define IR_SEND_PIN 12
#include <IRremote.hpp>

#include <WiFi.h>
#include <aWOT.h>

#include "secrets.h"

const int numInputs = 5;

const int inputLedPins[numInputs] = {11, 10, 9, 8, 7};
const int idleLedPin = 6;

const int necInputCommands[numInputs] = {0x1b, 0x1e, 0x0d, 0x12, 0x10};
const int necAddress = 0x01;
const int necWakeCommand = 0x5c;
const int necSleepCommand = 0x1f;
const int necNextInputCommand = 0x0c;
const int necPrevInputCommand = 0x05;

WiFiServer server(80);
Application app;

void sendCommand(int command, Response &res)
{
  res.print("Sent NEC ");
  char buffer[32];
  sprintf(buffer, "address=%02X, function=%02X", necAddress, command);
  res.println(buffer);
  IrSender.sendNEC(necAddress, command, 0);
}

int checkState()
{
  if (digitalRead(idleLedPin) == HIGH) {
    for (int i = 0; i < numInputs; i++) {
      if (digitalRead(inputLedPins[i]) == LOW) {
        return i + 1;
      }
    }
    return -1;
  } else {
    return 0;
  }
}

void index(Request &req, Response &res)
{
  res.println("# API");

  res.println("- GET `/status` : get switch status");

  res.print("- GET `/input/[1-");
  res.print(numInputs);
  res.println("]` : switch directly to input");

  res.println("- GET `/next` : switch to next input");
  res.println("- GET `/prev` : switch to previous input");
  res.println("- GET `/wake` : wake up switch from idle state");
  res.println("- GET `/sleep` : put switch in idle state");
}

void getStatus(Request &req, Response &res)
{
  int input = checkState();
  if (input == 0) {
    res.println("Idle");
  } else if (input == -1) {
    res.status(500);
    res.println("Unknown");
  } else {
    res.print("Input ");
    res.println(input);
  }
}

void setInput(Request &req, Response &res)
{
  char inputBuf[10];
  req.route("input", inputBuf, 10);
  int inputVal = atoi(inputBuf) - 1;

  if (inputVal < 0 || inputVal >= numInputs) {
    res.status(400);
    res.println("Invalid input");
  } else {
    int input = checkState();
    if (input <= 0) {
      res.status(422);
      res.println("Switch is not active");
    } else {
      sendCommand(necInputCommands[inputVal], res);
    }
  }
}

void wake(Request &req, Response &res)
{
  sendCommand(necWakeCommand, res);
}

void sleep(Request &req, Response &res)
{
  sendCommand(necSleepCommand, res);
}

void nextInput(Request &req, Response &res)
{
  int input = checkState();
  if (input <= 0) {
    res.status(422);
    res.println("Switch is not active");
  } else {
    sendCommand(necNextInputCommand, res);
  }
}

void prevInput(Request &req, Response &res)
{
  int input = checkState();
  if (input <= 0) {
    res.status(422);
    res.println("Switch is not active");
  } else {
    sendCommand(necPrevInputCommand, res);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(idleLedPin, INPUT);
  for (int i = 0; i < numInputs; i++) {
    pinMode(inputLedPins[i], INPUT);
  }

  Serial.begin(115200);
  IrSender.begin();

  Serial.print("Connecting ");
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  app.get("/", &index);
  app.get("/status", &getStatus);
  app.get("/input/:input", &setInput);
  app.get("/next", &nextInput);
  app.get("/prev", &prevInput);
  app.get("/wake", &wake);
  app.get("/sleep", &sleep);

  server.begin();
}

void loop()
{
  WiFiClient client = server.available();

  if (client.connected()) {
    app.process(&client);
    client.stop();
  }
}
