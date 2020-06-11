# ESP-AC-Control
Thanks to all blogs and libraries, which made that project easy:
  - ArduinoJson v6 - https://github.com/bblanchon/ArduinoJson
  - Arduino Time - https://github.com/PaulStoffregen/Time
  - Arduino Time Alarm - https://github.com/PaulStoffregen/TimeAlarms
  - NtpClientLib - https://github.com/gmag11/NtpClient
  - IRremoteESP8266 - https://github.com/crankyoldgit/IRremoteESP8266
  - ESP WiFiManager - https://github.com/tzapu/WiFiManager
  - StringSplitter - https://github.com/aharshac/StringSplitter


ESP8266 module is used to control Haier AC via web server and IR Emitter - ESP-01S
  - Web server is established to get commands.
  - Web server is also used to display recent AC parameters.
  - Web server can be used to setup schedule to switch AC on/off.
  - Seperate config files Schedule and AC.
  - Config saved in JSON format.
  - Implemented WiFiManager API for init setup.
  - Implemented mDNS API for Windows and Mac.
  - Implemented NTP API listener to update time at startup with PST +5.
  - Implemented OTA API for post-installation update.
  - ESP8266 GPIO pin 4 is used for IR blaster

This project works with Haier Air Conditioners currently. But, you can add support for your AC by changing the parameters in "Haier_AC.h" header file.
To check  support for particular Air Conditioner you can check https://github.com/crankyoldgit/IRremoteESP8266.
