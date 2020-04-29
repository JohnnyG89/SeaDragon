//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

void initWatchdog(void) {
  prglog("Initializing Watchdog...");

  if (_ENABLE_WATCHDOG) {
    prglog("Enabling Watchdog Timer");
    P1.configWD(10000, HOLD);
    P1.startWD();
  } else {
    prglog("Not Enabling Watchdog Timer");
  }

  tskWatchdog.setCallback(&cyclicWatchdog);
  prglog("Initialized Watchdog.");
}

void cyclicWatchdog(void) {
  logTaskTimer("Watchdog");

  if (_ENABLE_WATCHDOG) {
    //    prglog("Petting Watchdog...");
    P1.petWD();
  } else {
    tskWatchdog.disable();
  }
}
