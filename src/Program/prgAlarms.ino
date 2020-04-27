////                      ____                                                                  _____       ______
////              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
////           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
////        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
////  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
////
////                                                          Reef On
//#include "Global_Includes.h"
//
////
//////setSyncProvider(mySyncFunction); //TODO:  Do we need this?
//DefineCalendarType(Calendar, CALENDAR_MAX_NUM_EVENTS);
//Calendar MyCalendar;
//
//void initAlarms(void) {
//
//  if(!_ENABLE_ALARMS){
//    tskAlarms.disable();
//    prglog("Disabling Alarms Task..");
//    return;
//  }
//  
//  prglog("Initializing Alarms...");
//
//  //TODO:  Not 100% sure we need this....
//  //  Chronos::DateTime::setTime(rtc.getYear(), rtc.getMonth(), rtc.getDay(), rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
//
//  if (!gTimeSet) {
//    prglog("Time hasn't been set; Returning to alarm initialization on next pass");
//    return;
//  }
//
//  prglog("Chronos Says Time Is:");
//  Chronos::DateTime::now().printTo(Serial);
//
//  if(_RAND_CAL_DATA){
//    CalendarEventExample();
//  }
//
//  tskAlarms.setCallback(&cyclicAlarms);
//
//  prglog("Initialized Alarms");
//}
//
//void cyclicAlarms(void) {
//  logTaskTimer("Alarms");
//  
//  if (!gTimeSet) {
//    prglog("Time was unset.  Returning to Initialization Routine");
//    tskAlarms.setCallback(&initAlarms);
//    return;
//  }
//
//  Chronos::DateTime nowTime(Chronos::DateTime::now());
//  Chronos::Event::Occurrence  occurrenceList[OCCURRENCES_LIST_SIZE];
//
//  int numOngoing = MyCalendar.listOngoing(OCCURRENCES_LIST_SIZE, occurrenceList, nowTime);
//
//  if(!_PRINT_ALARM_INFO){
//    return;
//  }
//
//  if (numOngoing) {
//    prglog(String("Current Number Of Events:" + String(numOngoing)).c_str());
//
//    for (int ii = 0; ii < numOngoing; ii++) {
//      prglog(String("Event Ongoing::" + String(occurrenceList[ii].id) + "::" + EventNames[occurrenceList[ii].id] + "::Ends::").c_str());
//      (nowTime - occurrenceList[ii].finish).printTo(Serial);
//    }
//  } else {
//    prglog("No Active Events");
//  }
//
//  int numUpcoming = MyCalendar.listNext(OCCURRENCES_LIST_SIZE, occurrenceList, nowTime);
//  if (numUpcoming) {
//    for (int jj = 0; jj < numUpcoming; jj++) {
//      Chronos::Span::Absolute startsIn = (occurrenceList[jj].start - nowTime);
//      prglog(String("Event Upcoming::" + String(occurrenceList[jj].id) + "::" + EventNames[occurrenceList[jj].id] + "::Ends::").c_str());
//      occurrenceList[jj].start.printTo(Serial);
//      occurrenceList[jj].finish.printTo(Serial);
//    }
//  }
//}
//
//void CalendarEventExample(void) {
//  // so here's the meeting, an event created with a DateTime and a Span
//  MyCalendar.add(Chronos::Event(1, Chronos::DateTime(2015, 12, 21, 17, 00),
//                                Chronos::Span::Minutes(33))
//                );
//
//
//
//  // don't forget the daily workout, a daily recurring event every morning at 9:
//  // another Event constructed with (DateTime, Span)
//  MyCalendar.add(
//    Chronos::Event(2, Chronos::Mark::Daily(9, 00, 00),
//                   Chronos::Span::Minutes(45)));
//
//
//
//  // that longer meeting, what a drag...
//  // This one is constructed using bounding DateTimes for
//  // the start and finish
//  MyCalendar.add(
//    Chronos::Event(3, Chronos::DateTime(2015, 12, 21, 18, 00),
//                   Chronos::DateTime(2015, 12, 22, 1, 00)));
//
//
//  // weekly class, repeating every monday for one hour
//  // an Event created using a repeating Mark, and a Span
//  MyCalendar.add(
//    Chronos::Event(4,
//                   Chronos::Mark::Weekly(Chronos::Weekday::Monday, 10, 30, 00),
//                   Chronos::Span::Hours(1)));
//
//  // new year's eve party!
//  MyCalendar.add(
//    Chronos::Event(5, Chronos::DateTime(2015, 12, 31, 21, 00),
//                   Chronos::Span::Days(2)));
//
//
//  // anniversary dinner... could set this up as a yearly event
//  // but since the actual time for dinner is likely to change,
//  // we just do this year's:
//  MyCalendar.add(
//    Chronos::Event(6, Chronos::DateTime(2015, 12, 28, 19, 15),
//                   Chronos::Span::Minutes(90)));
//
//  // and *never* miss a monthly meeting of the council of the elders
//  MyCalendar.add(
//    Chronos::Event(7, Chronos::Mark::Monthly(2, 19, 0, 0),
//                   Chronos::Span::Hours(2)));
//}
