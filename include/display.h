#include <Arduino.h>
#include <TM1637Display.h>

//
extern int tm1637ClkPin;   // TM1637 connection pin clock
extern int tm1637DioPin;   // TM1637 connection pin data
extern bool evenSecs;      // show seconds leds? (every 1 sec)
extern uint8_t SEG_HOLA[]; // mensaje display

// TM1637 display
extern TM1637Display display;

void initDisplay();
void displayHola();
void beginDisplayTime();
void displayTime();
void digitalClockDisplay();
void printDigits(int digits);
String timeToString(unsigned long t);
int setIntensity(String command);

