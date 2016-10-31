#include "ThingSpeak.h"
#include <SPI.h>
#include <Ethernet.h>

#define SERIAL         true
#define DBG            true

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient client;

unsigned long myChannelNumber = 176278;
const char * myWriteAPIKey = "0DPUVLCONONBCYY4";
String s1 = "";
boolean s1k = false;
float pE, tnE, thE, hhE;

void setup() {
  if (SERIAL || DBG)
    Serial.begin(9600);
  Serial2.begin(9600);
  s1.reserve(50);

  Ethernet.begin(mac);
  ThingSpeak.begin(client);
}

void loop() {
  Serial2.print('k');
  if (s1k) {
    if (SERIAL) {
      sendmeg("Pres", "hPa(ext)", pE);
      sendmeg("Temp", "NTC(ext)", tnE);
      sendmeg("Temp", "HTU(ext)", thE);
      sendmeg("Hmd", "HTU(ext)", hhE);
    }
    ThingSpeak.setField(1, pE);
    ThingSpeak.setField(2, tnE);
    ThingSpeak.setField(3, thE);
    ThingSpeak.setField(4, hhE);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    s1k = false;
  }
  delay(5000);
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
