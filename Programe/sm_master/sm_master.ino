#include "AdafruitIO_Ethernet.h"
#define IO_USERNAME    "axeltech"
#define IO_KEY         "87787e9762ba1b77ef032124aa94cfdb2ebcb594"
#define SERIAL         true
#define DBG            true
AdafruitIO_Ethernet io(IO_USERNAME, IO_KEY);

String s1 = "";
boolean s1k = false;
float pE, tnE, thE, hhE;

AdafruitIO_Feed *humidext = io.feed("humidext");
AdafruitIO_Feed *tempnext = io.feed("tempnext");
AdafruitIO_Feed *temphext = io.feed("temphext");
AdafruitIO_Feed *pressext = io.feed("pressext");

void setup() {
  if (SERIAL || DBG)
    Serial.begin(9600);
  Serial2.begin(9600);
  s1.reserve(50);
  io.connect();
  while (io.status() < AIO_CONNECTED) {}
  delay(500);
  //Serial.println(io.statusText());
}

void loop() {
  io.run();
  Serial2.print('k');
  if (s1k) {
    if (SERIAL) {
      sendmeg("Pres", "hPa(ext)", pE);
      sendmeg("Temp", "NTC(ext)", tnE);
      sendmeg("Temp", "HTU(ext)", thE);
      sendmeg("Hmd", "HTU(ext)", hhE);
    }

    humidext->save((float)hhE);
    tempnext->save((float)tnE);
    temphext->save((float)thE);
    pressext->save((float)pE);
    s1k = false;
  }
  delay(4000);
}

void serialEvent2() {
  s1 = Serial2.readString();
  if (DBG)
    Serial.println(s1);
  int v[4], j = 1;
  for (int i = 1; i <= s1.length(); i++)
    if (s1[i] == ',')
    {
      v[j] = i;
      j++;
    }

  String pEs = "";
  pEs = s1.substring(0, v[1]);
  pE = pEs.toFloat();

  String tnEs = "";
  tnEs = s1.substring(v[1] + 1, v[2]);
  tnE = tnEs.toFloat();

  String thEs = "";
  thEs = s1.substring(v[2] + 1, v[3]);
  thE = thEs.toFloat();

  String hhEs = "";
  hhEs = s1.substring(v[3] + 1);
  hhE = hhEs.toFloat();

  s1k = true;
}

void sendmeg(String plot, String line, float data)
{
  String smth = "";
  smth += "{TIMEPLOT:";
  smth += plot;
  smth += "|data|";
  smth += line;
  smth += "|T|";
  smth += data;
  smth += "} \n";
  Serial.print(smth);
}
