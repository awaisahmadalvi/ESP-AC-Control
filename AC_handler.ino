
// AC Related URLs
void setACWebListeners(){
  
  server.serveStatic("/config_AC", SPIFFS, "/config_AC.json");
  server.on("/ac_state", HTTP_PUT, PUT_state);
  server.on("/ac_state", HTTP_GET, GET_state);
}

void PUT_state(){
  
  DynamicJsonDocument root(1024);
  DeserializationError error = deserializeJson(root, server.arg("plain"));
  if (error) {
    server.send(404, "text/plain", "FAIL. " + server.arg("plain"));
  } else {
    if (root.containsKey("temp")) {
      acState.temperature = (uint8_t) root["temp"];
    }

    if (root.containsKey("fan")) {
      acState.fan = (uint8_t) root["fan"];
    }

    if (root.containsKey("power")) {
      acState.powerStatus = root["power"];
    }

    if (root.containsKey("mode")) {
      acState.operation = root["mode"];
    }

    String output;
    serializeJson(root, output);
    saveACConfig();
    
    server.send(200, "text/plain", output);

    delay(200);

    if (acState.powerStatus) {
      ac.on();
      ac.setTemp(acState.temperature);
      if (acState.operation == 0) {
        ac.setMode(AUTO_MODE);
        ac.setFan(FAN_AUTO);
        acState.fan = 0;
      } else if (acState.operation == 1) {
        ac.setMode(COOL_MODE);
      } else if (acState.operation == 2) {
        ac.setMode(DRY_MODE);
      } else if (acState.operation == 3) {
        ac.setMode(HEAT_MODE);
      } else if (acState.operation == 4) {
        ac.setMode(FAN_MODE);
      }

      if (acState.operation != 0) {
        if (acState.fan == 0) {
          ac.setFan(FAN_AUTO);
        } else if (acState.fan == 1) {
          ac.setFan(FAN_MIN);
        } else if (acState.fan == 2) {
          ac.setFan(FAN_MED);
        } else if (acState.fan == 3) {
          ac.setFan(FAN_HI);
        }
      }
    } else {
      ac.off();
    }
    ac.send();
  }
}

void GET_state(){
  DynamicJsonDocument root(1024);
  root["mode"] = acState.operation;
  root["fan"] = acState.fan;
  root["temp"] = acState.temperature;
  root["power"] = acState.powerStatus;
  String output;
  serializeJson(root, output);
  Serial.print("Reply to AC GET: ");
  Serial.println(output);
  server.send(200, "text/plain", output);
}

void AC_Poweron(){
  ac.on();
  ac.send();
  acState.powerStatus = true;
  saveACConfig();
  Serial.println("AC_Poweron");
}

void AC_Poweroff(){
  ac.off();
  ac.send();
  acState.powerStatus = false;
  saveACConfig();
  Serial.println("AC_Poweroff");
}
