//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging prbLog;

//This task is meant to monitor the reading of the pH sensor:
//https://www.dfrobot.com/product-1782.html

//Wiring:
//Black: GND
//Red: +5V
//Blue: Analog Input

//Use moving avg. function to mitigate the effects of random noise
//Take that value and put it into the spline function, pH=f(Voltage;)

static        Smoothed<float>  TempSensor;
static        Smoothed<float>  phSensor;

static const  int              order              = 2;
static        double           coeffs[order + 1];
static        double           y[3]               = {4, 7, 10};
static        double           x[3]               = {2032, 1500, 1000};

void initProbeMonitor(void) {
  prbLog.begin(LOG_LEVEL_NOTICE, &Serial);
  prbLog.setPrefix(printTimestamp);
  prbLog.setSuffix(printNewline);
  prbLog.notice("ProbeMonitor:: Initialized ProbeMonitor Log ::");
  prbLog.trace("ProbeMonitor:: Beginning ProbeMonitor Initialization");

  phSensor.begin    (SMOOTHED_AVERAGE, 20);
  TempSensor.begin  (SMOOTHED_AVERAGE, 20);

  int ret = fitCurve(order, sizeof(x) / sizeof(double), x, y, sizeof(coeffs) / sizeof(double), coeffs);

  prbLog.notice("ProbeMonitor:: Coefficients calculation status = %d :: Order = %d ::", ret, order);

  if (ret == 0) {
    for (int ii = 0; ii <= order; ii++) {
      prbLog.notice("ProbeMonitor:: Coefficient %d :: Value %F ::", ii, coeffs[ii]);
    }
  } else {
    prbLog.fatal("ProbeMonitor:: Failed to calculate coefficients.  Error status = %d ::", ret);
  }

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicProbeMonitor);
  prbLog.trace("ProbeMonitor:: Finished ProbeMonitor Initialization");
}

void cyclicProbeMonitor(void) {
  logTaskTimer("ProbeMonitor", &Scheduler::currentScheduler().currentTask());
  phSensor.add(iAnalogValue);
  TempSensor.add(iTemperature);
  smoothedSensorValueAvg = phSensor.get();
  iTemperature_Smoothed = TempSensor.get();

  ipHSensorReading = 0;

  for (int ii = 0; ii <= order; ii++) {
    ipHSensorReading += (coeffs[ii] * (pow(smoothedSensorValueAvg, order - ii)));
  }
  prbLog.verbose("ProbeMonitor:: Original Sensor Value = %F :: Smoothed Float Value = %F :: Calibrated Value = %F", iAnalogValue, smoothedSensorValueAvg, ipHSensorReading);
}
