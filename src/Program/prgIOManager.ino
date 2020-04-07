//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

uint32_t inSlot1;
uint32_t outSlot2;


void initIOManager(void) {
  log("Initializing IO Manager...");
  
  tskIOManager.setCallback(&cyclicIOManager);
  
  log("Initialized IO Manager");
  
}

void cyclicIOManager(void) {
  logTaskTimer(&ts, "IOManager");

  //Write outputs from logic processed LAST cycle
  P1.writeDiscrete(outSlot2, 1, 0);

  //Read inputs for logic to be processed THIS cycle
  inSlot1 = P1.readDiscrete(3);
  inDiscrete_1 = bitRead(inSlot1, 0);
  inWaterLevelSensor_1 = bitRead(inSlot1, 1);
  inWaterLevelSensor_2 = bitRead(inSlot1, 2);
  inWaterLevelSensor_3 = bitRead(inSlot1, 3);
  
}
