/*
  Grove-Temperature-Humidity Sensor
 Read value from grove-temperature-humidity sensor then display on grove-4-digital-display
 
 The circuit:
 * 4-digital-display attached to pin38 & pin39 (J14 plug on Grove Base BoosterPack)
 * sig pin of the Grove-Temperature-Humidity Sensor to the analog pin 24 (Grove connector J6)
 * one side pin (either one) to ground
 * the other side pin to +VCC
 * LED anode (long leg) attached to RED_LED
 * LED cathode (short leg) attached to ground
 
 * Note: Put your hands on Grove-Temperature-Humidity Sensor, both of the value 
         will rise. 
         
         4-digital-display:
         |--------------------------|
         | temperature :  humidity  |    
         |--------------------------| 
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/wiki/Grove_-_Temperature_and_Humidity_Sensor_Pro 
 
 */
 
#include "TM1637.h" 
#include "DHT.h"

/* Macro Define */
#define CLK               39                  /* 4-digital display clock pin */
#define DIO               38                 /* 4-digital display data pin */
#define BLINK_LED         RED_LED            /* blink led */
#define TEMP_HUMI_PIN     24                 /* pin of temperature&humidity sensor */

/* Global Varibles */
TM1637 tm1637(CLK, DIO);                  /* 4-digital display object */
DHT dht(TEMP_HUMI_PIN, DHT22);            /* temperature&humidity sensor object */

int8_t t_bits[2] = {0};                   /* array to store the single bits of the temperature */
int8_t h_bits[2] = {0};                   /* array to store the single bits of the humidity */  

/* the setup() method runs once, when the sketch starts */
void setup() {
        
    tm1637.init();                       /* initialize 4-digital display */
    tm1637.set(BRIGHT_TYPICAL);          /* set the brightness */    
    tm1637.point(POINT_ON);              /* light the clock point ":" */
        
    dht.begin();                         /* initialize temperature humidity sensor */
        
    pinMode(RED_LED, OUTPUT);            /* declare the red_led pin as an OUTPUT */
  
}

/* the loop() method runs over and over again */
void loop() {   
  
    int _temperature = dht.readTemperature();             /* read the temperature value from the sensor */
    int _humidity = dht.readHumidity();                   /* read the humidity value from the sensor */    
             
    memset(t_bits, 0, 2);                                 /* reset array when we use it */
    memset(h_bits, 0, 2);
    
    /* 4-digital-display [0,1] is used to display temperature */
    t_bits[0] = _temperature % 10;
    _temperature /= 10;
    t_bits[1] = _temperature % 10;
    
    /* 4-digital-display [2,3] is used to display humidity */ 
    h_bits[0] = _humidity % 10;
    _humidity /= 10;
    h_bits[1] = _humidity % 10;
    
    /* show it */
    tm1637.display(1, t_bits[0]);
    tm1637.display(0, t_bits[1]);
    
    tm1637.display(3, h_bits[0]);
    tm1637.display(2, h_bits[1]);
     
}
