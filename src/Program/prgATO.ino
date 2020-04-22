//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

TON tmrMaxATOOnTime(10000);

void initATO(void) {
  prglog("Initializing ATO...");
  
  tskATO.setCallback(&cyclicATO);
  
  prglog("Initialized ATO");
}

void cyclicATO(void) {
  logTaskTimer("ATO");
   tmrMaxATOOnTime.process(outATOPump);

  //If all 3 sensors are out of water, turn on the ATO
  if(!inWaterLevelSensor_1 && !inWaterLevelSensor_2 && !inWaterLevelSensor_3 && !tmrMaxATOOnTime.Q){
    
  }
}
