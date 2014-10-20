/* DHT library 

MIT license
written by Adafruit Industries
*/

#include "DHT.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_uart.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/prcm.h"
#include "driverlib/uart.h"
#include "driverlib/systick.h"

#define cli()                         __set_PRIMASK(1) //NVIC_SETPRIMASK();NVIC_SETFAULTMASK();                                                
#define sei()                         __set_PRIMASK(0) //NVIC_RESETPRIMASK();NVIC_RESETFAULTMASK();

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) {
  _pin = pin;
  _type = type;
  _count = count;
  firstreading = true;
}

void DHT::begin(void) {
  // set up the pins!
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);
  _lastreadtime = 0;
}

//boolean S == Scale.  True == Farenheit; False == Celcius
float DHT::readTemperature(bool S) {
  float f;

  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[2];
      if(S)
      	f = convertCtoF(f);
      	
      return f;
    case DHT22:
    case DHT21:
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f /= 10;
      if (data[2] & 0x80)
	f *= -1;
      if(S)
	f = convertCtoF(f);

      return f;
    }
  }
  Serial.print("Read fail");
  return -1.0;
}

float DHT::convertCtoF(float c) {
	return c * 9 / 5 + 32;
}

float DHT::readHumidity(void) {
  float f;
  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[0];
      return f;
    case DHT22:
    case DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f /= 10;
      return f;
    }
  }
  Serial.print("Read fail");
  return -1.0;
}


boolean DHT::read(void) {
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  unsigned long currenttime;

  // pull the pin high and wait 250 milliseconds
  digitalWrite(_pin, HIGH);
  delay(250);

  currenttime = millis();
  if (currenttime < _lastreadtime) {
    // ie there was a rollover
    _lastreadtime = 0;
  }
  if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
    return true; // return last correct measurement    
  }
  firstreading = false;

  _lastreadtime = millis();

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
  // now pull it low for ~20 milliseconds
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);
  cli();
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);

  // read in timings
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (digitalRead(_pin) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = digitalRead(_pin);

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > _count)
        data[j/8] |= 1;
      j++;
    }

  }

  sei();
  
  // check we read 40 bits and that the checksum matches
  if ((j >= 40) && 
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
    return true;
  }
  
  return false;
}
