
//  ***** User Created *****
#include "Haier.h"

ESP8266WebServer server(80);

void setup() {

  Serial.begin(115200);

  // **************** Haier Setup ****************
  ac.begin();

  // **************** Flash Setup ****************
  Serial.println("mounting FS...");
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS Mount failed");
  } else {
    Serial.println("SPIFFS Mount succesfull");
  }
  delay(1000);

  // **************** WiFi Setup ****************
  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("ESP")) {
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  // **************** mDNS Setup ****************
  setup_mDNS();

  // **************** NTP Setup ****************
  NTP_setup();

  // **************** WebServer Setup ****************
  server.serveStatic("/", SPIFFS, "/index.html");

  setOTAWebListeners();
  setScheduleWebListeners();
  setACWebListeners();

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  loop_mDNS();
  ArduinoOTA.handle();
  server.handleClient();
  Alarm.delay(100); // wait one by 10 second between clock display
}

void setOTAWebListeners() {

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("haier");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.begin();
}

void handleNotFound() {

  if (!handleFileRead(server.uri())) {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
  }
}

bool handleFileRead(String path) {
  //  send the right file to the client (if it exists)
  //  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  // If a folder is requested, send the index file
  String contentType = getContentType(path);
  // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))  // If there's a compressed version available
      path += ".gz";  // Use the compressed verion
    File file = SPIFFS.open(path, "r");
    //  Open the file
    server.streamFile(file, contentType);
    //  Send it to the client
    file.close();
    // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  // If the file doesn't exist, return false
  return false;
}

String getContentType(String filename) {
  // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";

  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".csv")) return "text/csv";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".js"))  return "application/javascript";
  else if (filename.endsWith(".zip")) return "application/zip";
  else if (filename.endsWith(".gz"))  return "application/x-gzip";

  return "text/plain";
}
