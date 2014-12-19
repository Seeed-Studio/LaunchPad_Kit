/*
  Buzzer
 The example use a buzzer to play melodies. It sends a square wave of the 
 appropriate frequency to the buzzer, generating the corresponding tone.
 
 The circuit:
 * Buzzer attached to pin19 (J14 plug on Grove Base BoosterPack)
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to RED_LED
 * LED cathode (short leg) attached to ground
 
 * Note:  
 
 
 This example code is in the public domain.
 
 http://www.seeedstudio.com/wiki/index.php?title=GROVE_-_Starter_Kit_v1.1b#Grove_-_Buzzer
 
*/

/* Macro Define */
#define BUZZER_PIN               19            /* sig pin of the buzzer */
// You may need to place a jumper on P19 depending on your board

int length = 15;         /* the number of notes */
char notes[] = "ccggaagffeeddc ";
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void setup()
{
    /* set buzzer pin as output */
    pinMode(BUZZER_PIN, OUTPUT);       
}

void loop()
{
    for(int i = 0; i < length; i++) {
        if(notes[i] == ' ') {
            delay(beats[i] * tempo);
        } else {
            playNote(notes[i], beats[i] * tempo);
        }
        delay(tempo / 2);    /* delay between notes */
    }
  
}

/* play tone */
void playTone(int _tone, int duration) {
   
    for(long i = 0; i < duration * 1000L; i += _tone*2 ) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(_tone);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(_tone);
    }
}

void playNote(char note, int duration) {
    
    char name[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b'};
    int _tone[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014};
    
    /* play the tone corresponding to the tone name */
    for(int i = 0; i < 8; i++) {
        if(name[i] == note) {
            playTone(_tone[i], duration);
        }
    }
}
