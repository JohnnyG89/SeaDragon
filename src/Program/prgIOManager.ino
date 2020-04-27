////                      ____                                                                  _____       ______
////              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
////           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
////        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
////  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
////
////                                                          Reef On
//#include "Global_Includes.h"
//
//uint32_t inSlot1;
//uint32_t outSlot2;
//
//void initIOManager(void) {
//  prglog("Initializing IO Manager...");
//  
//  tskIOManager.setCallback(&cyclicIOManager);
//  
//  prglog("Initialized IO Manager");
//}
//
//void cyclicIOManager(void) {
//  logTaskTimer("IOManager");
//
//  
//
//  //TODO: IO Mapping!
////
////  //Write outputs from logic processed LAST cycle
////  P1.writeDiscrete(MyFirstTimer.Q, 3, 0);
////
////  //Read inputs for logic to be processed THIS cycle
////  inSlot1 = P1.readDiscrete(1);
////  inDiscrete_1 = bitRead(inSlot1, 0);
////
////  prglog(String("In: " + String(inSlot1) + " Out: " + String(inDiscrete_1)).c_str());
////  inWaterLevelSensor_1 = bitRead(inSlot1, 1);
////  inWaterLevelSensor_2 = bitRead(inSlot1, 2);
////  inWaterLevelSensor_3 = bitRead(inSlot1, 3);
//  
//}
//
////TODO: Integrate new hardware
