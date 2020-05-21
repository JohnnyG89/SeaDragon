//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

static Logging                    almLog;
DefineCalendarType(Calendar,      CALENDAR_MAX_NUM_EVENTS);
static Calendar                   MyCalendar;
static Chronos::Event::Occurrence occurrenceList[OCCURRENCES_LIST_SIZE];
static const int                  INTERVAL_PRINT_TODAY = 200;

void initAlarms(void) {
  if (!gTimeSet) {
    almLog.warning("Alarms::Time not set. Trying again later.");
    return;
  }
  almLog.begin(LOG_LEVEL_NOTICE, &Serial);
  almLog.setPrefix(printTimestamp);
  almLog.setSuffix(printNewline);
  almLog.notice("Alarms:: Initialized Alarms Log ::");
  almLog.trace("Alarms:: Initializing Alarms...");

  if (!_ENABLE_ALARMS) {
    Scheduler::currentScheduler().currentTask().disable();
    almLog.warning("Alarms:: Disabling Alarms Task..");
    return;
  }

  initCalendar();

  Scheduler::currentScheduler().currentTask().setCallback(&cyclicAlarms);

  almLog.trace("Alarms:: Initialized Alarms");
}

void cyclicAlarms(void) {
  logTaskTimer("Alarms", &Scheduler::currentScheduler().currentTask());

  if (!gTimeSet) {
    almLog.error("Alarms:: Time was unset.  Returning to Initialization Routine ::");
    Scheduler::currentScheduler().currentTask().setCallback(&initAlarms);
    return;
  }

  int numOngoing = MyCalendar.listOngoing(OCCURRENCES_LIST_SIZE, occurrenceList, Chronos::DateTime::now());

  if ((Scheduler::currentScheduler().currentTask().getRunCounter() % INTERVAL_PRINT_TODAY) == 0) {
    almLog.notice("Alarms:: Currently there are %d Alarms ongoing ::", numOngoing);
  }

  memset(gAlarmActiveArray, 0, CALENDAR_MAX_NUM_EVENTS * sizeof(bool));
  for (int ii = 0; ii < numOngoing; ii++) {
    gAlarmActiveArray[occurrenceList[ii].id] = true;
    if ((Scheduler::currentScheduler().currentTask().getRunCounter() % INTERVAL_PRINT_TODAY) == 0) {
      char buff_starttime[50];
      char buff_endtime[50];
      char buff_totaltime[50];
      CStringBuilder sb_starttime(buff_starttime, sizeof(buff_starttime));
      CStringBuilder sb_endtime(buff_endtime, sizeof(buff_endtime));
      CStringBuilder sb_totaltime(buff_totaltime, sizeof(buff_totaltime));
      occurrenceList[ii].start.printTo(sb_starttime);
      occurrenceList[ii].finish.printTo(sb_endtime);
      (occurrenceList[ii].finish - occurrenceList[ii].start).printTo(sb_totaltime);

      almLog.notice("Alarms:: Ongoing Alarm, id=%d - Name=%s :: Started %s :: Finishes %s :: Total Time %s ::", occurrenceList[ii].id, EventNames[occurrenceList[ii].id], buff_starttime, buff_endtime, buff_totaltime);
    }
  }
}

static void initCalendar() {
  //  Serial.println(Chronos::DateTime::now().second());
  char buff[50];
  CStringBuilder sb(buff, sizeof(buff));
  bool ret;

  //Print current time
  Chronos::DateTime NowTime = Chronos::DateTime::now();
  NowTime.printTo(sb);
  almLog.verbose("Alarms:: Right now it is %s :: Initializing Calendar Events ::", buff);
  sb.reset();

  //Initialize Freshwater Lights
  NowTime.next(Freshwater_Lights_OnTime).printTo(sb);
  ret = MyCalendar.add(FreshwaterLightCalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Hours :: Result %B ::", EventNames[ALMID_FRESHWATER_LIGHTS], buff, Freshwater_Lights_TotalTime.hours(), ret);
  sb.reset();

  //Initialize Saltwater Lights
  NowTime.next(Saltwater_Lights_OnTime).printTo(sb);
  ret = MyCalendar.add(SaltwaterLightCalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Hours :: Result %B ::", EventNames[ALMID_SALTWATER_LIGHTS], buff, Saltwater_Lights_TotalTime.hours(), ret);
  sb.reset();

  //Initialize Refugium Lights
  NowTime.next(Fuge_Lights_OnTime).printTo(sb);
  ret = MyCalendar.add(FugeLightCalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Hours :: Result %B ::", EventNames[ALMID_FUGE_LIGHT], buff, Fuge_Lights_TotalTime.hours(), ret);
  sb.reset();

  //Autofeeders;

  //Initialize Autofeeder 1 Instance 1
  NowTime.next(Autofeeder_1_1_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_1_1_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_1_1], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 1 Instance 2
  NowTime.next(Autofeeder_1_2_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_1_2_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_1_2], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 1 Instance 3
  NowTime.next(Autofeeder_1_3_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_1_3_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_1_3], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 1 Instance 4
  NowTime.next(Autofeeder_1_4_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_1_4_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_1_4], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 2 Instance 1
  NowTime.next(Autofeeder_2_1_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_2_1_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_2_1], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 2 Instance 2
  NowTime.next(Autofeeder_2_2_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_2_2_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_2_2], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 2 Instance 3
  NowTime.next(Autofeeder_2_3_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_2_3_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_2_3], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Autofeeder 2 Instance 4
  NowTime.next(Autofeeder_2_4_OnTime).printTo(sb);
  ret = MyCalendar.add(Autofeeder_2_4_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_AUTOFEEDER_2_4], buff, Autofeeder_Pulse_Time.seconds(), ret);
  sb.reset();


  //Dosing Pumps


  //Initialize DosePump 1 Instance 1
  NowTime.next(DosePump_1_1_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_1_1_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_1_1], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 1 Instance 2
  NowTime.next(DosePump_1_2_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_1_2_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_1_2], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 1 Instance 3
  NowTime.next(DosePump_1_3_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_1_3_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_1_3], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 1 Instance 4
  NowTime.next(DosePump_1_4_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_1_4_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_1_4], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 2 Instance 1
  NowTime.next(DosePump_2_1_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_2_1_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_2_1], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 2 Instance 2
  NowTime.next(DosePump_2_2_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_2_2_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_2_2], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 2 Instance 3
  NowTime.next(DosePump_2_3_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_2_3_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_2_3], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize DosePump 2 Instance 4
  NowTime.next(DosePump_2_4_OnTime).printTo(sb);
  ret = MyCalendar.add(DosePump_2_4_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Seconds :: Result %B ::", EventNames[ALMID_DOSEPUMP_2_4], buff, DosePump_Pulse_Time.seconds(), ret);
  sb.reset();

  //Initialize Powerhead Alarm
  NowTime.next(Powerhead_OnTime).printTo(sb);
  ret = MyCalendar.add(Powerhead_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Minutes :: Result %B ::", EventNames[ALMID_POWERHEAD], buff, Chronos::Span::Minutes(55).minutes(), ret);
  sb.reset();

  //Initialize Wavemaker Alarm
  NowTime.next(Wavemaker_OnTime).printTo(sb);
  ret = MyCalendar.add(Wavemaker_CalendarEvent);
  almLog.notice("Alarms:: Added Event - %s :: Turns On %s :: Duration %d Minutes :: Result %B ::", EventNames[ALMID_WAVEMAKER], buff, Chronos::Span::Minutes(50).minutes(), ret);
  sb.reset();
}
