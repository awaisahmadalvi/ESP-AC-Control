
void saveACConfig(){
  // Delete old version
  SPIFFS.remove("/config_AC.json");
  DynamicJsonDocument root(1024);
  root["mode"] = acState.operation;
  root["fan"] = acState.fan;
  root["temp"] = acState.temperature;
  root["power"] = acState.powerStatus;
  String output;
  serializeJson(root, output);
  // Create the new one
  File f = SPIFFS.open("/config_AC.json", "w");
  f.print(output);
  f.close();
}

String loadACConfig(){
  
  File  configFile = SPIFFS.open("/config_AC.json", "w");
      
  if (!configFile) {
    Serial.println("Error when opening the config_AC.json file!");
    return "";
  }
  

  size_t size = configFile.size();
  //Serial.println("File size: " + size);
  if (size > 1024) {
    Serial.println("The file is too large!");
    return "";
  }

  DynamicJsonDocument doc(1024);
  
  DeserializationError error = deserializeJson(doc, configFile);
  configFile.close();
  if (error) {
    Serial.println("Error parsing AC config file");
    server.send(404, "text/plain", "FAIL. " + server.arg("plain"));
  } else {
    if (doc.containsKey("temp")) {
      acState.temperature = (uint8_t) doc["temp"];
    }

    if (doc.containsKey("fan")) {
      acState.fan = (uint8_t) doc["fan"];
    }

    if (doc.containsKey("power")) {
      acState.powerStatus = doc["power"];
    }

    if (doc.containsKey("mode")) {
      acState.operation = doc["mode"];
    }
  }
}
