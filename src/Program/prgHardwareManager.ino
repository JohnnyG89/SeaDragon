//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

//TODO: Move channel declarations here

//Hardware-specific parameters:
char statusByteArray[P1_MODULE_COUNT + 1][32];
char configInputBuffer[P1_MODULE_COUNT + 1][32];

bool statusBit;                     //Variable to temporarily store individual bits of our Status Bytes

void initHardwareManager(void) {
  prglog("Initializing Hardware And Communications");
  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input

  if (!_REQUIRE_HARDWARE) {
    tskHardwareManager.disable();
    prglog("HardwareManager::Hardware Disabled.");
    return;
  }

  while (!P1.init()) {
    prglog("HardwareManager::Waiting For P1 Modules");
    delay(100);
  }
  //P1.printModules();                              //print out all currently signed-on modules to the console
  P1.getFwVersion();                              //print the Base Controller's firmware version to the console

  int baseErrors = P1.rollCall(baseArray, P1_MODULE_COUNT);     //Pass in list of modules and the number to check
  prglog(String("HardwareManager::Base Errors: " + String(baseErrors)).c_str());

  prglog(String("HardwareManager::Configuring Temperature Input, Slot: " + String(P1_SLOT_RTD) + "-" + baseArray[P1_SLOT_RTD-1] + " With Value: " + String(P1_04RTD_CONFIG)).c_str());
  while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
  prglog("HardwareManager::Configured Temperature Input");
  //
  //    prglog(String("Configuring pH Input, Slot: " + String(P1_SLOT_AIO) + "-" + baseArray[P1_SLOT_AIO-1] + " With Value: " + String(P1_04RTD_CONFIG)).c_str());
  //    while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
  //    prglog("Configured AIO Unit");

  configCheck();
  statusCheck();
  tskHardwareManager.setCallback(&cyclicHardwareManager);

  prglog("HardwareManager::Finished P1AM Initialization");
}

void cyclicHardwareManager(void) {
  logTaskTimer("Hardware Manager");

  if (_CHECK_MODULE_STATUS) {
    statusCheck();
  }

  oCabinetLight = !iCabinetDoorSensor;

  //Write previous cycle outputs:
  digitalWrite(LED_BUILTIN, oLEDIndicator); //Update the LED
  //  P1.writeDiscrete(outDataBuffer, P1_SLOT_RO);
  P1.writeDiscrete(oFreshwaterLightOutput, bout_FreshwaterGooseneck);
  P1.writeDiscrete(oCabinetLight, bout_CabinetLight);
//  prglog(String("Writing Outputs :: Freshwater Light=" + String(oFreshwaterLightOutput) + " :: Cabinet Light=" + String(oCabinetLight) + " :: LED Indicator=" + String(oLEDIndicator)).c_str());

  //Read this cycle's inputs:
  iCabinetDoorSensor = P1.readDiscrete(bin_DoorSwitch);
  iTemperature = P1.readTemperature(ain_Temp);
  iAnalogValue = P1.readAnalog(ain_pH);
  iSwitchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  //TODO: Fix IP Printing! Prints as char[] right now.

//  prglog(String("HardwareManager::Read Inputs :: Door Switch=" + String(iCabinetDoorSensor) + " :: Temperature=" + String(iTemperature) + " :: Analog=" + String(iAnalogValue) + " :: Switch=" + String(iSwitchState)).c_str());

}

void statusCheck(void) {
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    P1.readStatus(statusByteArray[ii], ii);
    if (statusByteArray[ii] != 0) {
      prglog(String("HardwareManager::Status Byte, Module:" + String(ii) + " Value: " + String(statusByteArray[ii])).c_str());
    }
  }
}

void configCheck(void) {
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    prglog(String("HardwareManager::Reading Module Configuration, Module: " + String(ii) + "-" + String(baseArray[ii-1])).c_str());
    P1.readModuleConfig(configInputBuffer[ii], ii);
    prglog(String("HardwareManager::Module Configuration Read, Module: " + String(ii) + "-" + String(baseArray[ii-1]) + String(configInputBuffer[ii])).c_str());
  }
  prglog("HardwareManager::Finished Config Check");
}

//TODO:  SDU.h -> automatically install new project if it's on the SD Card.  Low Priority.

//
