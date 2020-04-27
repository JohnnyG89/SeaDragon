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
char statusByteArray[P1_MODULE_COUNT+1][16];
char configInputBuffer[P1_MODULE_COUNT+1][16];

bool statusBit;                     //Variable to temporarily store individual bits of our Status Bytes

void initHardwareManager(void) {
  prglog("Initializing Hardware And Communications");
  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input

  if (!_REQUIRE_HARDWARE) {
    tskHardwareManager.disable();
    prglog("Hardware Disabled.");
    return;
  }

    while (!P1.init()) {
      prglog("Waiting For P1 Modules");
      delay(100);
    }
    //P1.printModules();                              //print out all currently signed-on modules to the console
    P1.getFwVersion();                              //print the Base Controller's firmware version to the console

    int baseErrors = P1.rollCall(baseArray, P1_MODULE_COUNT);     //Pass in list of modules and the number to check
    prglog(String("Base Errors: " + String(baseErrors)).c_str());

    prglog(String("Configuring Temperature Input, Slot: " + String(P1_SLOT_RTD) + "-" + baseArray[P1_SLOT_RTD] + " With Value: " + String(P1_04RTD_CONFIG)).c_str());
    while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
    prglog("Configured Temperature Input");
    //
    //    prglog(String("Configuring pH Input, Slot: " + String(P1_SLOT_AIO) + "-" + baseArray[P1_SLOT_AIO] + " With Value: " + String(P1_04RTD_CONFIG)).c_str());
    //    while (!P1.configureModule(P1_04RTD_CONFIG, P1_SLOT_RTD)) {};
    //    prglog("Configured AIO Unit");

    configCheck();
    statusCheck();
    tskHardwareManager.setCallback(&cyclicHardwareManager);
  
  prglog("Finished P1AM Initialization");
}

void cyclicHardwareManager(void) {
  logTaskTimer("Hardware Manager");

  if (_CHECK_MODULE_STATUS) {
    statusCheck();
  }

  oCabinetLight = !iCabinetDoorSensor;

//  bitWrite(outDataBuffer, 0, oFreshwaterLightOutput);
//  bitWrite(outDataBuffer, 1, oCabinetLight);

  //Write previous cycle outputs:
  digitalWrite(LED_BUILTIN, oLEDIndicator); //Update the LED
//  P1.writeDiscrete(outDataBuffer, P1_SLOT_RO);
  P1.writeDiscrete(oFreshwaterLightOutput, bout_FreshwaterGooseneck);
  P1.writeDiscrete(oCabinetLight, bout_CabinetLight);
  prglog(String("Writing Outputs :: Freshwater Light=" + String(oFreshwaterLightOutput) + " :: Cabinet Light=" + String(oCabinetLight)).c_str());

  //Read this cycle's inputs:
  iCabinetDoorSensor = P1.readDiscrete(bin_DoorSwitch);
  iTemperature = P1.readTemperature(ain_Temp);
  iAnalogValue = P1.readAnalog(ain_pH);
  iSwitchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  //TODO: Fix IP Printing! Prints as char[] right now.

  prglog(String("Read Inputs :: Door Switch=" + String(iCabinetDoorSensor) + " :: Temperature=" + String(iTemperature) + " :: Analog=" + String(iAnalogValue) + " :: Switch=" + String(iSwitchState)).c_str());
  
//  prglog(String("Temperature: " + String(iTemperature) + " pH: " + String(iAnalogValue) + " Switch: " + String(iSwitchState) + " Do: " + String(Ethernet.localIP())).c_str());
//  Serial.println(Ethernet.localIP());

//  prglog("Wrote Hardware IO");
}

void statusCheck(void) {
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    P1.readStatus(statusByteArray[ii], ii);
    if (statusByteArray[ii] != 0) {
      prglog(String("Status Byte, Module:" + String(ii) + " Value: " + String(statusByteArray[ii])).c_str());
    }
  }
}

void configCheck(void) {
  return;
  for (int ii = 1; ii < P1_MODULE_COUNT + 1; ii++) {
    prglog(String("Reading Module Configuration, Module: " + String(ii) + "-" + String(baseArray[ii])).c_str());
    P1.readModuleConfig(configInputBuffer[ii], ii);
    prglog(String("Module Configuration Read, Module: " + String(ii) + "-" + String(baseArray[ii]) + String(configInputBuffer[ii])).c_str());
  }
  prglog("Finished Config Check");
}

//TODO:  SDU.h -> automatically install new project if it's on the SD Card.  Low Priority.

//
