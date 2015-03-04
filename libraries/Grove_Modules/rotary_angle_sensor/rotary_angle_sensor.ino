/*
  Rotary Angle Sensor
 Demonstrates analog input by reading an analog sensor on analog 
 pin 1(pin3 on Grove Base BoosterPack), turning on and off a light emitting 
 diode(RED_LED on LanuchPad) and display the value on Grove 4-digital display. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 1(pin3 on Grove Base BoosterPack)
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to RED_LED
 * LED cathode (short leg) attached to ground
 
 * Note: Because of unstable of the voltage, the value of the rotary angle sensor 
         varies slightly from run to run even you don't touch it.  
 
 Created by Oliver Wang
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/wiki/GROVE_-_Starter_Kit_v1.1b#Grove_-_Rotary_Angle_Sensor 
 */
 
#include "TM1637.h" 

/* Macro Define */
#define CLK               9                  /* 4-digital display clock pin */
#define DIO               10                 /* 4-digiral display data pin */
#define BLINK_LED         RED_LED            /* blink led */
#define ROTARY_ANGLE_P    A1                 /* pin of rotary angle sensor */

/* Global Varibles */
TM1637 tm1637(CLK, DIO);                  /* 4-digital display object */
int analog_value = 0;                     /* varible to store the value coming from rotary angle sensor */
int blink_interval = 0;                   /* led delay time */
int8_t bits[4] = {0};                     /* array to store the single bits of the value */

/* the setup() method runs once, when the sketch starts */
void setup() {
    
    /* Initialize 4-digital display */
    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);
  
    /* declare the red_led pin as an OUTPUT */
    pinMode(RED_LED, OUTPUT);
  
}

/* the loop() method runs over and over again */
void loop() {   
  
    analog_value = analogRead(ROTARY_ANGLE_P);      /* read the value from the sensor */
    blink_interval = analog_value;                  /* store the rotary analog value */
    digitalWrite(BLINK_LED, HIGH);                  /* turn on red led */
    memset(bits, 0, 4);                             /* reset array when we use it */
    for(int i = 3; i >= 0; i--) {
        /* get single bits of the analog value */
        bits[i] = analog_value % 10;
        analog_value = analog_value / 10;  
        tm1637.display(i, bits[i]);                 /* display by 4-digital display */
    }
    /*
      The amount of time the LED will be on and off depends on
      the value obtained by analogRead().
    */ 
    delay(blink_interval);    
    digitalWrite(BLINK_LED, LOW);
    delay(blink_interval);
}

 
