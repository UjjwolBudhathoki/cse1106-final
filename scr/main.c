#include <stdio.h>
#include <pigpio.h>
#include <unistd.h>

#define PIN 17

// These are the files used by the program.
#define LOG_FILE "data/temperature_log.csv"
#define LIMIT_FILE "data/limits.txt"
#define STATUS_FILE "status.txt"

// These are the variables that i will use in the whole program.
float low = 68.0;
float high = 76.0;
float sum = 0.0;
int count = 0;
float last = 0.0;
int hasLastTempReading = 0;

void clearInput(void)
{
    // Creating a box to hold the current character we pick up
    int letter;

    // Starting a simple counter at 0
    int charactersCleaned = 0;

    // 3. Keep looping as long as our counter is under 100
    while (charactersCleaned < 100)
    {
        // Grabing one last character from the keyboard memory
        letter = getchar();

        // If this character is the "Enter" key (\n), we are good and breaks the loop
        if (letter == '\n')
        {
            break; // Stop the loop immediately
        }

        // otherwise we add 1 to our counter and repeat
        charactersCleaned = charactersCleaned + 1;
    }
}

// Check if a temperature is in the allowed range.
int isWithinNormalRange(float temp) {
    if (temp >= low && temp <= high) {
        return 1;
    }
    return 0;
}

// Turning the LED on for WARNING and off for NORMAL.
void setLed(int warning)
{
    if (warning == 1) {
        gpioWrite(PIN, 1); //lED ON
    } 
    else {
        gpioWrite(PIN, 0); //lED OFF
    }
}

// This saves the current status in a small text file.
void saveStatus(const char *status)
{
    FILE *file = fopen(STATUS_FILE, "w");

    if (file == NULL) {
        printf("Could not save status.txt.\n");
        return;
    }

    fprintf(file, "%s\n", status);
    fclose(file);
}

// Show the status and change the LED.
void showStatus(float temp)
{
    if (isWithinNormalRange(temp) == 1) {
        printf("Status: NORMAL | LED: OFF\n");
        saveStatus("NORMAL");
        setLed(0);
    } else {
        printf("Status: WARNING | LED: ON\n");
        saveStatus("WARNING");
        setLed(1);
    }
}

// Add one reading and its status to the CSV file.
void saveReading(float temp)
{
    FILE *file = fopen(LOG_FILE, "a+");

    if (file == NULL) {
        printf("Could not save the reading. Check the data folder.\n");
        return;
    }

    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0) {
        fprintf(file, "temperature,status\n");
    }

    if (isWithinNormalRange(temp) == 1) {
        fprintf(file, "%.1f,NORMAL\n", temp);
    } else {
        fprintf(file, "%.1f,WARNING\n", temp);
    }

    fclose(file);
}

// Read old temperatures so the average is not lost.
void loadReadings(void)
{
    FILE *file = fopen(LOG_FILE, "r");
    char line[100];
    char status[20];
    float temp;

    if (file == NULL) {
        return;
    }

    fgets(line, sizeof(line), file);

    while (fscanf(file, "%f,%19s", &temp, status) == 2) {
        sum = sum + temp;
        count = count + 1;
        last = temp;
        hasLastTempReading = 1;
    }

    fclose(file);
}

// Read the saved lower and upper limits.
void loadLimits(void)
{
    FILE *file = fopen(LIMIT_FILE, "r");
    float savedLow;
    float savedHigh;

    if (file == NULL) {
        return;
    }

    if (fscanf(file, "%f %f", &savedLow, &savedHigh) == 2) {
        if (savedLow < savedHigh) {
            low = savedLow;
            high = savedHigh;
        }
    }

    fclose(file);
}

// This allows to Save new limits for the next time as the customer requirements change.
void saveLimits(void)
{
    FILE *file = fopen(LIMIT_FILE, "w");

    if (file == NULL) {
        printf("Could not save the new limits.\n");
        return;
    }

    fprintf(file, "%.1f %.1f\n", low, high);
    fclose(file);
}

// Ask the user for one temperature.
void addReading(void)
{
    float temp;

    printf("Please enter temperature in Fahrenheit: ");

    if (scanf("%f", &temp) != 1) {
        
        clearInput();

        printf("Please enter a number.\n");

        return;
    }

    clearInput();

    sum = sum + temp;
    count = count + 1;
    last = temp;
    hasLastTempReading = 1;

    saveReading(temp);
    printf("Temperature: %.1f F\n", temp);
    showStatus(temp);
}

// Show the average of all saved temperatures.
void showAverage(void)
{
    if (count == 0) {
        printf("No temperatures have been entered yet.\n");
        return;
    }

    printf("Number of readings: %d\n", count);
    printf("Average temperature: %.1f F\n", sum / count);
}

// This Let the user change the acceptable temperature range later as per the requirement of the customer changes.
void changeLimits(void)
{
    float newLow;
    float newHigh;

    printf("Enter new low temperature: ");

    if (scanf("%f", &newLow) != 1) {
        clearInput();
        printf("Please enter a number.\n");
        return;
    }

    clearInput();
    printf("Enter new high temperature: ");

    if (scanf("%f", &newHigh) != 1) {
        clearInput();
        printf("Please enter a number.\n");
        return;
    }

    clearInput();

    if (newLow >= newHigh) {
        printf("The low temperature must be below the high temperature.\n");
        return;
    }

    low = newLow;
    high = newHigh;
    saveLimits();

    printf("New range: %.1f F to %.1f F\n", low, high);

    if (hasLastTempReading == 1) {
        printf("Checking the last temperature again.\n");
        showStatus(last);
    }
}

// Print the choices on the screen.
void welcomescreenWithUsersOptionsMenu(void)
{
    printf("\nCLASSROOM COMFORT MONITOR\n");
    printf("Allowed range: %.1f F to %.1f F\n", low, high);
    printf("1. Enter a temperature\n");
    printf("2. Show average\n");
    printf("3. Change temperature range\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

// Start the program and repeat the menu.
int main(void)
{
    int choice = 0;

    if (gpioInitialise() < 0) {
        printf("The LED could not start. Check the Raspberry Pi.\n");
        return 1;
    }

    gpioSetMode(PIN, PI_OUTPUT);
    setLed(0);

    loadLimits();
    loadReadings();

    if (hasLastTempReading == 1) {
        showStatus(last);
    } else {
        saveStatus("NORMAL");
    }

    while (choice != 4) {
        welcomescreenWithUsersOptionsMenu();

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) {
                printf("\nInput ended. Closing the program.\n");
                break;
            }

            clearInput();
            printf("Please enter a number from 1 to 4.\n");
            continue;
        }

        clearInput();

        if (choice == 1) {
            addReading();

        } else if (choice == 2) {
            showAverage();

        } else if (choice == 3) {
            changeLimits();

        } else if (choice == 4) {
            printf("Goodbye.\n");

        } else {
            printf("Please choose 1, 2, 3, or 4.\n");
        }
    }

    setLed(0);
    gpioTerminate();
    return 0;
}