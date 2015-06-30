/*
 Grove Rotary Angle Sensor

 Demonstrates analog input by reading an analog sensor on analog 
 pin, turning on and off a light emitting diode (RED_LED on LanuchPad) 
 and display the value on Grove 4 Digit Display. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * 4-Digit Display attached to Pin 38 and 39 (J14 plug on Grove Base BoosterPack)
 * Rotary Angle Sensor attached to Pin 24 (J6 plug on Grove Base BoosterPack)
 
 * Note: Because of changes in voltage, the value of the rotary angle sensor 
         varies slightly from run to run even you don't touch it.  
 
 Created by Oliver Wang
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-Rotary-Angle-Sensor-p-770.html
 */
 
#include "TM1637.h" 

/* Macro Define */
#define CLK               39          /* 4-Digit Display clock pin */
#define DIO               38          /* 4-Digit Display data pin */
#define LED               RED_LED     /* blink LED */
#define ROTARY_ANGLE_P    24          /* pin of rotary angle sensor */

/* Global Variables */
TM1637 tm1637(CLK, DIO);              /* 4-Digit Display object */
int analog_value = 0;                 /* variable to store the value coming from rotary angle sensor */
int blink_interval = 0;               /* LED delay time */
int8_t bits[4] = {0};                 /* array to store the single bits of the value */

/* the setup() method runs once, when the sketch starts */
void setup() 
{
    /* Initialize 4-Digit Display */
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);
  
    /* declare the LED pin as an OUTPUT */
    pinMode(LED, OUTPUT);
}

/* the loop() method runs over and over again */
void loop() 
{   
    analog_value = analogRead(ROTARY_ANGLE_P);      /* read the value from the sensor */
    blink_interval = analog_value;                  /* store the rotary analog value */
    digitalWrite(LED, HIGH);                        /* turn on LED */
    
    memset(bits, 0, 4);                             /* reset array before we use it */
    for(int i = 3; i >= 0; i--) 
	{
        /* Convert the value to individual decimal digits for display */
        bits[i] = analog_value % 10;
        analog_value = analog_value / 10;  
        tm1637.display(i, bits[i]);                 /* display on 4-Digit Display */
    }
    /*
      The amount of time the LED will be on and off depends on
      the value obtained by analogRead().
    */ 
    delay(blink_interval);    
    digitalWrite(LED, LOW);
    delay(blink_interval);
}

 
