/*
 Grove Sound Sensor
 
 A simple program demonstrating sound detection.  When sound is detected over the set threshold,
 the LED is turned on for 1 second.
 
 The circuit:
 * Sound Sensor attached to Pin 24 (J6 plug on Grove Base BoosterPack)
 
 * Note:   
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-Sound-Sensor-p-752.html
 */
 
#define SOUND_SENSOR       24           /* sound sensor pin */          
#define LED                RED_LED      /* LED pin */
#define THRESHOLD_VALUE    200          /* The threshold to turn the LED on 200.00*3.3/1024 = 0.65v */

/* Global Variables */
int sound_value = 0;       /* variable to store the value coming from sound sensor */

void setup() 
{
    /* Initialize LED pin */
    pinMode(LED, OUTPUT);  
    digitalWrite(LED, LOW);
}
 
void loop() 
{
    /* read the Sound Sensor value */
    sound_value = analogRead(SOUND_SENSOR); 
	
    /* if the sensor value is higher than threshold, turn on LED */
    if(sound_value > THRESHOLD_VALUE) 
    {
        digitalWrite(LED, HIGH); 
        delay(1000);
    }
    digitalWrite(LED, LOW);
}

 
