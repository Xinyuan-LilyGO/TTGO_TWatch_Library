#ifndef HomeAutomationModule_h
#define HomeAutomationModule_h
#include "ModuleParent.h"

//function_id
#define SLIDESWITCH 0x01

class HomeAutomationModule: public ModuleParent{
public:
HomeAutomationModule();
private:
void processData();
};
extern HomeAutomationModule HomeAutomation;
#endif