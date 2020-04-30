//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

//Constants for the IO module slots
const uint8_t P1_SLOT_RTD =  1;
const uint8_t P1_SLOT_AIO =  2;
const uint8_t P1_SLOT_DO  =  3;
const uint8_t P1_SLOT_DI  =  4;
const uint8_t P1_SLOT_RO  =  5;

const uint8_t P1_MODULE_COUNT = 5;

//Hardware-specific parameters:
char statusByteArray[P1_MODULE_COUNT + 1][32];
char configInputBuffer[P1_MODULE_COUNT + 1][32];

const char* baseArray[] = { "P1-04RTD", "P1-4ADL2DAL-2", "P1-08TD2", "P1-08ND3", "P1-08TRS"}; //Expected Modules

//IO Bits- Wired into slices
channelLabel bin_DoorSwitch = {P1_SLOT_DI, 0};
channelLabel bout_CabinetLight = {P1_SLOT_RO, 2};
channelLabel bout_FreshwaterGooseneck = {P1_SLOT_RO, 1};
channelLabel bout_StackLight_Red = {P1_SLOT_DO, 1};
channelLabel bout_StackLight_Green = {P1_SLOT_DO, 2};
channelLabel bout_StackLight_Amber = {P1_SLOT_DO, 3};
channelLabel bout_StackLight_Blue = {P1_SLOT_DO, 4};
channelLabel ain_pH = {P1_SLOT_AIO, 1};
channelLabel ain_Temp = {P1_SLOT_RTD, 1};

//const char P1_04RTD_CONFIG[] = { 0x40, 0x03, 0x60, 0x05, 0x20, 0x07, 0x80, 0x00 };
const char P1_04RTD_CONFIG[] = { 0x40, 0x00, 0x60, 0x01, 0x20, 0x01, 0x80, 0x00 }; //Low-side burnout, temp in C, PT100, 33Hz filter

bool statusBit;                     //Variable to temporarily store individual bits of our Status Bytes

void initHardwareManager(void) {
  Log.trace("HardwareManager:: Initializing Hardware And Communications");
  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input

  if (!_REQUIRE_HARDWARE) {
    Scheduler::currentScheduler().currentTask().disable();
    Log.warning("HardwareManager:: Hardware Disabled.");
    return;
  }

  while (!P1.init()) {
    Log.fatal("HardwareManager:: Waiting For P1 Modules");
    delay(100);
  }
  //P1.printModules();                              //print out all currently signed-on modules to the console
  P1.getFwVersion();                              //print the Base Controller's firmware version to the console

  int baseErrors = P1.rollCall(baseArray, P1_MODULE_COUNT);     //Pass in list of modules and the number to check
  Log.notice("HardwareManager:: Base Errors=%x", baseErrors);

  Log.notice("HardwareManager:: Configuring Temperature Input, Slot=%d, Module=%s :: Value=%s ::", P1_SLOT_RTD, baseArray[P1_SLOT_RTD - 1], P1_04RTD_CONFIG);

  while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
  Log.notice("HardwareManager:: Configured Temperature Input, Slot %d, Module %s, Value %s", P1_SLOT_RTD, baseArray[P1_SLOT_RTD - 1], P1_04RTD_CONFIG);

  configCheck();
  statusCheck();

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicHardwareManager);

  Log.trace("HardwareManager::Finished P1AM Initialization");
}

void cyclicHardwareManager(void) {
  logTaskTimer("Hardware Manager");

  oCabinetLight = !iCabinetDoorSensor;
  oLEDIndicator = Scheduler::currentScheduler().currentTask().getRunCounter()%2==0;

  //Write previous cycle outputs:
  digitalWrite(LED_BUILTIN, oLEDIndicator); //Update the LED
  P1.writeDiscrete(oFreshwaterLightOutput, bout_FreshwaterGooseneck);
  P1.writeDiscrete(oCabinetLight, bout_CabinetLight);
  Log.verbose("HardwareManager:: Writing Outputs :: Freshwater Light=%T :: Cabinet Light=%T :: LED Indicator=%T ::", oFreshwaterLightOutput, oCabinetLight, oLEDIndicator);
  //Read this cycle's inputs:
  iCabinetDoorSensor = P1.readDiscrete(bin_DoorSwitch);
  iTemperature = P1.readTemperature(ain_Temp);
  iAnalogValue = P1.readAnalog(ain_pH);
  iSwitchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  Log.verbose("HardwareManager:: Reading Inputs :: Door Switch=%T :: Temperature=%F :: pH Analog Voltage=%d :: PLC Switch=%T", iCabinetDoorSensor, iTemperature, iAnalogValue, iSwitchState);
}

void statusCheck(void) {
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    P1.readStatus(statusByteArray[ii], ii);
    if (statusByteArray[ii] != 0) {
      Log.notice("HardwareManager:: Status Byte, Module %d, Value ");
    }
  }
}

void configCheck(void) {
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    //    prglog(String("HardwareManager::Reading Module Configuration, Module: " + String(ii) + "-" + String(baseArray[ii-1])).c_str());
    P1.readModuleConfig(configInputBuffer[ii], ii);
    //    prglog(String("HardwareManager::Module Configuration Read, Module: " + String(ii) + "-" + String(baseArray[ii-1]) + String(configInputBuffer[ii])).c_str());
  }
  Log.notice("HardwareManager::Finished Config Check");
}
