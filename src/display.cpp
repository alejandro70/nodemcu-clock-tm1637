#include <Arduino.h>
#include <TimeLib.h>
#include <TM1637Display.h>

#include "display.h"
#include "global.h"

// Max72xxPanel
// DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
// --------------------------------------------------------------------------------
int tm1637ClkPin = D2; // TM1637 connection pin clock
int tm1637DioPin = D3; // TM1637 connection pin data
bool evenSecs = false; // show seconds leds? (every 1 sec)

uint8_t SEG_HOLA[] = {
    SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,         // H
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_D | SEG_E | SEG_F,                         // L
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G  // A
};

// TM1637 display
TM1637Display display(tm1637ClkPin, tm1637DioPin);

// reset display
// --------------------------------------------------------------------------------
void initDisplay()
{
    display.setBrightness(0x01);
}

// show current time
// --------------------------------------------------------------------------------
void displayHola()
{
    display.setSegments(SEG_HOLA);
}

// show current time
// --------------------------------------------------------------------------------
void displayTime()
{
    int hh = hour();
    hh = hh <= 12 ? hh : hh - 12;
    hh = hh != 0 ? hh : 12;
    int mm = minute();
    int intTime = hh * 100 + mm;

    uint8_t dots = evenSecs ? 0b01000000 : 0;
    display.showNumberDecEx(intTime, dots, false, 4, 0);

    evenSecs = !evenSecs;
}

void digitalClockDisplay()
{
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(".");
    Serial.print(month());
    Serial.print(".");
    Serial.print(year());
    Serial.println();
}

void printDigits(int digits)
{
    // utility for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10)
        Serial.print('0');
    Serial.print(digits);
}

int setIntensity(String queryString)
{
    int value = queryString.toInt();
    if (value <= 12)
    {
        //config.saveMatrixIntensity(value);
        display.setBrightness(0x01);
    }

    return 0;
}

String timeToString(unsigned long t)
{
    return String(hour(t)) + ":" + minute(t) + ":" + second(t) + " " + day(t) + "/" + month(t) + "/" + year(t);
}
