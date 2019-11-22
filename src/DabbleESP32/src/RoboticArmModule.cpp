#define FROM_DABBLE_LIBRARY
#include "DabbleESP32.h"
#include "RoboticArmModule.h"
RoboticArmModule::RoboticArmModule() : ModuleParent(ROBOTICARM_ID)
{
}

void RoboticArmModule::processData()
{
  servo1Angle = getDabbleInstance().getArgumentData(0)[0]; 
  Serial.print(servo1Angle);
  Serial.print("  ");
  servo2Angle = getDabbleInstance().getArgumentData(1)[0];
  Serial.println(servo2Angle);
}

uint8_t RoboticArmModule::getAngleServo1()
{
	return servo1Angle;
}

uint8_t RoboticArmModule::getAngleServo2()
{
	return servo2Angle;
}