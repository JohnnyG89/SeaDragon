//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"


TON MyFirstTimer(2000);

void initMyFirstTask(void) {
  prglog("Initializing Example Task");

  tskMyFirstTask.setCallback(&cyclicMyFirstTask);

  prglog("Initialized Example Task");

}

void cyclicMyFirstTask(void) {
  logTaskTimer(&ts_high, "MyFirstTask", "High");
  switchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  MyFirstTimer.process(switchState);
  digitalWrite(LED_BUILTIN, MyFirstTimer.Q); //Update the LED
}
