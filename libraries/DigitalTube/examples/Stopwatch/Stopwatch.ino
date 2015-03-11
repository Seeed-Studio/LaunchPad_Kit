/*
 * Stopwatch
 * An example sketch for the Grove 4-Digit Display
 *
 * Copyright (c) 2012 Seeed technology, Inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <MspFlash.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char microsecond_10 = 0;
unsigned char second;
unsigned char _microsecond_10 = 0;
unsigned char _second;
volatile unsigned int store_index = 0;

boolean Flag_ReadTime;

// Flash Infomem segment used in lieu of an EEPROM
#define FLASH_INFOB_SEGMENT __attribute__((section(".infob")))
#define MAX_FLASH_ENTRIES 31
FLASH_INFOB_SEGMENT volatile unsigned int flash_count = 0;
FLASH_INFOB_SEGMENT volatile unsigned char flash_microsecond_10[MAX_FLASH_ENTRIES];
FLASH_INFOB_SEGMENT volatile unsigned char flash_second[MAX_FLASH_ENTRIES];


#define CLK 39 //pins definitions for TM1637 and can be changed to other ports
#define DIO 38
TM1637 tm1637(CLK,DIO);

void setup()
{
  Serial.begin(9600);
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.init();
  TimerA0_initialize(10);//timing for 10ms
  store_index = flash_count;  // store_index points to newest entry written to flash

  Serial.println("Please send command to control the stopwatch:");
  Serial.println("S - start");
  Serial.println("P - pause");
  Serial.println("L - list the time");
  Serial.println("W - write the time to Flash");
  Serial.println("E - Erase the Flash");
  Serial.println("R - reset");
}
void loop()
{
  char command;
  command = Serial.read();
  switch(command)
  {
    case 'S':stopwatchStart();Serial.println("Start timing...");break;
    case 'P':stopwatchPause();Serial.println("Stopwatch was paused");break;
    case 'L':readTime();break;
    case 'W':saveTime();Serial.println("Save the time");break;
    case 'E':eraseTime();Serial.println("All stopwatch entries erased from flash");break;
    case 'R':stopwatchReset();Serial.println("Stopwatch was reset");break;
    default:break;
  }
  if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }
}
//************************************************

// Configure hardware Timer_A instance 0 to interrupt every <X> milliseconds using the
// board's 32768Hz crystal
void TimerA0_initialize(unsigned int ms)
{
  unsigned long timer_rollover_value = (ms * 32768) / 1000;
  
  // Init Timer_A0
  TA0CTL = TACLR;
  TA0CTL = TASSEL__ACLK | ID__1 | MC__STOP;
  // Configure Count Compare Register 0 - the value at which our timer IRQ will fire (see below)
  TA0CCR0 = (uint16_t) timer_rollover_value;
  TA0CCTL0 = CCIE; // Enable CCR0 interrupt
  
  // We're leaving the timer stopped here since the program manages that start/stop condition itself
}

// Hardware Timer_A instance 0's CCR0 interrupt
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void TimingISR()
{
  microsecond_10 ++;
  Update = ON;
  if(microsecond_10 == 100){
    second ++;
    if(second == 60)
    {
      second = 0;
    }
    microsecond_10 = 0;
  }
  ClockPoint = (~ClockPoint) & 0x01;
  if(Flag_ReadTime == 0)
  {
    _microsecond_10 = microsecond_10;
    _second = second;
  }
}
void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);//POINT_ON = 1,POINT_OFF = 0;
  else tm1637.point(POINT_ON);
  TimeDisp[2] = _microsecond_10 / 10;
  TimeDisp[3] = _microsecond_10 % 10;
  TimeDisp[0] = _second / 10;
  TimeDisp[1] = _second % 10;
  Update = OFF;
}
void stopwatchStart()// Start Timer_A0 counting
{
  Flag_ReadTime = 0;
  TA0CTL |= MC__UP;
}
void stopwatchPause()// Stop Timer_A0
{
  TA0CTL = TA0CTL & ~(MC_1 | MC_2 | MC_3);  // Clear all MC_* bits, meaning MC__STOP
}
void stopwatchReset()
{
  stopwatchPause();
  Flag_ReadTime = 0;
  _microsecond_10 = 0;
  _second = 0;
  microsecond_10 = 0;
  second = 0;
  Update = ON;
}
void saveTime()
{
  unsigned char save_microsecond_10[MAX_FLASH_ENTRIES], save_second[MAX_FLASH_ENTRIES];
  unsigned int save_flash_count = flash_count;
  int i;

  if (flash_count == MAX_FLASH_ENTRIES) {
    Serial.println("Can't save any more entries! Flash full.");
    return;
  }

  // Save entire contents of flash into memory first
  for (i=0; i < MAX_FLASH_ENTRIES; i++) {
    save_microsecond_10[i] = flash_microsecond_10[i];
    save_second[i] = flash_second[i];
  }

  // Update in-memory arrays with new microsecond_10 and second values
  save_microsecond_10[save_flash_count] = microsecond_10;
  save_second[save_flash_count] = second;

  // Erase flash segment
  Flash.erase((unsigned char *)&flash_count);

  // Write flash_count to flash, incrementing it
  save_flash_count++;
  Flash.write((unsigned char *)&flash_count, (unsigned char *)&save_flash_count, sizeof(flash_count));

  // Write saved copy of microsecond_10[] array and second[] array to their flash backing store
  Flash.write((unsigned char *)flash_microsecond_10, (unsigned char *)save_microsecond_10, sizeof(flash_microsecond_10));
  Flash.write((unsigned char *)flash_second, (unsigned char *)save_second, sizeof(flash_second));

  // Update store_index to point to the (new) end of the array
  store_index = flash_count;
}
void readTime()
{
  Flag_ReadTime = 1;
  if(flash_count == 0)
  {
    Serial.println("The time has never been written to flash");
    _microsecond_10 = 0;
    _second = 0;
  }
  else {
    _second = flash_second[store_index-1]; // Reading from MSP430 Infomem Flash is easy!
    _microsecond_10 = flash_microsecond_10[store_index-1];
    Serial.println("List the time");
    store_index--;
    if (store_index == 0) {
      store_index = flash_count;  // Roll back to beginning
      Serial.println("Last entry shown");
    }
  }
  Update = ON;
}
void eraseTime()
{
  // Purge Flash infomem contents
  Flash.erase((unsigned char *)&flash_count);

  // Reinitialize flash_count and store_index to 0.
  unsigned int new_flash_count = 0;
  Flash.write((unsigned char *)&flash_count, (unsigned char *)&new_flash_count, sizeof(flash_count));
  store_index = flash_count;
}
