/*
  Light Sensor
 A simple program demonstrate sound sensor senses a sound that is up to the threshold you set 
 in the code, the LED is on for 1s. 
 
 The circuit:
 * sig pin of the sound sensor to the analog pin A1 (serial plug on Grove Base BoosterPack)
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to RED_LED
 * LED cathode (short leg) attached to ground
 
 * Note:   
 

 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-Sound-Sensor-p-752.html?cPath=25_128
 */
 
#define SOUND_SENSOR       A1           /* sound sensor pin */          
#define LED                RED_LED      /* LED pin */
#define THRESHOLD_VALUE    200          /* The threshold to turn the led on 400.00*5/1024 = 1.95v */

#define ON                 HIGH                    /* led on */
#define OFF                LOW                     /* led off */
#define _handle_led(x)     digitalWrite(LED, x)    /* handle led */

/* Global Varibles */
int sound_value = 0;

void setup() {
  
    /* Initialize led pin */
    pinMode(LED, OUTPUT);  
    digitalWrite(LED, LOW);
}
 
void loop() {
  
    /* read the sound value */
    sound_value = analogRead(SOUND_SENSOR);         
    /* if the value is larger than threshold, turn on led */
    if(sound_value > THRESHOLD_VALUE) {
        _handle_led(ON); 
        delay(1000);
    }
    _handle_led(OFF);
}

 
