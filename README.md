## hdmi-switch-controller

Arduino project for controlling a HDMI switch. It takes commands via an HTTP API over WiFi, and fakes an IR signal sent directly to the HDMI switch's circuit to control it. It also gets the current status of the HDMI switch by hooking into the lines powering its status LEDs.

Tested with an Arduino Nano 33 IoT board and the 5-port switch internally marked HDSW0015M1_1V4 by ASK Technology. The switch is sold on Amazon and AliExpress under a number of different brands; for example [here](https://www.amazon.com/dp/B07MJ783KG), [here](https://www.amazon.com/dp/B07CF793HQ), [here](https://www.amazon.com/dp/B087CTR22G), and [here](https://www.aliexpress.com/item/3256802650160287.html).

The code should be relatively easy to modify to work with any other switch that includes an IR receiver.

## Assembly

1. Acquire one of the above switches and a 3.3V Arduino-compatible board (I use the Arduino Nano 33 IoT).
2. Open up the shell of the switch, and remove the PCB.
3. Remove the IR receiver from the PCB with a soldering iron.
4. Connect output 12 of the Arduino to the control line where the IR receiver was (should be the leftmost of the three pads for the IR receiver, the one next to resistor R43).
5. Connect the positive terminal of each of the six LEDs on the switch to inputs 11, 10, 9, 8, 7, and 6 on the Arduino.
6. Find the five test pads on the switch (marked CN1) and connect the second and third one to 3.3V and GND on the Arduino, respectively. This will provide 3.3V of power to the board directly from the switch itself, allowing you to run it without USB power. Do NOT plug in USB power to the Arduino while it's powered from the switch.
   - The first test pad is apparently used to put the switch's controller into in-circuit programming mode, which is not needed for this project.
   - The fourth and fifth test pads connect to the switch's input and power buttons, and also to the serial port of the switch's controller. They're also not needed for this project.
7. Copy secrets.template.h to secrets.h, and fill in the secrets.h file with your WiFi credentials.
8. Upload the code to the board.

## Usage

1. Install [PlatformIO Core](http://docs.platformio.org/page/core.html)
2. Run `pio run --target upload`
3. Connect to the device with `curl http://<ip address>/` and and see the rest of the API in the response.

<img width="822" alt="CleanShot 2022-06-06 at 20 29 39@2x" src="https://user-images.githubusercontent.com/1320357/172289732-a700c2ee-77dc-4474-8997-a40c13b6f087.png">
