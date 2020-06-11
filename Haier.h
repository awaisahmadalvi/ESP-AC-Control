
//  ***** Timer related *****
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <String.h>

//  ***** NTP related *****
//#include <NTPClient.h>
#include <NtpClientLib.h>
#include <WiFiUdp.h>

//  ***** AC related *****
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include "Haier_AC.h"

//  ***** WiFi related *****
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

//  ***** Config related *****
#include <FS.h>
#include <ArduinoJson.h>

//  ***** Common *****
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <StringSplitter.h>

//  ***** Arduino OTA *****
#include <ArduinoOTA.h>

//  ***** USER *****
#include "WiFiCredentials.h"

//  Alarm VARS
AlarmId id;

struct scheduleSettings {
  String active;  // is he schedule active?
  String s_time;  // Sch Start Time
  String e_time;    // Sch End Time?
  String on_dur;  // AC On duration
  String off_dur;  // AC Off duration
  String last_time;  // AC last action time
};

scheduleSettings SCH = {"false", "00:00", "00:00", "0", "0", "00:00"};


// Global Declaration related to AC Control
state acState;
