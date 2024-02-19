#include <EEPROM.h>
#include "RTC.h"


int addr0 = 0;
int addr1 = 1;
int addr2 = 2;


DayOfWeek convertDayOfWeek(String s) {

  if (s == String("Mon")) {
    return DayOfWeek::MONDAY;
  }
  if (s == String("Tue")) {
    return DayOfWeek::TUESDAY;
  }
  if (s == String("Wed")) {
    return DayOfWeek::WEDNESDAY;
  }
  if (s == String("Thu")) {
    return DayOfWeek::THURSDAY;
  }
  if (s == String("Fri")) {
    return DayOfWeek::FRIDAY;
  }
  if (s == String("Sat")) {
    return DayOfWeek::SATURDAY;
  }
  if (s == String("Sun")) {
    return DayOfWeek::SUNDAY;
  }
}
Month convertMonth(String s) {

  if (s == String("Jan")) {
    return Month::JANUARY;
  }
  if (s == String("Feb")) {
    return Month::FEBRUARY;
  }
  if (s == String("Mar")) {
    return Month::MARCH;
  }
  if (s == String("Apr")) {
    return Month::APRIL;
  }
  if (s == String("May")) {
    return Month::MAY;
  }
  if (s == String("Jun")) {
    return Month::JUNE;
  }
  if (s == String("Jul")) {
    return Month::JULY;
  }
  if (s == String("Aug")) {
    return Month::AUGUST;
  }
  if (s == String("Sep")) {
    return Month::SEPTEMBER;
  }
  if (s == String("Oct")) {
    return Month::OCTOBER;
  }
  if (s == String("Nov")) {
    return Month::NOVEMBER;
  }
  if (s == String("Dec")) {
    return Month::DECEMBER;
  }
}
RTCTime currentRTCTime() {

  String timeStamp = __TIMESTAMP__;
  int pos1 = timeStamp.indexOf(" ");
  DayOfWeek dayOfWeek = convertDayOfWeek(timeStamp.substring(0, pos1));
  ++pos1;
  int pos2 = timeStamp.indexOf(" ", pos1);
  Month month = convertMonth(timeStamp.substring(pos1, pos2));
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(" ", pos1);
  int day = timeStamp.substring(pos1, pos2).toInt();
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(":", pos1);
  int hour = timeStamp.substring(pos1, pos2).toInt();
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(":", pos1);
  int minute = timeStamp.substring(pos1, pos2).toInt();
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(" ", pos1);
  int second = timeStamp.substring(pos1, pos2).toInt();
  pos1 = ++pos2;
  pos2 = timeStamp.indexOf(" ", pos1);
  int year = timeStamp.substring(pos1, pos2).toInt();
  return RTCTime(day, month, year, hour, minute, second, dayOfWeek, SaveLight::SAVING_TIME_INACTIVE);
}
void RTCinit() {
  //  Set RTC Time From Previous Upload
  RTC.begin();
  RTCTime timeToSet = currentRTCTime();
  RTC.setTime(timeToSet);
  RTCTime currentTime;
  RTC.getTime(currentTime);
  unsigned int rtcMonth = Month2int(currentTime.getMonth());
  unsigned int rtcDay = currentTime.getDayOfMonth();
  unsigned int rtcYear = currentTime.getYear();
  unsigned int rtcHour = currentTime.getHour();
  unsigned int rtcMinutes = currentTime.getMinutes(); 
  unsigned int rtcSeconds = currentTime.getSeconds();                                                                                                                    
  Serial.println("The RTC was just set to: ");
  Serial.print(rtcMonth);
  Serial.print("/");
  Serial.print(rtcDay);
  Serial.print("/");
  Serial.print(rtcYear);
  Serial.print("  ");
  Serial.print(rtcHour);
  Serial.print(":");
  Serial.print(rtcMinutes);
  Serial.print(":");
  Serial.println(rtcSeconds);
  //  Save RTC Time To EEPROM
  byte hour = rtcHour;
  byte minute = rtcMinutes;
  byte second = rtcSeconds;
  EEPROM.write(addr0, hour);
  EEPROM.write(addr1, minute);
  EEPROM.write(addr2, second);
}