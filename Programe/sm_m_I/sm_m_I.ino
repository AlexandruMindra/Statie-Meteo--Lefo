#include <Wire.h>
#include "HTU21D.h"
#include <SoftwareSerial.h>

HTU21D htu21d;
SoftwareSerial s2(3, 2); // RX, TX

float hPa, vout, volt, rt, tntc, h, tmp;
float a1 = 3.354016e-03;
float b1 = 2.56985e-04;
float c1 = 2.620131e-06;
float d1 = 6.383091e-08;

void setup() {
  s2.begin(9600);
  htu21d.begin();
}

void loop() {
  char c = s2.read();
  if (c == 'k')
  {
    hPa = 10 * ((analogRead(0) * 5.000 / 1024) / (0.009 * 5.0) + (0.095 / 0.009));

    volt = analogRead(1) * 5.000 / 1023;
    rt = 50000 / volt - 10000;
    tntc = log(rt / 10000);
    tntc = 1 / (a1 + b1 * tntc + c1 * tntc * tntc + d1 * tntc * tntc * tntc);
    tntc = tntc - 273.15;

    h = htu21d.readHumidity();
    tmp = htu21d.readTemperature();
    String s = "";
    s += hPa;   s += ",";
    s += tntc;  s += ",";
    s += tmp;   s += ",";
    s += h;
    s2.print(s);
  }
}
