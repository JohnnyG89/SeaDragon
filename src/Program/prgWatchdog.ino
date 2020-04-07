//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

void initWatchdog(void) {
  log("Initializing Watchdog...");

  if (_ENABLE_WATCHDOG) {
    log("Enabling Watchdog Timer");
    P1.configWD(_WATCHDOG_TIMEOUT, TOGGLE);
    P1.startWD();
  } else {
    log("Not Enabling Watchdog Timer");
  }

  tskWatchdog.setCallback(&cyclicWatchdog);
  log("Initialized Watchdog.");
}

void cyclicWatchdog(void) {
  logTaskTimer(&ts, "Watchdog");

  if (_ENABLE_WATCHDOG) {
    log("Petting Watchdog...");
    P1.petWD();
  } else {
    tskWatchdog.disable();
  }
}
