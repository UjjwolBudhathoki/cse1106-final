# Smart Classroom Comfort Monitor by Ujjwol Budhathoki

This project checks classroom temperatures on a Raspberry Pi where the user types a temperature in Fahrenheit. The program NORMAL when it is between 68 F and 76 F, including 68 and 76 and doesnot show any warning light. But when the temperature is outside that range it shows the warning by turning the led light on.

## What the program can do
1. Allow enter a classroom temperature.
2. Show NORMAL or WARNING.
3. Turn the warning LED on or off.
4. Save temperatures to a CSV file.
5. Show the average of all saved temperatures.
6. Change the low and high temperature limits.
7. Remember saved readings and limits after a restart.


## Connect the LED

Turn off the Raspberry Pi before connecting wires.

1. Use GPIO 17, which is physical pin 11.
2. Connect GPIO 17 to a 1,000-ohm resistor.
3. Connect the resistor to the long leg (+) of the LED.
4. Connect the short leg (-) of the LED to a GND pin (for example, physical pin 6).
5. Check the wires before turning on the Pi.

Note: The LED is only a warning light. The user enters the temperature; this project does not use a temperature sensor.

## Compile on the Raspberry Pi

Open the terminal in the project folder. Compile c source code and run as instructed in the class handout.

Choose from the menu to do what you like to do:

- 1 - Enter a temperature.
- 2 - Show the average.
- 3 - Change the allowed range.
- 4 - Exit.