#pragma once

#include "secrets.h"

// default config is for the HDSW0015M1_1V4

// basic config for switching inputs
#define NUM_INPUTS 5
#define SIGNAL_PIN 2
#define NEC_ADDRESS 0x01
#define NEC_INPUT_COMMANDS                                                     \
  {                                                                            \
    0x1b, 0x1e, 0x0d, 0x12, 0x10                                               \
  }

// optional config for waking up switch from idle
// #define ENABLE_WAKE_FROM_IDLE
#define IDLE_LED_PIN 3
#define NEC_WAKE_FROM_IDLE_COMMAND 0x5c
#define WAKE_FROM_IDLE_DELAY 1000