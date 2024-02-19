#include <WiFiS3.h>
#include "WiFi_Secret.h"

int status = WL_IDLE_STATUS;
unsigned int localPort = 2390;   // local port to listen for UDP packets
char ssid[] = SECRET_SSID;  //  your network SSID (name)
char pass[] = SECRET_PASS;  // your network password
const int NTP_PACKET_SIZE = 48;  // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];  //buffer to hold incoming and outgoing packets
IPAddress timeServer(129, 6, 15, 28);  // time.nist.gov NTP server
WiFiUDP Udp;

unsigned long sendNTPpacket(IPAddress& address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;  // LI, Version, Mode
  packetBuffer[1] = 0;           // Stratum, or type of clock
  packetBuffer[2] = 6;           // Polling Interval
  packetBuffer[3] = 0xEC;        // Peer Clock Precision
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  Udp.beginPacket(address, 123);  //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void WifiInit() {
  //  WiFi Connection Initialization
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module not established.");
    while (true)
      ;  // don't continue:
  }
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
  Udp.begin(localPort);
  Serial.println("");
}

void GetTime() {
  sendNTPpacket(timeServer);  // send an NTP packet to a time server
  delay(1000);
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long NTPTime = highWord << 16 | lowWord;   //   NTP Time From Packet
    const unsigned long seventyYears = 2208988800UL;    // Used To Convert NTP To UTC Time
    unsigned long UTCTime = NTPTime - seventyYears;     // UTC is the time at Greenwich Meridian (GMT)
    unsigned int UTCHour = (UTCTime % 86400L) / 3600;  // UTC hour (86400 equals secs per day)
    unsigned int UTCMinute = (UTCTime % 3600) / 60;  // UTC minute (3600 equals secs per minute)
    unsigned int UTCSecond = UTCTime % 60;          // UTC second
    int ESTHour = UTCHour;
    int ESTMinute = UTCMinute;    //    Eastern Standard Time Minutes 
    int ESTSecond = UTCSecond;    //    Eastern Standard Time Seconds
    int adj1 = -5;
    int adj2 = 19;
    Serial.print("Current Time: ");
    if(ESTHour <= 24) {  //    Convert UTC Hours to Easter Standard Time
      Serial.print(ESTHour + adj1);
    }
    else if(ESTHour < 5) {   //    Convert UTC Hours to Easter Standard Time
      Serial.print(ESTHour + adj2);
    }
    Serial.print(':');
    if (ESTMinute < 10) {
      Serial.print('0');
    }
    Serial.print(ESTMinute);
    Serial.print(':');
    if (ESTSecond < 10) {
      Serial.print('0');
    }
    Serial.print(ESTSecond);
    Serial.print(" ");
    Serial.println("(ET)");
  }
}

