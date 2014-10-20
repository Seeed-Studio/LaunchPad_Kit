/*
  Moisture-Sensor
 The following sketch demonstrates a simple application of sensing 
 the moisture of the soil. You can know whether a plant needs water 
 or not by observing the results that the sensor outputs.
 
 The circuit:
 * Moisture-Sensor attached to pin3 (UART plug on Grove Base BoosterPack)
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to RED_LED
 * LED cathode (short leg) attached to ground
 
 * Note:  
 
 
 This example code is in the public domain.
 
 http://seeedstudio.com/wiki/Grove_-_Moisture_Sensor

*/
#include "TM1637.h"

/* Macro Define */
#define CLK               9                  /* 4-digital display clock pin */
#define DIO               10                 /* 4-digiral display data pin */
#define BLINK_LED         RED_LED            /* blink led */
#define MOISTURE_PIN      A1                 /* pin of moisture sensor */
#define THRESHOLD_VALUE   300                /* threshold for watering the flowers */

#define ON                 HIGH                          /* led on */
#define OFF                LOW                           /* led off */
#define _handle_led(x)     digitalWrite(BLINK_LED, x)    /* handle led */

/* Global Varibles */
TM1637 tm1637(CLK, DIO);                  /* 4-digital display object */
int analog_value = 0;                     /* varible to store the value coming from rotary angle sensor */
int8_t bits[4] = {0};                     /* array to store the single bits of the value */

/* the setup() method runs once, when the sketch starts */
void setup() {
     
    /* Initialize 4-digital display */
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);
  
    /* declare the red_led pin as an OUTPUT */
    pinMode(BLINK_LED, OUTPUT);
  
}

/* the loop() method runs over and over again */
void loop() {
  
    analog_value = analogRead(MOISTURE_PIN);      /* read the value from the sensor */
  
    /* if the value is smaller than threshold, turn on led */
    if(analog_value < THRESHOLD_VALUE) {
        _handle_led(ON);       
    } else {
        _handle_led(OFF);
    }
    
    memset(bits, 0, 4);                             /* reset array when we use it */
    for(int i = 3; i >= 0; i--) {
        /* get single bits of the analog value */
        bits[i] = analog_value % 10;
        analog_value = analog_value / 10;  
        tm1637.display(i, bits[i]);                 /* display by 4-digital display */
    }
    delay(200);
}
