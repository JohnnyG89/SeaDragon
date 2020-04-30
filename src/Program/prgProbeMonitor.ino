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
  Log.trace("ProbeMonitor:: Beginning ProbeMonitor Initialization");

  phSensor.begin(SMOOTHED_AVERAGE, 20);  

  ret = fitCurve(order, sizeof(x)/sizeof(double), x, y, sizeof(coeffs)/sizeof(double), coeffs);

  Log.verbose("ProbeMonitor:: Coefficients calculation status = %d :: Order = %d ::", ret, order);

  if(ret==0){
    for(int ii=0; ii <= order; ii++){
      Log.notice("ProbeMonitor:: Coefficient %d :: Value %F ::", ii, coeffs[ii]);
    }
  }else{
      Log.error("ProbeMonitor:: Failed to calculate coefficients.  Error status = %d ::", ret);
  }
  
  Scheduler::currentScheduler().currentTask().setCallback(&cyclicProbeMonitor);

  Log.trace("ProbeMonitor:: Finished ProbeMonitor Initialization");
}

void cyclicProbeMonitor(void) {
  phSensor.add(iAnalogValue);
  smoothedSensorValueAvg = phSensor.get();

  ipHSensorReading = 0;

  for(int ii=0; ii <= order; ii++){
    ipHSensorReading += (coeffs[ii]*(pow(smoothedSensorValueAvg, order-ii)));
  }

  Log.verbose("ProbeMonitor:: Original Sensor Value = %F :: Smoothed Float Value = %F :: Calibrated Value = %F", iAnalogValue, smoothedSensorValueAvg, ipHSensorReading);

  logTaskTimer("ProbeMonitor");
}
