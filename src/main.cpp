#include <Arduino.h>
#include <TM1637Display.h>

/*
 * Time sync to NTP time source
 * git remote add origin https://github.com/alejandro70/nodemcu-clock.git
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <SimpleTimer.h>
#include <Wire.h>

#include "global.h"
#include "display.h"
#include "ntp.h"

#define BTN_TRIGGER D2 // Activación de Access Point (AP) mode
#define ANALOG_PIN A0  // NodeMCU board ADC pin

const char ssid[] = "Tech_D0003407"; //  your network SSID (name)
const char pass[] = "Apolo@1969";    // your network password

// local variables
volatile int timer1Seconds = 0;

// timers
SimpleTimer timer;
int timerDisplayTime;
int timerMatrixBanner;
int timerLightSensor;

// functions
void restart();
void ldrRange();

// ISR to Fire when Timer is triggered
void ICACHE_RAM_ATTR onTimer1()
{
  timer1Seconds++;
  if (timer1Seconds == 20)
  {
    timer1Seconds = 0;
    if (WiFi.status() != WL_CONNECTED || timeStatus() == timeNotSet)
    {
      ESP.restart();
    }
  }

  // Re-Arm the timer as using TIM_SINGLE
  timer1_write(312500); //1 s
}

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_TRIGGER, INPUT_PULLUP);

  {
    //Initialize NodeMCU Timer1 every 1s
    timer1_attachInterrupt(onTimer1); // Add ISR Function
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
    timer1_write(312500); // 312500 / 1 tick per 3.2 us from TIM_DIV256 == 1 s interval
  }

  // Max72xxPanel
  initDisplay();
  displayHola();

  // WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // NTP config
  udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(5 * 60);

  // timers (init disabled)
  timerDisplayTime = timer.setInterval(1000L, displayTime);
  //timer.disable(timerDisplayTime);
  timer.setTimeout(86400000L, restart);
  timerLightSensor = timer.setInterval(10000L, ldrRange);
}

void loop()
{
  timer.run();
}

void restart()
{
  ESP.restart();
}

void ldrRange()
{
  int sensorValue = analogRead(ANALOG_PIN);

  // ajustar intensidad de display
  int intensity = map(sensorValue, 0, 1024, 0, 7);
  display.setBrightness(intensity);
}
