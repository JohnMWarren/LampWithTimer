
/*
  -UKR-
  Проект таймеру з дісплеєм LED TM1637.
  Налаштуванням від енкодеру EC11. Таймер вмикає і вимикає реле 250V.
  Цей девайс зроблено для настільного світильника з UV лампою. Використовуються для фоторезиста друкованих плат.
  Світильникк працює як в звичайному режимі від кнопки, так і в режимі таймера.

  -EN-
  TM1637 LED Display Timer Project.
  Setting from encoder EC11. The timer turns the 250V relay on and off.
  by JhonMWarren4242@gmail.com 08.09.2022 Ukraine
*/

#include "TM1637.h"                             // Lib for TM1637 display with duble dot
#include <EncButton2.h>                         // Encoder library

EncButton2  <EB_ENCBTN> enc(INPUT, 4, 5, 3);    // EC11 LEft(Dt), Right(Clk), But(Sw)

#define CLK 6                                   //pins definitions for TM1637 ports    
#define DIO 7
TM1637 tm1637(CLK, DIO);

int8_t TimeDisp[] = {0, 0, 0, 0};   // Blank led display

byte int1;
byte int2;
byte int3;
byte int4;

int timeup;
int timeupchek;
int Push_timeup;
int Push_timeupchek;

bool ClockPoint = true;                         // double point flag
bool TimStart = false;                          // timer start flag

unsigned long previousMill = 0;
const long interval = 1000;                     // 1 second for the timer to count down

int sec;

int relePin = 8;                                // rele pin
int relePin13 = 13;                             // indication pin



// -function blanks- //

void Intro();                                  // Start screen
void displayData();                            // displaying the timer on the screen
void StartTimer();                             // the start of the timer countdown
void EncoderRead();


void setup() {

  pinMode(relePin, OUTPUT);                   // rele 250 v
  pinMode(relePin13, OUTPUT);                 // This pin need for led indicator relay work

  digitalWrite(relePin13, 1);
  digitalWrite(relePin, 1);                   // when power On - lamp is normal mode ON, but is Off when touch encoder

  Serial.begin(9600);

  tm1637.set(BRIGHT_TYPICAL);
  tm1637.init();

  Intro();                                   // here is a blank start screen

}

void loop() {

  enc.tick();
  EncoderRead();

  displayData();

  StartTimer();

  digitalWrite(relePin13, (digitalRead(relePin)));    // indication led rele state

}



void Intro() {

  tm1637.point(POINT_OFF);
  tm1637.clearDisplay();

}

void displayData() {

  if (timeup != timeupchek) {                    // check for a new action to execute the code only when the value changes

    TimStart = false;                             // each press on the encoder stops the timer

    digitalWrite(relePin, 0);                     // rele off

    if (timeup > 9) {                             // when the value exceeds 9 (becomes decimal), you need to connect the following indicator
      int1 = timeup / 10;
      int2 = timeup % 10;
    } else {
      int2 = timeup;
    }
    if (timeup < 9) {                             // We reset the next indicator when the value is not decimal
      int1 = 0;
    }

    tm1637.point(POINT_ON);
    TimeDisp[0] = int1;
    TimeDisp[1] = int2;
    TimeDisp[2] = int3;
    TimeDisp[3] = int4;

    tm1637.display(TimeDisp);

    timeupchek = timeup;

    // Serial.println(int1 + String(int2) + String(":") + String(int3) + String(int4));
  }

  if (Push_timeup != Push_timeupchek) {       // check for a new action to execute the code only when the value changes

    TimStart = false;                         // each press on the encoder stops the timer

    digitalWrite(relePin, 0);

    if (Push_timeup > 9) {
      int3 = Push_timeup / 10;
      int4 = Push_timeup % 10;
    } else {
      int4 = Push_timeup;
    }
    if (Push_timeup < 9) {
      int3 = 0;
    }

    tm1637.point(POINT_ON);
    TimeDisp[0] = int1;
    TimeDisp[1] = int2;
    TimeDisp[2] = int3;
    TimeDisp[3] = int4;
    tm1637.display(TimeDisp);

    Push_timeupchek = Push_timeup;

    //  Serial.println(int1 + String(int2) + String(":") + String(int3) + String(int4));
  }
}

void  StartTimer() {

  if (TimStart) {

    digitalWrite(relePin, 1);

    if (millis() - previousMill >= interval) {
      previousMill = millis();
      sec--;

      int timeMins = (sec % 3600ul) / 60ul;  // минуты
      int timeSecs = (sec % 3600ul) % 60ul;  // секунды
      Serial.println(timeMins + String(":") + String(timeSecs));
      if (ClockPoint) {
        tm1637.point(POINT_ON);
        ClockPoint = false;
      } else {
        tm1637.point(POINT_OFF);
        ClockPoint = true;
      }
      TimeDisp[0] = timeMins / 10;
      TimeDisp[1] = timeMins % 10;
      TimeDisp[2] = timeSecs / 10;
      TimeDisp[3] = timeSecs % 10;
      tm1637.display(TimeDisp);
      if (sec == 0) {
        TimStart = false;
        digitalWrite(relePin, 0);
      }
    }
  }
}


void EncoderRead() {

  if (enc.hasClicks(2)) {                           // a double click will tell the timer to start
    sec = (timeup * 60) + Push_timeup;
    TimStart = true;                                // the flag is raised to start timer
  }

  if (enc.left()) {
    timeup--;                                       // turning the encoder to the left will take a minute
    if (timeup < 0) {
      timeup = 60;
    }
  }

  if (enc.right()) {
    timeup++;                                       // turning the encoder to the left will add a minute
    if (timeup > 60) {
      timeup = 0;
    }
  }


  if (enc.leftH()) {                                // pressed, turning the encoder to the left will subtract 10 seconds
    Push_timeup = Push_timeup - 10;
    if (Push_timeup < 0) {
      Push_timeup = 50;
    }
  }
  if (enc.rightH())  {                               //turning the encoder to the left when pressed will add 10 seconds
    Push_timeup = Push_timeup + 10;
    if (Push_timeup > 50) {
      Push_timeup = 0;
    }
  }
}
