//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static      Logging		afsLog;
static const int MAXNUM_AUTOFEEDERS = 2;
void initAutoFeeders(void) {
  afsLog.begin(LOG_LEVEL_NOTICE, &Serial);
  afsLog.setPrefix(printTimestamp);
  afsLog.setSuffix(printNewline);
  afsLog.notice("AutoFeeders:: Initialized AutoFeeders Log ::");
  afsLog.trace("AutoFeeders:: Beginning AutoFeeders Initialization ::");

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicAutoFeeders);
  afsLog.trace("AutoFeeders:: Finished AutoFeeders Initialization ::");
}

void cyclicAutoFeeders(void) {
  static R_TRIG		rTriggerAutoFeederOs_Manual[MAXNUM_AUTOFEEDERS];
  static TP			  Autofeeder_TP_Manual[MAXNUM_AUTOFEEDERS];
  static R_TRIG		rTriggerAutoFeederOs_Remote[MAXNUM_AUTOFEEDERS];
  static TP			  Autofeeder_TP_Remote[MAXNUM_AUTOFEEDERS];
  static bool			Autofeeder_Alarm_Active[MAXNUM_AUTOFEEDERS];
  static bool			Autofeeder_AutoMode_ON[MAXNUM_AUTOFEEDERS];
  static bool			Autofeeder_ManualMode_ON[MAXNUM_AUTOFEEDERS];
  static bool			Autofeeder_RemoteControl_ON[MAXNUM_AUTOFEEDERS];
  static R_TRIG		Autofeeder_Trigger_Log[MAXNUM_AUTOFEEDERS];
  logTaskTimer("AutoFeeders", &Scheduler::currentScheduler().currentTask());

  //	for(int ii = 0; ii < MAXNUM_AUTOFEEDERS; ii++){
  //		//On the rising edge of the manual switch, activate a pulse
  //		rTriggerAutoFeederOs_Manual[ii].process(imode_manAutoFeeder[ii]);
  //		Autofeeder_TP_Manual[ii].process(rTriggerAutoFeederOs_Manual[ii].Q);
  //
  //		rTriggerAutoFeederOs_Remote[ii].process(rmt_oAutoFeeder[ii]);
  //		Autofeeder_TP_Remote[ii].process(rTriggerAutoFeederOs_Remote[ii].Q);
  //
  //		//Check to see if each autofeeder's alarm is active (note: 4*ii only works if you've numbered your alarms like I have, otherwise you can set each bit individually)
  //		Autofeeder_Alarm_Active[ii] = (gAlarmActiveArray[ALMID_AUTOFEEDER_1_1 + 4*ii] || gAlarmActiveArray[ALMID_AUTOFEEDER_1_2 + 4*ii] || gAlarmActiveArray[ALMID_AUTOFEEDER_1_3 + 4*ii] || gAlarmActiveArray[ALMID_AUTOFEEDER_1_4 + 4*ii]);
  //
  //		Autofeeder_AutoMode_ON[ii] = Autofeeder_Alarm_Active[ii] && imode_Auto;
  //		Autofeeder_ManualMode_ON[ii] = Autofeeder_TP_Manual[ii].Q && !imode_Auto;
  //		Autofeeder_RemoteControl_ON[ii] = Autofeeder_TP_Manual[ii].Q && imode_WebControl;
  //
  //		oAutofeeder[ii] = Autofeeder_AutoMode_ON[ii] || Autofeeder_ManualMode_ON[ii] || Autofeeder_RemoteControl_ON[ii];
  //
  //		Autofeeder_Trigger_Log[ii].process(oAutofeeder[ii]);
  //
  //		if(Autofeeder_Trigger_Log[ii].Q){
  //			afsLog.notice("AutoFeeders:: Autofeeder %d Triggered :: Auto Mode %T :: Manual Mode %T :: Remote Mode %T ::", ii, Autofeeder_AutoMode_ON[ii], Autofeeder_ManualMode_ON[ii], Autofeeder_RemoteControl_ON[ii]);
  //		}
  //
  //	}
}
