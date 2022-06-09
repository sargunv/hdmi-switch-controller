#include "config.h"

#include <Arduino.h>

#define USE_NO_SEND_PWM
#define IR_SEND_PIN SIGNAL_PIN
#include <IRremote.hpp>

#include <WiFi.h>
#include <aWOT.h>

const int numInputs = NUM_INPUTS;

const int necAddress = NEC_ADDRESS;
const int necInputCommands[numInputs] = NEC_INPUT_COMMANDS;

#ifdef ENABLE_WAKE_FROM_IDLE
const int idleLedPin = IDLE_LED_PIN;
const int necWakeFromIdleCommand = NEC_WAKE_FROM_IDLE_COMMAND;
#endif

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

void index(Request &req, Response &res)
{
  res.println("# API");

  res.print("- GET `/input/[1-");
  res.print(numInputs);
  res.println("]` : switch directly to input");

#ifdef ENABLE_WAKE_FROM_IDLE
  res.print("- GET `/wake` : wake up from idle");
#endif
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
#ifdef ENABLE_WAKE_FROM_IDLE
    if (digitalRead(idleLedPin) == LOW) {
      sendCommand(necWakeFromIdleCommand, res);
      delay(WAKE_FROM_IDLE_DELAY);
    }
#endif
    sendCommand(necInputCommands[inputVal], res);
  }
}

#ifdef ENABLE_WAKE_FROM_IDLE
void wakeFromIdle(Request &req, Response &res)
{
  sendCommand(necWakeFromIdleCommand, res);
}
#endif

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

#ifdef ENABLE_WAKE_FROM_IDLE
  pinMode(idleLedPin, INPUT);
#endif

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
  app.get("/input/:input", &setInput);
#ifdef ENABLE_WAKE_FROM_IDLE
  app.get("/wake", &wakeFromIdle);
#endif

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
