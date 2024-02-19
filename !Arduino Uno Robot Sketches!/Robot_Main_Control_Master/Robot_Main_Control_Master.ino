#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Modbus.h>
#include <ModbusEthernet.h>

 
const int MCU_Output = 2;     
const int NTP_PACKET_SIZE = 48;
unsigned int localPort = 8888;
unsigned long UTCTime;
unsigned long StartMillisT;
unsigned long StartMillisN;
unsigned long StartMillisS;
unsigned long CurrentMillisT;
unsigned long CurrentMillisN;
unsigned long CurrentMillisS;
const unsigned long TimePeriod = 30000;
const unsigned long NetworkPeriod = 15000;
const unsigned long SerialPeriod = 10000;
char timeServer[] = "time.nist.gov";
IPAddress ip(192, 168, 0, 170);
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAF, 0x19, 0x44 };
byte packetBuffer[NTP_PACKET_SIZE];
EthernetUDP Udp;


void setup() {
  pinMode(MCU_Output, OUTPUT);
  Ethernet.init(10);
  Serial.begin(115200);
  Serial1.begin(115200);
  while(!Serial, !Serial1) {
    ;
  }
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    while (true) {
      delay(1);
    }
  }
  Udp.begin(localPort);
  StartMillisT = millis();
  StartMillisN = millis();
  StartMillisS = millis();
  digitalWrite(MCU_Output, HIGH);
}


void loop() {
  Ethernet.maintain();
  CurrentMillisT = millis();
  if (CurrentMillisT - StartMillisT >= TimePeriod) {
    GetTime();
    StartMillisT = CurrentMillisT;
  }
  CurrentMillisN = millis();
  if (CurrentMillisN - StartMillisN >= NetworkPeriod) {
    EthernetLinkStatus();
    StartMillisN = CurrentMillisN;
  }
  CurrentMillisS = millis();
  if (CurrentMillisS - StartMillisS >= SerialPeriod) {
    SerialShare();
    StartMillisS = CurrentMillisS;
  }
}


void sendNTPpacket(const char * address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;  
  packetBuffer[1] = 0;     
  packetBuffer[2] = 6;   
  packetBuffer[3] = 0xEC; 
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  Udp.beginPacket(address, 123); 
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}


void GetTime() {
  int adj1 = -5;
  sendNTPpacket(timeServer);  // send an NTP packet to a time server
  delay(500);
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long NTPTime = highWord << 16 | lowWord;   //   NTP Time From Packet
    const unsigned long seventyYears = 2208988800UL;    // Used To Convert NTP To UTC Time
    UTCTime = NTPTime - seventyYears;     // UTC is the time at Greenwich Meridian (GMT)
    unsigned int UTCHour = (UTCTime % 86400L) / 3600;  // UTC hour (86400 equals secs per day)
    unsigned int UTCMinute = (UTCTime % 3600) / 60;  // UTC minute (3600 equals secs per minute)
    unsigned int UTCSecond = UTCTime % 60;          // UTC second
    unsigned int ESTHour = ((UTCTime - 18000) % 86400L) / 3600;  // UTC hour (86400 equals secs per day)
    int ESTMinute = UTCMinute;    //    Eastern Standard Time Minutes 
    int ESTSecond = UTCSecond;    //    Eastern Standard Time Seconds
    int adj2 = -12;
    int adj3 = 12;
    Serial.print("Current Time: ");
    if(ESTHour < 12 && ESTHour > 0) {  //    Convert UTC Hours to Eastern Standard Time
      Serial.print(ESTHour);
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
      Serial.print("A.M.");
    }
    else if(ESTHour > 12 && ESTHour < 24) {   //    Convert UTC Hours to Eastern Standard Time
      Serial.print(ESTHour + adj2);
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
      Serial.print("P.M.");
    }
    else if(ESTHour = 0) {
      Serial.print(ESTHour + adj3);
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
      Serial.print("A.M.");
    }
    else if(ESTHour = 12) {
      Serial.print(ESTHour);
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
      Serial.print("P.M.");
    }
    Serial.print(" ");
    Serial.println("(ET)");
  }
}


void EthernetLinkStatus() {
  auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Link Ready");
      break;
    case LinkON:
      Serial.println("Link Active");
      break;
    case LinkOFF:
      Serial.println("Link Not Found");
      break;
  }
  delay(100);
}


void SerialShare() {
  if (Serial.available()) {    
    Serial1.write(Serial.read()); 
  }
  if (Serial1.available()) {
    Serial.println(Serial1.read());
  }
  delay(100);
}


