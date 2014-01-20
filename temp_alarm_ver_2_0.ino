/*Mintduino Temperature Alarm
By Jason Procknow
First version of code for project was started 1-10-2014
version comments located in the readme.md for now at:
https://github.com/zang206/mintduino_temp_alarm

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
const long SnoozeLength = 600000; //600000 mills = 10 min
const int MaxTemp = 75

// Variables will change:

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long SnoozeStartTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
boolean SnoozeActive = false;  //stores current state of snooze button
float currentTemp = 0; // varible to store current temp so get temp function only 
                        //needs to run once.



void setup(void)
{
  // initialize the digital pins as inputs or outputs.
  pinMode(buzzer_pin, OUTPUT);
  pinMode(dualLED_green_pin, OUTPUT);
  pinMode(dualLED_red_pin, OUTPUT);
  pinMode(snooze_pin, INPUT);
  
  //define the inital states of the digital outputs
  digitalWrite(buzzer_pin, LOW); //turn buzzer off. set high to turn on buzzer.
  //the dualLED is a 2 pin LED with two LEDs connected in parallel in opposite polarities
  //so only one can be on at one time. radioshack part# 276-012 ie if you want green then
  //green_pin must be high and red_pin must be low.  vise versa for red. if both are low or
  //high the LED will be off.
  digitalWrite(dualLED_green_pin, LOW);   // turn the LED off if both low
  digitalWrite(dualLED_red_pin, LOW);   // turn the LED off if both low
  
  Serial.begin(9600);  //start the serial port for the debug terminal
  Serial.println("MintDuino Temperature Alarm");

  7segDisplay.begin(0x70);  //Start the 4 character, 7-segment display
  
  TempSensor.begin();  // Start up the Dallas devices library for the DS18B20
} //end setup function


void loop(void)
{ 
  // call TempSensor.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  TempSensor.requestTemperatures(); // Send the command to get temperature from DS18B20
  Serial.println("DONE");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  currentTemp = sensors.getTempFByIndex(0);
  Serial.println(currentTemp);
  
  SnoozeActive = readSnoozeButton(int button_pin);
  
  7segDisplay.println(currentTemp); //display current temp on display
  7segDisplay.writeDisplay();  //write display memory to physical device
  
  if (currentTemp > MaxTempF && SnoozeActive)  //evaluate temp compared to high temp alarm value in degrees F
  {
    Serial.println("Snooze Active");
      
    
  }
  else if (currentTemp > MaxTempF && SnoozeActive ==false)
  {
    Serial.println("Alarm On")
    soundAlarm();
    
    /*7segDisplay.blinkRate(0);
    digitalWrite(buzzer_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_green_pin, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(dualLED_red_pin, LOW);    // turn the LED off by making the voltage LOW
*/
  }
  else if (currentTemp < MaxTempf && SnoozeActive == true)
  {
    SnoozeActive = false;
}

boolean readSnoozeButton(int button_pin)
{
  int snooze1 = digitalRead(button_pin);
  delay(50);
  if (snooze1 == true && digitalRead(button_pin) == true)
  {
	SnoozeStartTime = mills();
	return true;
  }
  else if (SnoozeActive == true && (mills() - SnoozeStartTime) > SnoozeLength)
  {
	SnoozeStartTime = 0;
	return false;
  }
  else if (SnoozeActive == true)
  {
    return true;
  }
  else if (SnoozeActive == false)
  {
    return false;
  }
}

void soundAlarm()
{
  7segDisplay.blinkRate(1);
  digitalWrite(buzzer_pin, HIGH);   // turn the buzzer on
  digitalWrite(dualLED_green_pin, HIGH);   // turn the green LED on
  digitalWrite(dualLED_red_pin, LOW);   // turn the red LED off
  delay(500);*/               // wait for half a second
  digitalWrite(buzzer_pin, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(dualLED_green_pin, LOW);    // turn the green LED off by making the voltage LOW
  digitalWrite(dualLED_red_pin, HIGH);    // turn the red LED on by making the voltage HIGH
  //delay(300);               // wait for a third of a second

}
