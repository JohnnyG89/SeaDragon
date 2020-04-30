//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

void initATO(void) {
  Log.trace("ATO:: Initializing ATO...");
  if (!_ENABLE_ATO) {
    Log.warning("ATO:: Disabling ATO Task...");
    Scheduler::currentScheduler().currentTask().disable();
    return;
  }
  Scheduler::currentScheduler().currentTask().setCallback(&cyclicATO);
  Log.trace("ATO:: Initialized ATO");
}

void cyclicATO(void) {
  logTaskTimer("ATO");
  Log.notice("ATO:: Disabling Task ::");
  Scheduler::currentScheduler().currentTask().disable();
}
