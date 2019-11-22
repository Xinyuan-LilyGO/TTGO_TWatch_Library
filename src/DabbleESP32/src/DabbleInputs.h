
#ifndef DabbleInputs_h
#define DabbleInputs_h

#include "ModuleParent.h"

#define Potentiometer_1   0x01
#define Potentiometer_2   0x02
#define Switches          0x03
//#define DEBUG
class DabbleInputs: public ModuleParent
{
public:
	//Constructor
	DabbleInputs();
	//Checker Functions
	uint16_t getPot1Data();
	uint16_t getPot2Data();
	
	uint8_t getSlideSwitch1Data();
	uint8_t getSlideSwitch2Data();
	
	bool getTactileSwitch1Data();
	bool getTactileSwitch2Data();
	
	
    bool getSlideSwitchStatus(uint8_t SS,uint8_t dir); //1 2 SS1 left       //2 2 SS2 left
	                                                   //1 3  SS1 right     //2 3 SS2 right
	                                                   //1 1  SS1 Off       //2 1 SS2 Off
	bool getTactileSwitchStatus(uint8_t TS);	//1 TS1  2 TS2											 
													 
	int  getPotValue(uint8_t Pot);
	

public:
	byte functionId;
	
	byte data_1=0;
	byte data_2=0;
	byte data_3=0;
	byte data_4=0;
	byte data_5=0;
	uint8_t state_ss1;
    uint8_t state_ss2;
	
	
	
private:
	
	
	void processData();
};

//Extern Object
extern DabbleInputs Inputs;
#endif 