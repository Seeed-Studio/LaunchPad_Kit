/*
 * Clock Display
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
#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
unsigned char second;
unsigned char minute = 0;
unsigned char hour = 12;


#define CLK 39 //pins definitions for TM1637 and can be changed to other ports
#define DIO 38
TM1637 tm1637(CLK,DIO);

void setup()
{
  tm1637.set();
  tm1637.init();
  TimerA0_initialize(500); // timing for 500ms
}
void loop()
{
  if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }

}

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
  
  TA0CTL |= MC__UP;  // Start the timer!
}

// Hardware Timer_A instance 0's CCR0 interrupt
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void TimingISR()
{
  halfsecond ++;
  Update = ON;
  if(halfsecond == 2){
    second ++;
    if(second == 60)
    {
      minute ++;
      if(minute == 60)
      {
        hour ++;
        if(hour == 24)hour = 0;
        minute = 0;
      }
      second = 0;
    }
    halfsecond = 0;
  }
  ClockPoint = (~ClockPoint) & 0x01;
}


void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;
  Update = OFF;
}
