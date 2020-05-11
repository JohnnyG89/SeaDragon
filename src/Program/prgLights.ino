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
  litLog.begin(LOG_LEVEL_NOTICE, &Serial);
  litLog.setPrefix(printTimestamp);
  litLog.setSuffix(printNewline);
  litLog.notice("Lights:: Initialized Lights Log ::");
  litLog.trace("Lights:: Beginning Lights Initialization ::");

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicLights);
  litLog.trace("Lights:: Finished Lights Initialization ::");
}

void cyclicLights(void) {
	static TOF		CabinetLight_TOF(2500);
	logTaskTimer("Lights", &Scheduler::currentScheduler().currentTask());

	CabinetLight_TOF.process(!iCabinetDoorSensor);

	oCabinetLight = CabinetLight_TOF.Q;
	
	FreshwaterLightControl();

	
  //Set machine indicators
  oIndicatorLight_Red	=	gFaultWord != 0;
  oIndicatorLight_Green = imode_Auto && !imode_WebControl;
  oIndicatorLight_Blue = !imode_Auto || imode_WebControl;
}

static void FreshwaterLightControl(void){
	bool Freshwater_Light_Manual_Active = imode_manFreshwaterLight && !imode_Auto;
  static bool Freshwater_Light_Remote_Active;
  static bool Freshwater_Light_Auto_Active;
	
	static R_TRIG rtrgFreshwaterLightRemote;
	rtrgFreshwaterLightRemote.process(rmt_oFreshwaterLight && imode_WebControl);
	if(rtrgFreshwaterLightRemote.Q){
		Freshwater_Light_Remote_Active = true;
	}
	
	static F_TRIG ftrgFreshwaterLightRemote;
	ftrgFreshwaterLightRemote.process(rmt_oFreshwaterLight && imode_WebControl);
	if(ftrgFreshwaterLightRemote.Q){
		Freshwater_Light_Remote_Active = false;
	}
	
	static R_TRIG rtrgFreshwaterLightAuto;
	rtrgFreshwaterLightAuto.process(gAlarmActiveArray[ALMID_FRESHWATER_LIGHTS]);
	if(rtrgFreshwaterLightAuto.Q){
		Freshwater_Light_Auto_Active = true;
	}
	
	static F_TRIG ftrgFreshwaterLightAuto;
	ftrgFreshwaterLightAuto.process(gAlarmActiveArray[ALMID_FRESHWATER_LIGHTS]);
	if(ftrgFreshwaterLightAuto.Q){
		Freshwater_Light_Auto_Active = false;
	}
	
	static R_TRIG rtrgEnteredAutoMode;
	rtrgEnteredAutoMode.process(imode_Auto);
	if(rtrgEnteredAutoMode.Q){
		Freshwater_Light_Remote_Active = false;
		Freshwater_Light_Auto_Active = gAlarmActiveArray[ALMID_FRESHWATER_LIGHTS];
	}

  oFreshwaterLight = !(Freshwater_Light_Auto_Active || Freshwater_Light_Remote_Active || Freshwater_Light_Manual_Active);//Needs negating only since it's on an NC relay output!
	
	static R_TRIG rtrgFreshwaterLightLog;
	static F_TRIG ftrgFreshwaterLightLog;
	rtrgFreshwaterLightLog.process(oFreshwaterLight);
	ftrgFreshwaterLightLog.process(oFreshwaterLight);
	
//	if(rtrgFreshwaterLightLog.Q || ftrgFreshwaterLightLog.Q){
		litLog.notice("Lights:: Freshwater Light %T :: Auto Mode %T :: Light Auto ON %T :: Manual ON %T :: Remote ON %T :: Remote Switch %T ::", oFreshwaterLight,imode_Auto, Freshwater_Light_Auto_Active, Freshwater_Light_Manual_Active, Freshwater_Light_Remote_Active, rmt_oFreshwaterLight);
//	}
}
