#ifndef RoboticArmModule_h
#define RoboticArmModule_h

#include "ModuleParent.h"

//Input Function ID
#define MOVEARM 0x01
class RoboticArmModule : public ModuleParent
{
public:
	//Constructor
	RoboticArmModule();
    void processData();
	uint8_t getAngleServo1();
	uint8_t getAngleServo2();
    private:
	uint8_t  servo1Angle = 0;
	uint8_t  servo2Angle = 0;
 };

//Extern Object
extern RoboticArmModule RoboticArm;
#endif 