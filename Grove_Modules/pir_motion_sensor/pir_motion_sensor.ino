/*
 Grove PIR Motion Sensor (Passive Infrared)
 
 The following sketch demonstrates a simple application of sensing motion.
 When someone moves in the sensor's detecting range, it will output HIGH through 
 its SIG pin and the LED will turn on. Otherwise, it will output LOW. Then you can 
 use it to detect the motion of people. 
 
 The circuit:
 * Grove PIR Motion Sensor attached to Pin 7 (J12 plug on Grove Base BoosterPack)
 
 * Note: Position the Motion Sensor's jumper on N_Retrig to ensure the light stays 
   on while motion is present
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-PIR-Motion-Sensor-p-802.html
 
*/

/* Macro Define */
#define PIR_MOTION_SENSOR        7            /* sig pin of the PIR sensor */
#define LED                      RED_LED      /* LED */

/* the setup() method runs once, when the sketch starts */
void setup() 
{
    pinMode(PIR_MOTION_SENSOR, INPUT);   /* declare the sig pin as an INPUT */
    pinMode(LED, OUTPUT);            /* declare the LED pin as an OUTPUT */
    digitalWrite(LED, LOW);
}

/* the loop() method runs over and over again */
void loop() 
{
    if(isMotionDetected()) 
    {
        digitalWrite(LED, HIGH);           /* if we detect movement, turn on the LED */
    } 
    else 
    {
        digitalWrite(LED, LOW);          /* no movement, turn off the light */
    }
}

/* check if motion has been detected */
boolean isMotionDetected() 
{
    int sensor_val = digitalRead(PIR_MOTION_SENSOR);        /* read sig pin */
    if(sensor_val == HIGH) 
    {
        return true;                                        /* motion detected */
    } 
    else 
    {
        return false;                                       /* no motion detected */
    }
}
