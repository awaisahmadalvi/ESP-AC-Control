void saveScheduleConfig(){
  String schStr = "";
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 100;
  DynamicJsonDocument doc(bufferSize);
  
  doc["active"] =  SCH.active;
  if(doc["active"] == "true"){
    doc["s_time"] =  SCH.s_time;
    doc["e_time"] =  SCH.e_time;
    doc["on_dur"] =  SCH.on_dur;
    doc["off_dur"] =  SCH.off_dur;
    doc["last_time"] =  SCH.last_time;
  }
  serializeJson(doc, schStr);

  SPIFFS.remove("/config_schedule.json");
  // Create the new one
  File f = SPIFFS.open("/config_schedule.json", "w");

  //Serial.println(schStr);
  f.print(schStr);
  f.close();
}

bool loadScheduleConfig() {
  File configFile = SPIFFS.open("/config_schedule.json", "r+");

  if (!configFile) {
    Serial.println("Error when opening the config file!");
    return false;
  }

  size_t size_config = configFile.size();
  if (size_config > 1024) {
    Serial.println("The file is too large!");
    return false;
  }
  
  const size_t bufferSize = JSON_OBJECT_SIZE(5) + 100;
  DynamicJsonDocument doc(bufferSize);
  
  DeserializationError error = deserializeJson(doc, configFile);
  if (error){
    switch (error.code()) {
    case DeserializationError::Ok:
        Serial.print(F("Deserialization succeeded"));
        break;
    case DeserializationError::IncompleteInput:
        Serial.print(F("IncompleteInput!"));
        break;
    case DeserializationError::TooDeep:
        Serial.print(F("TooDeep!"));
        break;
    case DeserializationError::NotSupported:
        Serial.print(F("NotSupported!"));
        break;
    case DeserializationError::InvalidInput:
        Serial.print(F("Invalid input!"));
        break;
    case DeserializationError::NoMemory:
        Serial.print(F("Not enough memory"));
        break;
    default:
        Serial.print(F("Deserialization failed"));
        break;
    }
    Serial.println("Error parsing schedule config file");
  } 
  configFile.close();
  
  if (doc.containsKey("active")) {
    SCH.active = doc["active"].as<String>();
  }if (doc.containsKey("s_time")) {
    SCH.s_time = doc["s_time"].as<String>();
  }if (doc.containsKey("e_time")) {
    SCH.e_time = doc["e_time"].as<String>();
  }if (doc.containsKey("on_dur")) {
    SCH.on_dur = doc["on_dur"].as<String>();
  }if (doc.containsKey("off_dur")) {
    SCH.off_dur = doc["off_dur"].as<String>();
  }if (doc.containsKey("last_time")) {
    SCH.last_time = doc["last_time"].as<String>();
  }
}
