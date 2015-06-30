/*
 Grove Temperature & Humidity Sensor Pro
 
 Read value from Temperature & Humidity sensor then display on 
 the Grove 4-Digit Display.
 
 The circuit:
 * 4-Digit Display attached to Pin 38 and 39 (J14 plug on Grove Base BoosterPack)
 * Rotary Angle Sensor attached to Pin 24 (J6 plug on Grove Base BoosterPack)
 * sig pin of the Grove-Temperature-Humidity Sensor to the analog pin A1

 
 * Note: Put your hands on Temperature & Humidity Sensor, both of the values 
         will rise. 
         
         4-Digit Display:
         |--------------------------|
         | temperature :  humidity  |    
         |--------------------------| 
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-TemperatureHumidity-Sensor-Pro-p-838.html 
 
 */
 
#include "TM1637.h" 
#include "DHT.h"

/* Macro Define */
#define CLK               39          /* 4-Digit Display clock pin */
#define DIO               38          /* 4-Digit Display data pin */
#define BLINK_LED         RED_LED            /* blink LED */
#define TEMP_HUMI_PIN     24                 /* pin of temperature&humidity sensor */

/* Global Variables */
TM1637 tm1637(CLK, DIO);                  /* 4-Digit Display object */
DHT dht(TEMP_HUMI_PIN, DHT22);            /* temperature&humidity sensor object */

int8_t t_bits[2] = {0};                   /* array to store the single bits of the temperature */
int8_t h_bits[2] = {0};                   /* array to store the single bits of the humidity */  

/* the setup() method runs once, when the sketch starts */
void setup() 
{    
    tm1637.init();                       /* initialize 4-Digit Display */
    tm1637.set(BRIGHT_TYPICAL);          /* set the brightness */    
    tm1637.point(POINT_ON);              /* light the clock point ":" */
        
    dht.begin();                         /* initialize temperature humidity sensor */
        
    pinMode(BLINK_LED, OUTPUT);            /* declare the LED pin as an OUTPUT */
}

/* the loop() method runs over and over again */
void loop() 
{   
    int _temperature = dht.readTemperature();             /* read the temperature value from the sensor */
    int _humidity = dht.readHumidity();                   /* read the humidity value from the sensor */    
             
    memset(t_bits, 0, 2);                                 /* reset array before we use it */
    memset(h_bits, 0, 2);                                 /* reset array before we use it */
    
    /* 4-Digit Display [0,1] is used to display temperature */
    t_bits[0] = _temperature % 10;
    _temperature /= 10;
    t_bits[1] = _temperature % 10;
    
    /* 4-Digit Display [2,3] is used to display humidity */ 
    h_bits[0] = _humidity % 10;
    _humidity /= 10;
    h_bits[1] = _humidity % 10;
    
    /* show it */
    tm1637.display(1, t_bits[0]);
    tm1637.display(0, t_bits[1]);
    
    tm1637.display(3, h_bits[0]);
    tm1637.display(2, h_bits[1]);
}

 
