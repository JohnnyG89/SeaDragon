//                      ____                                                                  _____       ______                   
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          | 
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      | 
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  | 
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``| 
//                                                                                                                                 
//                                                          Reef On

void initAlarms(void){
  prglog("Initializing Alarms...");
  
  tskAlarms.setCallback(&cyclicAlarms);
  
  prglog("Initialized Alarms");
}

void cyclicAlarms(void){
  logTaskTimer(&ts_high, "Alarms", "High");
  
}
