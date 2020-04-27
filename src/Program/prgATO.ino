//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

TON tmrMaxATOOnTime(10000);
bool oLEDBuffer[1000];

void initATO(void) {
  if (!_ENABLE_ATO) {
    prglog("Disabling ATO Task...");
    tskATO.disable();
    return;
  }

  prglog("Initializing ATO...");
  tskATO.setCallback(&cyclicATO);
  prglog("Initialized ATO");
}

void cyclicATO(void) {
  logTaskTimer("ATO");

  Scheduler &s = Scheduler::currentScheduler();
  Task &pTask = s.currentTask();
  
  oLEDIndicator = pTask.getRunCounter()%2==0;

//  //If all 3 sensors are out of water, turn on the ATO
//  if (!inWaterLevelSensor_1 && !inWaterLevelSensor_2 && !inWaterLevelSensor_3 && !tmrMaxATOOnTime.Q) {
//
//  }
}
