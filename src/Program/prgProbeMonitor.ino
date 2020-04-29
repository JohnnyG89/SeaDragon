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

Smoothed <float> phSensor; 
const int order = 2;
double coeffs[order+1];
double y[3] = {4, 7, 10};
double x[3] = {2032, 1500, 1000};
int ret;


void initProbeMonitor(void) {
  prglog("ProbeMonitor::Beginning ProbeMonitor Initialization");

  phSensor.begin(SMOOTHED_AVERAGE, 50);  

  ret = fitCurve(order, sizeof(x)/sizeof(double), x, y, sizeof(coeffs)/sizeof(double), coeffs);

  prglog(String("ProbeMonitor::Coeff. Calc Status= " + String(ret) + " :: Order= " + String(order)).c_str());

  if(ret==0){
    prglog("Coefficients Are:");
    for(int ii=0; ii <= order; ii++){
      prglog(String("ProbeMonitor::Coeff.: " + String(ii) + " Value: " + String(coeffs[ii])).c_str());
    }
  }else{
    prglog(String("ProbeMonitor::Error calculating coefficients" + String(ret)).c_str());
  }

  tskProbeMonitor.setCallback(&cyclicProbeMonitor);

  prglog("ProbeMonitor::Finished ProbeMonitor Initialization");
}

void cyclicProbeMonitor(void) {
  phSensor.add(iAnalogValue);
  smoothedSensorValueAvg = phSensor.get();

  ipHSensorReading = 0;

  for(int ii=0; ii <= order; ii++){
    ipHSensorReading += (coeffs[ii]*(pow(smoothedSensorValueAvg, order-ii)));
  }
  
  prglog(String("ProbeMonitor::Original Sensor Value= " + String(iAnalogValue) + " :: Smoothed Float Value=" + String(smoothedSensorValueAvg) + " :: Calibrated Value= " + String(ipHSensorReading)).c_str());

  logTaskTimer("ProbeMonitor");
}
