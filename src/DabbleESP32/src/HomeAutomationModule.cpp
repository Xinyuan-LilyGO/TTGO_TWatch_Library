#define FROM_DABBLE_LIBRARY
#include "DabbleESP32.h"
#include "HomeAutomationModule.h"
HomeAutomationModule::HomeAutomationModule():ModuleParent(HOMEAUTOMATION_ID){
}

void HomeAutomationModule::processData(){
  uint8_t pin  = getDabbleInstance().getArgumentData(0)[0];
  bool state   = getDabbleInstance().getArgumentData(0)[1];
 /*#ifdef DEBUG
  Serial.print("pin: ");
  Serial.print(pin);
  Serial.print("state: ");
  Serial.println(state);
  #endif*/
  pinMode(pin,OUTPUT);
  digitalWrite(pin,state);
} 
