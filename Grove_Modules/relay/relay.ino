/*
 Grove Relay 
 
 Basic Relay example.  
 Turns relay on for one second, the off for a second, repeatedly.  
 
 The circuit:
 * Grove Relay attached to Pin 39 (J14 plug on Grove Base BoosterPack)
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/depot/Grove-Relay-p-769.html

*/

/* Macro Define */
#define RELAY_PIN 39

/* the setup() method runs once, when the sketch starts */
void setup() { 
  pinMode(RELAY_PIN, OUTPUT); // initialize the digital pin as an output. 
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(RELAY_PIN, HIGH); // turn the relay on (HIGH is the voltage level)
  delay(1000);   // wait for a second
  digitalWrite(RELAY_PIN, LOW);   // turn the relay off by making the voltage LOW
  delay(1000);   // wait for a second
}
