/*
 Ultrasonic-Ranger

 Measure the distance to obstacles in front and display the value on 
 the Grove 4-Digit Display
 
 The circuit:
 * 4-Digit Display attached to Pin 38 and 39 (J14 plug on Grove Base BoosterPack)
 * Ultrasonic Ranger attached to Pin 24 (J6 plug on Grove Base BoosterPack)
 
 * Note:  
 
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-Ultrasonic-Ranger-p-960.html
 */
 
#include "TM1637.h" 
#include "Ultrasonic.h"

/* Macro Define */
#define CLK               39              /* 4-Digit Display clock pin */
#define DIO               38              /* 4-Digit Display data pin */
#define ULTRASONIC_PIN    24              /* pin of the Ultrasonic Ranger */

/* Global Variables */
TM1637 tm1637(CLK, DIO);                  /* 4-Digit Display object */
Ultrasonic ultrasonic(ULTRASONIC_PIN);    /* Ultrasonic Ranger object */
int distance = 0;                         /* variable to store the distance to obstacles in front */
int8_t bits[4] = {0};                     /* array to store the single bits of the value */

/* the setup() method runs once, when the sketch starts */
void setup() 
{
    /* Initialize 4-Digit Display */
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);
}

/* the loop() method runs over and over again */
void loop() 
{   
    distance = ultrasonic.MeasureInCentimeters();   /* read the value from the sensor */  
    
    memset(bits, 0, 4);                             /* reset array before we use it */
    for(int i = 3; i >= 0; i--) 
    {
        /* Convert the value to individual decimal digits for display */
        bits[i] = distance % 10;
        distance = distance / 10;  
        tm1637.display(i, bits[i]);                 /* display on 4-Digit Display */
    }
    delay(100);
}

 
