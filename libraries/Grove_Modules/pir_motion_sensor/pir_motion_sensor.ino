/*
  Grove-PIR-Motion-Sensor
 The following sketch demonstrates a simple application of sensing montion.
 When someone moves in its detecting range, it will output High through its 
 SIG pin and the LED will light. Otherwise, it will output LOW. Then you can 
 use it to detect the motion of people. 
 
 The circuit:
 * sig pin of the Grove-PIR-Motion-Sensor to pin7 (SPI plug on Grove Base BoosterPack)
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to RED_LED
 * LED cathode (short leg) attached to ground
 
 * Note: Position the trig jumper on N_Retrig to ensure  
 
 This example code is in the public domain.
 
http://www.seeedstudio.com/wiki/index.php?title=Twig_-_PIR_Motion_Sensor
 
*/

/* Macro Define */
#define PIR_MOTION_SENSOR        7            /* sig pin of the PIR sensor */
#define LED                      RED_LED      /* led */

#define ON                       HIGH                    /* led on */
#define OFF                      LOW                     /* led off */
#define _handle_led(x)           digitalWrite(LED, x)    /* handle led */ 

/* the setup() method runs once, when the sketch starts */
void setup() {
    
    pinMode(PIR_MOTION_SENSOR, INPUT);   /* declare the sig pin as an INPUT */
    pinMode(RED_LED, OUTPUT);            /* declare the red_led pin as an OUTPUT */
    _handle_led(OFF);
}

/* the loop() method runs over and over again */
void loop() {
   
    if(isPeopleDetected()) {
        _handle_led(ON);           /* if we detect a people, turn on the led */
    } else {
        _handle_led(OFF);          /* found nobody, turn off the light */
    }
  
}

/* judge if there is a people around */
boolean isPeopleDetected() {
    int sensor_val = digitalRead(PIR_MOTION_SENSOR);        /* read sig pin */
    if(HIGH == sensor_val) {
        return true;                                        /* people detected */
    } else {
        return false;                                       /* people un-detected */
    }
}
