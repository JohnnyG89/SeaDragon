//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static        Logging    equLog;

static const uint32_t PULSE_CYCLE_uS      = 20000000;
static const uint32_t PULSE_CYCLE_DIVIDER = 32;
static const uint32_t PULSE_TIME          = PULSE_CYCLE_uS / PULSE_CYCLE_DIVIDER;
static const uint32_t PULSE_COUNT         = 7;

void initEquipment(void) {
  equLog.begin(LOG_LEVEL_NOTICE, &Serial);
  equLog.setPrefix(printTimestamp);
  equLog.setSuffix(printNewline);
  equLog.notice("Equipment:: Initialized Equipment Log ::");
  equLog.trace("Equipment:: Initializing Equipment...");

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicEquipment);
  equLog.trace("ATO:: Initialized ATO");
}

void cyclicEquipment(void) {
  static uint32_t   Time_uS_Start       = 0;
  uint32_t          Time_uS_Elapsed     = 0;
  static R_TRIG     rTriggerPulseTrain;
  logTaskTimer("Equipment", &Scheduler::currentScheduler().currentTask());


  //This is kind of a "helloworld" type application
  //in that it can be done without any additional IO
  //It pulses the LED indicator on the front whenever
  //there is a rising edge on the front switch
  rTriggerPulseTrain.process(iSwitchState);

  if (rTriggerPulseTrain.Q) {
    Time_uS_Start = micros();
  }

  Time_uS_Elapsed = micros() - Time_uS_Start;

  uint32_t Current_Pulse_Cycle  = (Time_uS_Elapsed / PULSE_TIME);
  bool Cycle_Is_Odd             = (Current_Pulse_Cycle % 2) == 1;
  bool Cycle_Under_Count        = Current_Pulse_Cycle < (PULSE_COUNT * 2);
  oLEDIndicator                 = Cycle_Is_Odd && Cycle_Under_Count && (Time_uS_Elapsed < PULSE_CYCLE_uS);
  //TODO: Blink status code instead

  //Skimmer
  ioSkimmer._out = ioSkimmer.ctrl._Manual && !iSkimmerLevelHigh;
  bitWrite(gFaultWord, FLT_SKIMMER_LEVEL, 1);

  //ATO functionality:
  //Note: Optical sensor 0 is a "failsafe" to prevent the ATO from turning on if all 3 sensors are out of the water!
  if (!iTankOpticalSensor[0] && iTankOpticalSensor[1] && iTankOpticalSensor[2]) {
    ioATOPump._out = true;
  }

  //Throw a fault
  static TON tmrFaultOpticalLevelSensor(30000);
  tmrFaultOpticalLevelSensor.process(iTankOpticalSensor[0]);

  bitWrite(gFaultWord, FLT_ATO_OPTICAL, tmrFaultOpticalLevelSensor.Q);

  //Powerhead/Wavemaker:
  ioPowerhead._out = gAlarmActiveArray[ALMID_POWERHEAD];
  ioWavemaker._out = gAlarmActiveArray[ALMID_WAVEMAKER];

}
