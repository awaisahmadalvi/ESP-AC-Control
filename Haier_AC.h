/*
 * 
 * ######## User configuration space for AC library classes ##########
 * 
 */


#include <ir_Haier.h>  //  replace library based on your AC unit model, check https://github.com/crankyoldgit/IRremoteESP8266

#define AUTO_MODE kHaierAcYrw02Auto
#define COOL_MODE kHaierAcYrw02Cool
#define DRY_MODE kHaierAcYrw02Dry
#define HEAT_MODE kHaierAcYrw02Heat
#define FAN_MODE kHaierAcYrw02Fan

#define FAN_AUTO kHaierAcYrw02FanAuto
#define FAN_MIN kHaierAcYrw02FanLow
#define FAN_MED kHaierAcYrw02FanMed
#define FAN_HI kHaierAcYrw02FanHigh

#define AC_ON true
#define AC_OFF false

// ESP8266 GPIO pin to use for IR blaster.
const uint16_t kIrLed = 4;
// Library initialization, change it according to the imported library file.
IRHaierACYRW02 ac(kIrLed);

/// ##### End user configuration ######
struct state {
  uint8_t temperature = 22, fan = 0, operation = 0;
  bool powerStatus;
};
