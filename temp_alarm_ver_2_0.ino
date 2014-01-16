/*Mintduino Temperature Alarm
By Jason Procknow
First draft was 1-10-2014

version history

!!!!!!!!!!!!!!!!!!!as of 1-16-2014 code not working yet!!!!!!!!!!

version 2.0 1-16-2014 
-Changing to 2.0 because this will be a major code redesign. Over
80% of the code will be different all of which will be documented
here or on projects.procknation.com.
-Added rework of if decision structure based on hand written code
from graph pad.  Source is on project.procknation.com
found that the first try at if structure was to complicated. I
simplified the structure to basic T,F decisions.
-Also removed previous debouce function code and added my own debounce
code based on previous debounce code only in design and not content.

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

#define ONE_WIRE_BUS 2 // Data wire is plugged into port 2 on the Arduino

Adafruit_7segment matrix = Adafruit_7segment(); //setup matrix instance to communicate with the Adafruit 0.56" 7-segment HT16K33 backpack Display

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)

DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.

// constants won't change. They're used here to 
// set pin numbers:
const int buzzer_pin = 13;
const int dualLED_green_pin = 7;
const int dualLED_red_pin = 8;
const int snooze_pin = 12;

// Variables will change:

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers




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

  matrix.begin(0x70);  //Start the 8 character, 7-segment display
  
  sensors.begin();  // Start up the library
}


void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempFByIndex(0));
  matrix.println(sensors.getTempFByIndex(0));
  matrix.writeDisplay();
  if (sensors.getTempFByIndex(0) > 75.00)
  {
    Serial.println("alarmalarm");
    if (digitalRead(snooze_pin) == HIGH)
    {
      Serial.println("button pressed");
      
    matrix.blinkRate(1);
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
    matrix.blinkRate(0);
    digitalWrite(buzzer_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_green_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_red_pin, LOW);    // turn the LED off by making the voltage LOW
  }
  
  matrix.writeDisplay();
  //delay(500);
  
  }
}

/* 
 Debounce
 
 Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 a minimum delay between toggles to debounce the circuit (i.e. to ignore
 noise).  
 
 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground
 
 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.
 
 
 created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Debounce
 */
