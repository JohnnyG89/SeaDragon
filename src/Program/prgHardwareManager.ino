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
  logTaskTimer(&ts_high, "Hardware Manager", "High");
  
}
