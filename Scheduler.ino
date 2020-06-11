
void setScheduleWebListeners(){
  server.serveStatic("/config_schedule", SPIFFS, "/config_schedule.json");
  server.on("/scheduler", HTTP_PUT, PUT_Scheduler);
  server.on("/scheduler", HTTP_GET, GET_Scheduler);
}

void PUT_Scheduler(){
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 100;
  DynamicJsonDocument root(bufferSize);
  DeserializationError error = deserializeJson(root, server.arg("plain"));
  if (error) {
    server.send(404, "text/plain", "FAIL. " + server.arg("plain"));
  } else {
    if (root.containsKey("active")) {
      SCH.active = root["active"].as<String>();
    }
    if (root.containsKey("s_time")) {
      SCH.s_time = root["s_time"].as<String>();
      SCH.last_time = SCH.s_time;
    }
    if (root.containsKey("e_time")) {
      SCH.e_time = root["e_time"].as<String>();
    }
    if (root.containsKey("on_dur")) {
      SCH.on_dur = root["on_dur"].as<String>();
    }
    if (root.containsKey("off_dur")) {
      SCH.off_dur = root["off_dur"].as<String>();
    }
    String output;
    serializeJson(root, output);
    saveScheduleConfig();
    
    server.send(200, "text/plain", "Done!");

    Alarm.delay(200);
    SetSchedule();
  }
}

void GET_Scheduler(){
  
  loadScheduleConfig();
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 100;
  DynamicJsonDocument root(bufferSize);
  
  root["active"] = SCH.active;
  
  if(root["active"] == "true"){
    root["s_time"] = SCH.s_time;
    root["e_time"] = SCH.e_time;
    root["on_dur"] = SCH.on_dur;
    root["off_dur"] = SCH.off_dur;
  }
  String output;
  serializeJson(root, output);
  Serial.print("Reply to SCH GET: ");
  Serial.println(output);
  server.send(200, "text/plain", output);
}

String addTime(String time_now, String min_add){
  StringSplitter *splitter = new StringSplitter(time_now, ':', 3);
  int itemCount = splitter->getItemCount();
  int hours = splitter->getItemAtIndex(0).toInt();
  int minutes = splitter->getItemAtIndex(1).toInt();
  minutes = minutes + (int)(min_add.toInt());
  hours = hours + (minutes/60);
  minutes = (minutes%60);
  String new_time = "";
  new_time = String(hours) + ":" + String(minutes);
  return new_time;
}

bool isGreaterTime(String a, String b){
  StringSplitter *splitter = new StringSplitter(a, ':', 3);
  int itemCount = splitter->getItemCount();
  int hours = splitter->getItemAtIndex(0).toInt();
  int minutes = splitter->getItemAtIndex(1).toInt();
  int a_t = hours*100+minutes;
  StringSplitter *splitter2 = new StringSplitter(b, ':', 3);
  itemCount = splitter2->getItemCount();
  hours = splitter2->getItemAtIndex(0).toInt();
  minutes = splitter2->getItemAtIndex(1).toInt();
  int b_t = hours*100+minutes;
  return (a_t>=b_t);
}

void SetSchedule(){
  loadScheduleConfig();
  DisableSchedule();//&id);
  if(SCH.active == "false"){
    if(acState.powerStatus == AC_ON){
      AC_Poweroff(); 
    }
    return;
  }
    
  time_t start_t, end_t, now_t, next_t;
  now_t = now();
  tmElements_t tmE;
  breakTime(now_t,tmE);
  StringSplitter *splitter = new StringSplitter(SCH.s_time, ':', 3);
  int itemCount = splitter->getItemCount();
  int sHours = splitter->getItemAtIndex(0).toInt();
  int sMinutes = splitter->getItemAtIndex(1).toInt();
  tmE.Hour = (uint8_t)sHours;
  tmE.Minute = (uint8_t)sMinutes;
  tmE.Second = 0;
  start_t = makeTime(tmE);
  StringSplitter *splitter2 = new StringSplitter(SCH.e_time, ':', 3);
  int itemCount2 = splitter2->getItemCount();
  int eHours = splitter2->getItemAtIndex(0).toInt();
  int eMinutes = splitter2->getItemAtIndex(1).toInt();
  tmE.Hour = (uint8_t)eHours;
  tmE.Minute = (uint8_t)eMinutes;
  tmE.Second = 0;
  end_t = makeTime(tmE);

  if(now_t < start_t){
    next_t = start_t; 
  }
  else if(now_t < end_t){
    if(acState.powerStatus == AC_OFF){
      AC_Poweron();
      next_t = now_t+(SCH.on_dur.toInt()*60);
      if(next_t > end_t)
        next_t = end_t;
    }
    else if(acState.powerStatus == AC_ON){
      AC_Poweroff();
      next_t = now_t+(SCH.off_dur.toInt()*60);
      if(next_t > end_t)
        next_t = start_t + SECS_PER_DAY;
    }
  }else{
    AC_Poweroff();
    next_t = start_t + SECS_PER_DAY;  // NEXT Day
  }
  Serial.println("Schedule Set for "+getFormattedTime(next_t));
  id = Alarm.triggerOnce(next_t, SetSchedule);
}

void DisableSchedule() {
  //Serial.println("Schedule disabled.");
  // use Alarm.free() to disable a timer and recycle its memory.
  Alarm.free(id);
  // optional, but safest to "forget" the ID after memory recycled
  id = dtINVALID_ALARM_ID;
  // you can also use Alarm.disable() to turn the timer off, but keep
  // it in memory, to turn back on later with Alarm.enable().
}

String getFormattedTime(time_t rawTime){
  unsigned long hours = (rawTime % 86400L) / 3600;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = (rawTime % 3600) / 60;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  unsigned long seconds = rawTime % 60;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return hoursStr + ":" + minuteStr + ":" + secondStr;
}
