//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static        Logging    litLog;

void initLights(void) {
  if (!gTimeSet) {
    litLog.warning("Lights::Time not set. Trying again later.");
    return;
  }

  litLog.begin(LOG_LEVEL_VERBOSE, &Serial);
  litLog.setPrefix(printTimestamp);
  litLog.setSuffix(printNewline);
  litLog.notice("Lights:: Initialized Lights Log ::");
  litLog.trace("Lights:: Beginning Lights Initialization ::");

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicLights);
  litLog.trace("Lights:: Finished Lights Initialization ::");
}

void cyclicLights(void) {
  logTaskTimer("Lights", &Scheduler::currentScheduler().currentTask());

  CabinetLightControl();
  FreshwaterLightControl();

  //Set machine indicators
  oIndicatorLight_Red	=	gFaultWord != 0;
  oIndicatorLight_Green = gFaultWord == 0;
  oIndicatorLight_Blue = false;
}


static void CabinetLightControl(void) {
  static TOF		CabinetLight_TOF(2500);
  static R_TRIG	rtrgLogCabinetLight;
  static F_TRIG	ftrgLogCabinetLight;

  CabinetLight_TOF.process(!iCabinetDoorSensor);

  oCabinetLight = CabinetLight_TOF.Q;

  rtrgLogCabinetLight.process(oCabinetLight);
  ftrgLogCabinetLight.process(oCabinetLight);

  if (rtrgLogCabinetLight.Q || ftrgLogCabinetLight.Q) {
    litLog.notice("Lights:: Cabinet Light %T ::", oCabinetLight);
  }
}

static void FreshwaterLightControl(void) {
  ProcessTriggers("fsh", ioFreshwaterLight);

  ioFreshwaterLight.ctrl._Auto = gAlarmActiveArray[ALMID_FRESHWATER_LIGHTS];

  ioFreshwaterLight.out._Auto = ioFreshwaterLight.ctrl._Auto && ioFreshwaterLight.mode._Auto;
  ioFreshwaterLight.out._Manual = ioFreshwaterLight.ctrl._Manual && ioFreshwaterLight.mode._Manual;

//  litLog.verbose("Lights:: auto/manual/remote :: ctrl-%T-%T-%T :: mode-%T-%T-%T :: out-%T-%T-%T ::", 
//  
//  ioFreshwaterLight.ctrl._Auto, ioFreshwaterLight.ctrl._Manual, ioFreshwaterLight.ctrl._Remote, 
//  ioFreshwaterLight.mode._Auto, ioFreshwaterLight.mode._Manual, ioFreshwaterLight.mode._Remote,
//  ioFreshwaterLight.out._Auto, ioFreshwaterLight.out._Manual, ioFreshwaterLight.out._Remote
//  );
  
  if (ioFreshwaterLight.trig.mode.Auto._Fall) {
    ioFreshwaterLight.ctrl._Remote = false;
  }
  
  if (ioFreshwaterLight.trig.mode.Manual._Rise) {
    ioFreshwaterLight.ctrl._Remote = false;
  }
  
  if (ioFreshwaterLight.trig.mode.Manual._Fall) {
    ioFreshwaterLight.ctrl._Remote = false;
  }
  
  if (ioFreshwaterLight.trig.mode.Remote._Fall) {
    ioFreshwaterLight.ctrl._Remote = false;
  }
  
  if (ioFreshwaterLight.trig.ctrl.Remote._Rise) {
    ioFreshwaterLight.out._Remote = true;
  }
  
  if (ioFreshwaterLight.trig.ctrl.Remote._Fall) {
    ioFreshwaterLight.out._Remote = false;
  }


  ioFreshwaterLight._out = !(ioFreshwaterLight.out._Auto || ioFreshwaterLight.out._Manual || ioFreshwaterLight.out._Remote);

  static R_TRIG rtrgFreshwaterLightLog;
  static F_TRIG ftrgFreshwaterLightLog;
  rtrgFreshwaterLightLog.process(ioFreshwaterLight._out);
  ftrgFreshwaterLightLog.process(ioFreshwaterLight._out);

  if (rtrgFreshwaterLightLog.Q || ftrgFreshwaterLightLog.Q) {
    litLog.notice("Lights:: %s :: State %T :: (Auto/Manual/Remote) :: Control %T %T %T :: Mode %T %T %T :: Out %T %T %T ::", "FW", ioFreshwaterLight._out, ioFreshwaterLight.ctrl._Auto, ioFreshwaterLight.ctrl._Manual, ioFreshwaterLight.ctrl._Remote, ioFreshwaterLight.mode._Auto, ioFreshwaterLight.mode._Manual, ioFreshwaterLight.mode._Remote, ioFreshwaterLight.out._Auto, ioFreshwaterLight.out._Manual, ioFreshwaterLight.out._Remote);
  }
}

static void ProcessIndividualTrigger(const char* spec, bool& ctrlBit, rfTrig_typ& Latch) {
  char buff_rising[30];
  char buff_falling[30];
  sprintf(buff_rising, "%s/rise", spec);
  sprintf(buff_falling, "%s/fall", spec);

  Latch.Rising.process(ctrlBit);
  Latch.Falling.process(ctrlBit);

  Latch._Rise = Latch.Rising.Q;
  Latch._Fall = Latch.Falling.Q;
  Latch._Change = Latch._Rise || Latch._Fall;

  if (Latch._Rise) {
    litLog.verbose("Lights:: Rising Edge :: %s ::", buff_rising);
  }

  if (Latch._Fall) {
    litLog.verbose("Lights:: Falling Edge :: %s ::", buff_falling);
  }
}

static void ProcessModeTrigger(const char* Set, opMode_typ& trigger, rfTrigMode_typ& Latches) {
  char buff_auto[25];
  char buff_manual[25];
  char buff_remote[25];

  sprintf(buff_auto, "%s/auto", Set);
  sprintf(buff_manual, "%s/manual", Set);
  sprintf(buff_remote, "%s/remote", Set);

  ProcessIndividualTrigger(buff_auto, trigger._Auto, Latches.Auto);
  ProcessIndividualTrigger(buff_manual, trigger._Manual, Latches.Manual);
  ProcessIndividualTrigger(buff_remote, trigger._Remote, Latches.Remote);
}

static void ProcessTriggers(const char* Name, ioModule_typ& pmodule) {
  char buff_mode[20];
  char buff_ctrl[20];
  char buff_out[20];

  sprintf(buff_mode, "%s/mode", Name);
  sprintf(buff_ctrl, "%s/ctrl", Name);
  sprintf(buff_out, "%s/out", Name);

  ProcessModeTrigger(buff_mode, pmodule.mode, pmodule.trig.mode);
  ProcessModeTrigger(buff_ctrl, pmodule.ctrl, pmodule.trig.ctrl);
  ProcessModeTrigger(buff_out, pmodule.out, pmodule.trig.out);
}
