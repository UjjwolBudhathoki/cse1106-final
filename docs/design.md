# System Design Explained

## Main parts of the system

1. User: Types a temperature or selects a menu choice.
2. main C program: Checks the temperature against the current limits.
3. Screen: Shows NORMAL or WARNING and the average.
4. Files: Keep readings, limits, and the latest status.
5. LED: Shows the warning. The C program uses GPIO 17 to turn it on or off.

## How one reading works

1. The user enters a number, such as 79.
2. The program checks whether 79 is from 68 through 76.
3. Because it is above 76, the screen says WARNING.
4. The program turns the LED on.
5. The reading and WARNING status are saved in the CSV file.
6. status.txt is changed to WARNING.

For 72 F, the screen says NORMAL and the LED turns off.

## Hardware design

We use the Raspberry Pi 4B, one red LED, jumper wires, a breadboard, and a 1,000-ohm resistor. GPIO 17 is physical pin 11. A GND connection can use physical pin 6. The resistor is in series with the LED to limit the current.

The C program uses gpioWrite(17, 1) to turn on the LED and gpioWrite (17, 0) to turn it off. The real circuit must be tested on the team's Raspberry Pi.

## Files used while the program runs

- data/temperature_log.csv: One row per valid reading. Example format: 79.0,WARNING.
- data/limits.txt: The current low and high limits, such as 68.0 76.0.
- status.txt: The latest status, either NORMAL or WARNING.