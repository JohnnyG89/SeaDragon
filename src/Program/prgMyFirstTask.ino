//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

#include <plclib.h>

TON MyFirstTimer(2000);

void initMyFirstTask(void) {
  log("Initializing Example Task");

  tskMyFirstTask.setCallback(&cyclicMyFirstTask);

  log("Initialized Example Task");

}

void cyclicMyFirstTask(void) {
  logTaskTimer(&ts, "MyFirstTask");
  switchState = digitalRead(SWITCH_BUILTIN);//Read the state of the switch
  MyFirstTimer.process(switchState);
  digitalWrite(LED_BUILTIN, MyFirstTimer.Q); //Update the LED
}
