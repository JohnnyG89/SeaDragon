//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

//This task is meant to monitor the reading of the pH sensor:
//https://www.dfrobot.com/product-1782.html

//Wiring:
//Black: GND
//Red: +5V
//Blue: Analog Input

//Use moving avg. function to mitigate the effects of random noise
//Take that value and put it into the spline function, pH=f(Voltage;)


void initProbeMonitor(void) {
  prglog("Beginning ProbeMonitor Initialization");

  tskProbeMonitor.setCallback(&cyclicProbeMonitor);

  prglog("Finished ProbeMonitor Initialization");
}

void cyclicProbeMonitor(void) {
  logTaskTimer("ProbeMonitor");
}
