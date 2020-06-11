#define NTP_TIMEOUT 1500
int8_t timeZone = 4;
int8_t minutesTimeZone = 0;
const PROGMEM char *ntpServer = "pool.ntp.org";

void NTP_setup(){
  
    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        //ntpEvent = event;
        processSyncEvent (event);
        
    });
    
    NTP.setInterval (15);
    NTP.setNTPTimeout (NTP_TIMEOUT);
    NTP.begin (ntpServer, timeZone, true, minutesTimeZone);
}

void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
      Serial.printf ("Time Sync error: %d\n", ntpEvent);
    } else {
      if (ntpEvent == timeSyncd) {
        NTP.setInterval (60*60);  //1 hour = 60 x 60
        Serial.print ("Got NTP time: ");
        Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
        SetSchedule(); // On Restart
      }
    }
}
