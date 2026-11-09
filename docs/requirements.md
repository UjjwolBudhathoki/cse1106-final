# Requirements - Classroom Comfort Monitor

## Purpose

The main goal of this project is to help check if a classroom is too cold or too warm. A person first enters temperatures in Fahrenheit and the program then checks each number and shows a warning if needed.

## Functional requirements

- ID - The program should... - How to check it -

- FR-01 - Let a user type a temperature in Fahrenheit. - Enter number.
- FR-02 - Accept 68 F through 76 F as NORMAL. - Try 68, 72, and 76. -
- FR-03 - Show WARNING for a reading below 68 F or above 76 F. - Try 67 and 77. -
- FR-04 - Turn the LED off for NORMAL and on for WARNING. - Check GPIO 17 and the real LED. -
- FR-05 - Save every valid reading and its status. - Open the CSV file. -
- FR-06 - Calculate the average of saved temperatures. - Enter 70 and 74; expect 72. -
- FR-07 - Let the user change the low and high limits. - Change the range to 65-75. -
- FR-08 - Save the new limits for the next run. - Restart the program. -
- FR-09 - Keep older readings for the average after a restart. - Restart and show the average. -
- FR-10 - Reject letters and an invalid low/high range. - Enter abc or set low to 80 and high to 70. -

## Quality requirements

- The program should handel error when a letter is entered instead of a temperature.
- The data should be saved in files that we can read later.
- The LED should turn off when the program exits normally.

## What this program does and what are the things to note

- The starting range is 68 F to 76 F (inclusive).
- The warning LED turns on if one reading is outside the current range.
- A new range must have a lower limit that is less than the upper limit.
- The light shows the status of the most recent temperature.

## What this project does not do

- It does not measure room temperature by itself, instead a user enters each temperature reading.
- It does not send messages or does any push notification.