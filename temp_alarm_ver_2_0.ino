/*Mintduino Temperature Alarm
By Jason Procknow
First version of code for project was started 1-10-2014

version history

!!!!!!!!!!!!!!!!!!!as of 1-16-2014 code not working yet!!!!!!!!!!

version 0.9 1-17-2014
-version change to 0.9 now that added to gitHub. Added my own debounce
code based on previous debounce code only in design and not content.
-Added rework of 'if' decision structure based on hand written code
from graph pad. Source is on project.procknation.com
-Found that the first try at if structure was to complicated. I
simplified the structure to basic boolean decisions.
-Also editing code and comments to make it easier to read.

version 2.0 1-16-2014 
-Changing to 2.0 because this will be a major code redesign. Over
80% of the code will be different all of which will be documented
here or on projects.procknation.com.

-Also removed previous debouce function code 

version 1.8 1-15-2014
Added first try at decision processing stucture and switch debounce.

version 1.7 1-14-2014

version 1.6 1-13-2014

version 1.0 to 1.5 1-10-2014 to 1-12-2014
testing out different sensor inputs and such.  I did this before I was
keeping track of version changes.

*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#define ONE_WIRE_BUS 2 // Data wire is plugged into D2 on the Arduino

//setup matrix instance to communicate with the Adafruit 0.56" 7-segment HT16K33 backpack Display
Adafruit_7segment 7segDisplay = Adafruit_7segment();

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature TempSensor(&oneWire); 

// constants won't change. They're used here to set pin numbers:
const int buzzer_pin = 13;
const int dualLED_green_pin = 7;
const int dualLED_red_pin = 8;
const int snooze_pin = 12;

// Variables will change:

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
long MaxTempF = 75



void setup(void)
{
  // initialize the digital pin as an output.
  pinMode(buzzer_pin, OUTPUT);
  pinMode(dualLED_green_pin, OUTPUT);
  pinMode(dualLED_red_pin, OUTPUT);
  pinMode(snooze_pin, INPUT);
  
  digitalWrite(buzzer_pin, LOW);
  digitalWrite(dualLED_green_pin, LOW);   // turn the LED off
  digitalWrite(dualLED_red_pin, LOW);   // turn the LED off
  
  Serial.begin(9600);  //start the serial port for the debug terminal
  Serial.println("MintDuino Temperature Alarm");

  7segDisplay.begin(0x70);  //Start the 4 character, 7-segment display
  
  TempSensor.begin();  // Start up the Dallas library
}


void loop(void)
{ 
  // call TempSensor.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  TempSensor.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempFByIndex(0));
  7segDisplay.println(TempSensor.getTempFByIndex(0));
  7segDisplay.writeDisplay();
  if (TempSensor.getTempFByIndex(0) > MaxTempF)
  {
    Serial.println("alarmalarm");
    if (digitalRead(snooze_pin) == HIGH)
    {
      Serial.println("button pressed");
      
    7segDisplay.blinkRate(1);
    /*digitalWrite(buzzer_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(dualLED_green_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(dualLED_red_pin, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(500);*/               // wait for a second
    digitalWrite(buzzer_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_green_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_red_pin, HIGH);    // turn the LED off by making the voltage LOW
    delay(300);               // wait for a second
    digitalWrite(buzzer_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(dualLED_green_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(dualLED_red_pin, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    7segDisplay.blinkRate(0);
    digitalWrite(buzzer_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_green_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_red_pin, LOW);    // turn the LED off by making the voltage LOW
  }
  
  7segDisplay.writeDisplay();
  //delay(500);
  
  }
}
