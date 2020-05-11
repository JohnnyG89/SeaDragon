//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static        Logging    dspLog;
static const int MAXNUM_DOSEPUMPS = 2;

void initDosePumps(void) {
  dspLog.begin(LOG_LEVEL_NOTICE, &Serial);
  dspLog.setPrefix(printTimestamp);
  dspLog.setSuffix(printNewline);
  dspLog.notice("DosePumps:: Initialized DosePumps Log ::");
  dspLog.trace("DosePumps:: Beginning DosePumps Initialization ::");


  Scheduler::currentScheduler().currentTask().setCallback(&cyclicDosePumps);
  dspLog.trace("DosePumps:: Finished DosePumps Initialization ::");
}

void cyclicDosePumps(void) {
	static R_TRIG		rTriggerDosePumpOs_Manual[MAXNUM_DOSEPUMPS];
	static TP			DosePump_TP_Manual[MAXNUM_DOSEPUMPS];
	static R_TRIG		rTriggerDosePumpOs_Remote[MAXNUM_DOSEPUMPS];
	static TP			DosePump_TP_Remote[MAXNUM_DOSEPUMPS];
	static bool			DosePump_Alarm_Active[MAXNUM_DOSEPUMPS];
	static bool			DosePump_AutoMode_ON[MAXNUM_DOSEPUMPS];
	static bool			DosePump_ManualMode_ON[MAXNUM_DOSEPUMPS];
	static bool			DosePump_RemoteControl_ON[MAXNUM_DOSEPUMPS];
	static R_TRIG		DosePump_Trigger_Log[MAXNUM_DOSEPUMPS];
	logTaskTimer("DosePumps", &Scheduler::currentScheduler().currentTask());
	
//	for(int ii = 0; ii < MAXNUM_DOSEPUMPS; ii++){
//		//On the rising edge of the manual switch, activate a pulse
//		rTriggerDosePumpOs_Manual[ii].process(imode_manDosePump[ii]);
//		DosePump_TP_Manual[ii].process(rTriggerDosePumpOs_Manual[ii].Q);
//		
//		rTriggerDosePumpOs_Remote[ii].process(rmt_oDosePump[ii]);
//		DosePump_TP_Remote[ii].process(rTriggerDosePumpOs_Remote[ii].Q);
//		
//		//Check to see if each DosePump's alarm is active (note: 4*ii only works if you've numbered your alarms like I have, otherwise you can set each bit individually)
//		DosePump_Alarm_Active[ii] = (gAlarmActiveArray[ALMID_AUTOFEEDER_1_1 + 4*ii] || gAlarmActiveArray[ALMID_AUTOFEEDER_1_2 + 4*ii] || gAlarmActiveArray[ALMID_AUTOFEEDER_1_3 + 4*ii] || gAlarmActiveArray[ALMID_AUTOFEEDER_1_4 + 4*ii]);
//		
//		DosePump_AutoMode_ON[ii] = DosePump_Alarm_Active[ii] && imode_Auto;
//		DosePump_ManualMode_ON[ii] = DosePump_TP_Manual[ii].Q && !imode_Auto;
//		DosePump_RemoteControl_ON[ii] = DosePump_TP_Manual[ii].Q && imode_WebControl;
//		
//		oDosePump[ii] = DosePump_AutoMode_ON[ii] || DosePump_ManualMode_ON[ii] || DosePump_RemoteControl_ON[ii];
//		
//		DosePump_Trigger_Log[ii].process(oDosePump[ii]);
//		
//		if(DosePump_Trigger_Log[ii].Q){
//			dspLog.notice("DosePumps:: DosePump %d Triggered :: Auto Mode %T :: Manual Mode %T :: Remote Mode %T ::", ii, DosePump_AutoMode_ON[ii], DosePump_ManualMode_ON[ii], DosePump_RemoteControl_ON[ii]);
//		}
//		
//	}
}
