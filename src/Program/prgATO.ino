//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static        Logging    atoLog;

static const uint32_t   PULSE_CYCLE_uS            = 20000000;
static const uint32_t   PULSE_CYCLE_DIVIDER       = 32;
static const uint32_t   PULSE_TIME                = PULSE_CYCLE_uS / PULSE_CYCLE_DIVIDER;
static const uint32_t   PULSE_COUNT               = 7;

void initATO(void) {
  atoLog.begin(LOG_LEVEL_VERBOSE, &Serial);
  atoLog.setPrefix(printTimestamp);
  atoLog.setSuffix(printNewline);
  atoLog.notice("ATO:: Initialized ATO Log ::");
  atoLog.trace("ATO:: Initializing ATO...");

  if (!_ENABLE_ATO) {
    atoLog.warning("ATO:: Disabling ATO Task...");
    Scheduler::currentScheduler().currentTask().disable();
    return;
  }
  Scheduler::currentScheduler().currentTask().setCallback(&cyclicATO);
  atoLog.trace("ATO:: Initialized ATO");
}

void cyclicATO(void) {
  static uint32_t   Time_uS_Start       = 0;
  uint32_t          Time_uS_Elapsed     = 0;
  static R_TRIG     rTriggerPulseTrain;
  logTaskTimer("ATO", &Scheduler::currentScheduler().currentTask());


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
  
  //ATO functionality:
}
