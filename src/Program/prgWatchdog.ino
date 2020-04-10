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
    P1.configWD(WATCHDOG_TIMEOUT, TOGGLE);
    P1.startWD();
  } else {
    prglog("Not Enabling Watchdog Timer");
  }

  tskWatchdog.setCallback(&cyclicWatchdog);
  prglog("Initialized Watchdog.");
}

void cyclicWatchdog(void) {
  logTaskTimer(&ts_high, "Watchdog", "High");

  if (_ENABLE_WATCHDOG) {
//    prglog("Petting Watchdog...");
    P1.petWD();
  } else {
    tskWatchdog.disable();
  }
}
