/*
|-----------------------------------------------------------------------------------|
|                        *   Serial IO Controller    *                              |
|                                Cody Schaefer                                      |
|                        schaeferdraftdesign@gmail.com                              |
|                           Schaefer Draft & Design                                 |
|                               Massillon, Ohio                                     |
|-----------------------------------------------------------------------------------|
|  6 Digital Inputs  *   2 Digital Outputs  *   4 Relay Outputs & Delay Timers      |
|-----------------------------------------------------------------------------------|
|        WiFi WPA-2 Connection   *   UDP/NTP Client    *   EEPROM Data              |
|-----------------------------------------------------------------------------------|
|    Real Time Clock Set    *   UTC Time    *   Easter Standard Time Update         |
|-----------------------------------------------------------------------------------|
*/
//    Program Stucture Files / Libraries
#include <Arduino.h>
#include <Serial.h>
#include <SPI.h>
#include <Scheduler.h>
#include "I-O_Pins.h"
#include "RTC_Setup.h"
#include "WiFi_Functions.h"
//    Global Program Variables
int OutputTimer1;  //    Relay 1 Timer Variable
int OutputTimer2;  //    Relay 2 Timer Variable
int OutputTimer3;  //    Relay 3 Timer Variable
int OutputTimer4;  //    Relay 4 Timer Variable
//    Input 1 Pushbutton Variable
//  int PBinput1 = Input1;
//    Input 1 Change State Variables
//  int PBinput1Counter = 0;
//  int PBinput1State = 0;
//  int PBinput1LastState = 0;
//    Serial Input String
String readString, input1, timer1, input2, timer2, input3, timer3, input4, timer4;  //    Serial Command String
//    Main Program Setup
void setup() {
  //  Input Pin Set
  pinMode(Input1, INPUT_PULLUP);  //    Set Input 1 Pin
  pinMode(Input2, INPUT_PULLUP);  //    Set Input 2 Pin
  pinMode(Input3, INPUT_PULLUP);  //    Set Input 3 Pin
  pinMode(Input4, INPUT_PULLUP);  //    Set Input 4 Pin
  pinMode(Input5, INPUT_PULLUP);  //    Set Input 5 Pin
  pinMode(Input6, INPUT_PULLUP);  //    Set Input 6 Pin
  // Output Pin Set
  pinMode(Relay1, OUTPUT);      //    Set Relay 1 Output Pin
  pinMode(Relay2, OUTPUT);      //    Set Relay 2 Output Pin
  pinMode(Relay3, OUTPUT);      //    Set Relay 3 Output Pin
  pinMode(Relay4, OUTPUT);      //    Set Relay 4 Output Pin
  pinMode(Output1, OUTPUT);      //    Set Output 1 Pin
  pinMode(Output2, OUTPUT);      //    Set Output 2 Pin
  //  Scheduler Loop Initialization
  Scheduler.startLoop(TimeLoop);    //    Time Readout Loop
  Scheduler.startLoop(RelayLoop);   //    IO Relay Loop
  // Serial Port Initialization
  Serial.begin(115200);        //    Start Serial Com Port
  Serial1.begin(115200);       //    Start Serial1 Com Port
  while (!Serial, !Serial1) {  //     Wait For Port To Initialize
    ;
  }
  RTCinit();    //    RTC Initialization
  WifiInit();   //    WiFi Initialization
  Serial.print("Serial Command Format: ");    //    Start Up Readout
  Serial.println("Relay #: (I, T)");    //  Serial Command Format / Structure
}
//    Main Program Loop
void loop() {
 /* 
  PBinput1State = digitalRead(PBinput1);
  if (PBinput1State != PBinput1LastState) {
    if (PBinput1State == LOW) {
      PBinput1Counter++;
    } else {
      return;
    }
    delay(50);
  }
  PBinput1LastState = PBinput1State;
  if (PBinput1Counter % 1 == 0) {
    digitalWrite(Relay1, HIGH);
  } else {
    digitalWrite(Relay1, LOW);
  }
 */
  yield();
}
//    Time Readout Loop
void TimeLoop() {
  GetTime();
  delay(15000);
}
//    IO Relay Loop
void RelayLoop() {
  while (Serial.available()) {
    delay(10);
    if (Serial.available() > 0) {
      char c = Serial.read();
      readString += c;
      RelayRun();
      RelayStatus();
      delay(1000);
    }
    else {
      delay(10);
      return;
    }
  }
}
//    Serial IO Read/Write Function
void RelayRun() {
  if (readString.length() > 0) {
    Serial1.println(readString);
    input1 = readString.substring(0, 1);  //    Read Relay 1 Input
    timer1 = readString.substring(1, 2);  //    Read Relay 1 Timer
    input2 = readString.substring(2, 3);  //    Read Relay 2 Input
    timer2 = readString.substring(3, 4);  //    Read Relay 2 Timer
    input3 = readString.substring(4, 5);  //    Read Relay 3 Input
    timer3 = readString.substring(5, 6);  //    Read Relay 3 Timer
    input4 = readString.substring(6, 7);  //    Read Relay 4 Input
    timer4 = readString.substring(7, 8);  //    Read Relay 4 Timer
    int i1;  //    Input Variable 1
    int i2;  //    Input Variable 2
    int i3;  //    Input Variable 3
    int i4;  //    Input Variable 4
    int t1;  //    Timer Variable 1
    int t2;  //    Timer Variable 2
    int t3;  //    Timer Variable 3
    int t4;  //    Timer Variable 4
    char carray1[6];  //    Relay 1 Input
    input1.toCharArray(carray1, sizeof(carray1));
    i1 = atoi(carray1);
    char carray2[6];  //    Relay 1 Timer
    timer1.toCharArray(carray2, sizeof(carray2));
    t1 = atoi(carray2);
    char carray3[6];  //    Relay 2 Input
    input2.toCharArray(carray3, sizeof(carray3));
    i2 = atoi(carray3);
    char carray4[6];  //    Relay 2 Timer
    timer2.toCharArray(carray4, sizeof(carray4));
    t2 = atoi(carray4);
    char carray5[6];  //    Relay 3 Input
    input3.toCharArray(carray5, sizeof(carray5));
    i3 = atoi(carray5);
    char carray6[6];  //    Relay 3 Timer
    timer3.toCharArray(carray6, sizeof(carray6));
    t3 = atoi(carray6);
    char carray7[6];  //    Relay 4 Input
    input4.toCharArray(carray7, sizeof(carray7));
    i4 = atoi(carray7);
    char carray8[6];  //    Relay 4 Timer
    timer4.toCharArray(carray8, sizeof(carray8));
    t4 = atoi(carray8);
    if (t1 > 0) {  // Timer 1 Condition
      OutputTimer1 = t1 * 1000;
      digitalWrite(Relay1, i1);
      delay(OutputTimer1);
      digitalWrite(Relay1, LOW);
    } else {
      digitalWrite(Relay1, i1);
    }
    if (t2 > 0) {  // Timer 2 Condition
      OutputTimer2 = t2 * 1000;
      digitalWrite(Relay2, i2);
      delay(OutputTimer2);
      digitalWrite(Relay2, LOW);
    } else {
      digitalWrite(Relay2, i2);
    }
    if (t3 > 0) {  // Timer 3 Condition
      OutputTimer3 = t3 * 1000;
      digitalWrite(Relay3, i3);
      delay(OutputTimer3);
      digitalWrite(Relay3, LOW);
    } else {
      digitalWrite(Relay3, i3);
    }
    if (t4 > 0) {  // Timer 4 Condition
      OutputTimer4 = t4 * 1000;
      digitalWrite(Relay4, i4);
      delay(OutputTimer4);
      digitalWrite(Relay4, LOW);
    } else {
      digitalWrite(Relay4, i4);
    }
    readString = "";  //    Reset Input String
  }
}
//    Relay Status Readout Function
void RelayStatus() {
  int R1On = digitalRead(Relay1);
  int R2On = digitalRead(Relay2);
  int R3On = digitalRead(Relay3);
  int R4On = digitalRead(Relay4);
  if (R1On == HIGH) {  // Relay 1 Status
    Serial.println("Relay 1: On");
    Serial1.println("Relay 1: On");
  } else {
    Serial.println("Relay 1: Off");
    Serial1.println("Relay 1: Off");
  }
  if (R2On == HIGH) {  // Relay 2 Status
    Serial.println("Relay 2: On");
    Serial1.println("Relay 2: On");
  } else {
    Serial.println("Relay 2: Off");
    Serial1.println("Relay 2: Off");
  }
  if (R3On == HIGH) {  // Relay 3 Status
    Serial.println("Relay 3: On");
    Serial1.println("Relay 3: On");
  } else {
    Serial.println("Relay 3: Off");
    Serial1.println("Relay 3: Off");
  }
  if (R4On == HIGH) {  // Relay 4 Status
    Serial.println("Relay 4: On");
    Serial1.println("Relay 4: On");
  } else {
    Serial.println("Relay 4: Off");
    Serial1.println("Relay 4: Off");
  }
  Serial.println(" ");
  Serial1.println(" ");
  delay(1000);
  Serial.println(" ");
  Serial1.println(" ");
}


