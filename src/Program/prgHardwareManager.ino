//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

#define _REQUIRE_HARDWARE true

#include <P1AM.h>

//Hardware-specific parameters:
char statusByteArray[12];                //Array for second implementation where all bytes are read
bool statusBit;                     //Variable to temporarily store individual bits of our Status Bytes
const char* baseArray[] = { "P1-08TD2", "P1-08TRS", "P1-08ND3" }; //Expected Modules

void initHardwareManager(void) {
  log("Initializing Hardware And Communications");
  pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
  pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input

  if (_REQUIRE_HARDWARE) {
    while (!P1.init()) {
      log("Waiting For P1 Modules");
      delay(100);
    }
    //P1.printModules();                              //print out all currently signed-on modules to the console
    P1.getFwVersion();                              //print the Base Controller's firmware version to the console

    int baseErrors = P1.rollCall(baseArray, 3);     //Pass in list of modules and the number to check
    log(String("Base Errors: " + String(baseErrors)).c_str());
  } else {
    log("Hardware Disabled.");
  }

  log("Finished P1AM Initialization");

  tskHardwareManager.setCallback(&cyclicHardwareManager);

}

void cyclicHardwareManager(void) {
  logTaskTimer(&ts, "Hardware Manager");
  
}
