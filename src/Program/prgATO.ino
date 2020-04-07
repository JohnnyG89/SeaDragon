//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

#include <plclib.h>

TON tmrMaxATOOnTime(10000);

void initATO(void) {
  log("Initializing ATO...");
  
  tskATO.setCallback(&cyclicATO);
  
  log("Initialized ATO");
}

void cyclicATO(void) {
  logTaskTimer(&ts, "ATO");
   tmrMaxATOOnTime.process(outATOPump);

  //If all 3 sensors are out of water, turn on the ATO
  if(!inWaterLevelSensor_1 && !inWaterLevelSensor_2 && !inWaterLevelSensor_3 && !tmrMaxATOOnTime.Q){
    
  }
}
