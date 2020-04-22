//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

//Hardware-specific parameters:
char statusByteArray[12];                //Array for second implementation where all bytes are read
bool statusBit;                     //Variable to temporarily store individual bits of our Status Bytes

void initHardwareManager(void) {
  prglog("Initializing Hardware And Communications");
  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input

  if (_REQUIRE_HARDWARE) {
    while (!P1.init()) {
      prglog("Waiting For P1 Modules");
      delay(100);
    }
    //P1.printModules();                              //print out all currently signed-on modules to the console
    P1.getFwVersion();                              //print the Base Controller's firmware version to the console

    int baseErrors = P1.rollCall(baseArray, 3);     //Pass in list of modules and the number to check
    prglog(String("Base Errors: " + String(baseErrors)).c_str());
  } else {
    prglog("Hardware Disabled.");
  }

  prglog("Finished P1AM Initialization");

  tskHardwareManager.setCallback(&cyclicHardwareManager);

}

void cyclicHardwareManager(void) {
  logTaskTimer("Hardware Manager");
  //TODO: Check to make sure the base power is still active.  Return to init() if anything is lost
  //statusBit = P1.check24V(1);  //Read the missing 24V bit on slot 1.
  //bool underRangeBit = checkUnderRange(1,3) //Check under range bit on slot 1 channel 3.
  //bool overRangeBit = checkOverRange(1,3) //Check over range bit on slot 1 channel 3.
  //bool burnoutBit = checkBurnout(1,3) //Check under range bit on slot 1 channel 3.
  //P1.readStatus(statusByteArray,1); //Store all Status Bytes of slot 1 into statusByteArray
}

//TODO:  SDU.h -> automatically install new project if it's on the SD Card
