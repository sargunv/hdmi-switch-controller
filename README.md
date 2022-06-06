## hdmi-switch-controller

Arduino project for controlling a HDMI switch. It takes commands over the serial port on the microcontroller, and fakes an IR signal sent directly to the HDMI switch's circuit to control it. It also gets the current status of the HDMI switch by hooking into the lines powering its status LEDs.

Tested with a SparkFun Pro Mini board and the 5-port switch internally marked HDSW0015M1_1V4 by ASK Technology. It's sold on Amazon and AliExpress under a number of different brands; for example [here](https://www.amazon.com/dp/B07MJ783KG), [here](https://www.amazon.com/dp/B07CF793HQ), [here](https://www.amazon.com/dp/B087CTR22G), and [here](https://www.aliexpress.com/item/3256802650160287.html).

The code should be relatively easy to modify to work with any other switch that includes an IR receiver.

## Assembly

1. Acquire one of the above switches and a 3.3V Arduino-compatible board (I use the Sparkfun Pro Mini).
2. Open up the shell of the switch, and remove the PCB.
3. Remove the IR receiver from the PCB with a soldering iron.
4. Connect output 9 of the Pro Mini to the control line where the IR receiver was (should be the leftmost of the three pads for the IR receiver, the one next to resistor R43).
5. Connect the positive terminal of each of the six LEDs on the switch to inputs 8, 7, 6, 5, 4, and 3 on the Pro Mini.
6. Find the five test pads on the switch (marked CN1) and connect the second and third one to RAW and GND on the Pro Mini, respectively. This will provide 3.3V of power to the Pro Mini directly from the switch itself, allowing you to run it without USB power.
   - The first test pad is apparently used to put the switch's controller into in-circuit programming mode, which is not needed for this project.
   - The fourth and fifth test pads connect to the switch's input and power buttons, and also to the serial port of the switch's controller. They're also not needed for this project.

## Usage

1. Install [PlatformIO Core](http://docs.platformio.org/page/core.html)
2. Run `pio run --target upload`
3. Connect to the device with `pio device monitor` and send a `?` to see the list of available commands.

<img width="696" alt="CleanShot 2022-06-05 at 23 31 46@2x" src="https://user-images.githubusercontent.com/1320357/172107786-80d7dcf9-6b7b-4880-9a18-5e314e57aaf4.png">
