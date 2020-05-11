static const int CALENDAR_MAX_NUM_EVENTS   = 20;
static const int OCCURRENCES_LIST_SIZE    = 30;
bool             gAlarmActiveArray[CALENDAR_MAX_NUM_EVENTS];
//Constants for the alarm active array
const int       ALMID_FRESHWATER_LIGHTS 	= 0;
const int       ALMID_SALTWATER_LIGHTS 		= 1;
const int       ALMID_FUGE_LIGHT 					= 2;
const int       ALMID_AUTOFEEDER_1_1 			= 3;
const int       ALMID_AUTOFEEDER_1_2			= 4;
const int       ALMID_AUTOFEEDER_1_3			= 5;
const int       ALMID_AUTOFEEDER_1_4			= 6;
const int       ALMID_AUTOFEEDER_2_1 			= 7;
const int       ALMID_AUTOFEEDER_2_2			= 8;
const int       ALMID_AUTOFEEDER_2_3			= 9;
const int       ALMID_AUTOFEEDER_2_4			= 10;
const int       ALMID_DOSEPUMP_1_1 				= 11;
const int       ALMID_DOSEPUMP_1_2 				= 12;
const int       ALMID_DOSEPUMP_1_3 				= 13;
const int       ALMID_DOSEPUMP_1_4 				= 14;
const int       ALMID_DOSEPUMP_2_1 				= 15;
const int       ALMID_DOSEPUMP_2_2 				= 16;
const int       ALMID_DOSEPUMP_2_3 				= 17;
const int       ALMID_DOSEPUMP_2_4 				= 18;

const char * EventNames[] = {
  "Freshwater Lights",
  "Saltwater Lights",
  "Refugium Lights",
  "Autofeeder 1-1",
  "Autofeeder 1-2",
  "Autofeeder 1-3",
  "Autofeeder 1-4",
  "Autofeeder 2-1",
  "Autofeeder 2-2",
  "Autofeeder 2-3",
  "Autofeeder 2-4",
  "Dose Pump 1-1",
  "Dose Pump 1-2",
  "Dose Pump 1-3",
  "Dose Pump 1-4",
  "Dose Pump 2-1",
  "Dose Pump 2-2",
  "Dose Pump 2-3",
  "Dose Pump 2-4",
  NULL
};



//Add the freshwater light scheduling alarm
Chronos::Mark::Daily 		Freshwater_Lights_OnTime(8, 0, 0);
Chronos::Span::Hours 		Freshwater_Lights_TotalTime(15);
Chronos::Event 			    FreshwaterLightCalendarEvent(	  ALMID_FRESHWATER_LIGHTS,  Freshwater_Lights_OnTime,   Freshwater_Lights_TotalTime);

//Add the Saltwater light scheduling alarm
Chronos::Mark::Daily 		Saltwater_Lights_OnTime(7, 0, 0);
Chronos::Span::Hours 		Saltwater_Lights_TotalTime(16);
Chronos::Event 			    SaltwaterLightCalendarEvent(	  ALMID_SALTWATER_LIGHTS,   Saltwater_Lights_OnTime,    Saltwater_Lights_TotalTime);

//Add the Refugium light scheduling alarm
Chronos::Mark::Daily 		Fuge_Lights_OnTime(22, 0, 0);
Chronos::Span::Hours 		Fuge_Lights_TotalTime(8);
Chronos::Event 			    FugeLightCalendarEvent(			    ALMID_FUGE_LIGHT,         Fuge_Lights_OnTime,         Fuge_Lights_TotalTime);


//Autofeeder Setup
Chronos::Span::Seconds Autofeeder_Pulse_Time(30);

//  //Add the Carnivore Autofeeder (1-1) Scheduler
Chronos::Mark::Daily 		Autofeeder_1_1_OnTime(11, 0, 0);
Chronos::Event 			    Autofeeder_1_1_CalendarEvent(	ALMID_AUTOFEEDER_1_1, 	Autofeeder_1_1_OnTime, 	Autofeeder_Pulse_Time);

//  //Add the Carnivore Autofeeder (1-2) Scheduler
Chronos::Mark::Daily 		Autofeeder_1_2_OnTime(13, 0, 0);
Chronos::Event 			    Autofeeder_1_2_CalendarEvent(	ALMID_AUTOFEEDER_1_2, 	Autofeeder_1_2_OnTime, 	Autofeeder_Pulse_Time);

//  //Add the Carnivore Autofeeder (1-3) Scheduler
Chronos::Mark::Daily 		Autofeeder_1_3_OnTime(15, 0, 0);
Chronos::Event 			    Autofeeder_1_3_CalendarEvent(	ALMID_AUTOFEEDER_1_3, 	Autofeeder_1_3_OnTime, 	Autofeeder_Pulse_Time);

//  //Add the Carnivore Autofeeder (1-4) Scheduler
Chronos::Mark::Daily 		Autofeeder_1_4_OnTime(17, 0, 0);
Chronos::Event 			    Autofeeder_1_4_CalendarEvent(	ALMID_AUTOFEEDER_1_4, 	Autofeeder_1_4_OnTime, 	Autofeeder_Pulse_Time);


//  //Add the Herbivore Autofeeder (2-1) Scheduler
Chronos::Mark::Daily 		Autofeeder_2_1_OnTime(12, 0, 0);
Chronos::Event 			    Autofeeder_2_1_CalendarEvent(	ALMID_AUTOFEEDER_2_1, 	Autofeeder_2_1_OnTime, 	Autofeeder_Pulse_Time);

//  //Add the Herbivore Autofeeder (2-2) Scheduler
Chronos::Mark::Daily 		Autofeeder_2_2_OnTime(14, 0, 0);
Chronos::Event 			    Autofeeder_2_2_CalendarEvent(	ALMID_AUTOFEEDER_2_2, 	Autofeeder_2_2_OnTime, 	Autofeeder_Pulse_Time);

//  //Add the Herbivore Autofeeder (2-3) Scheduler
Chronos::Mark::Daily 		Autofeeder_2_3_OnTime(16, 0, 0);
Chronos::Event 			    Autofeeder_2_3_CalendarEvent(	ALMID_AUTOFEEDER_2_3, 	Autofeeder_2_3_OnTime, 	Autofeeder_Pulse_Time);

//  //Add the Herbivore Autofeeder (2-4) Scheduler
Chronos::Mark::Daily 		Autofeeder_2_4_OnTime(18, 0, 0);
Chronos::Event 			    Autofeeder_2_4_CalendarEvent(	ALMID_AUTOFEEDER_2_4, 	Autofeeder_2_4_OnTime, 	Autofeeder_Pulse_Time);

//Autofeeder Setup
Chronos::Span::Seconds 		DosePump_Pulse_Time(30);

//  //Add the Alk. Dosing (1-1) Scheduler
Chronos::Mark::Daily 		DosePump_1_1_OnTime(11, 0, 0);
Chronos::Event 			    DosePump_1_1_CalendarEvent(		ALMID_DOSEPUMP_1_1, 	  DosePump_1_1_OnTime, 	DosePump_Pulse_Time);

//  //Add the Alk. Dosing (1-2) Scheduler
Chronos::Mark::Daily 		DosePump_1_2_OnTime(13, 0, 0);
Chronos::Event 			    DosePump_1_2_CalendarEvent(		ALMID_DOSEPUMP_1_2, 	  DosePump_1_2_OnTime, 	DosePump_Pulse_Time);

//  //Add the Alk. Dosing (1-3) Scheduler
Chronos::Mark::Daily 		DosePump_1_3_OnTime(15, 0, 0);
Chronos::Event 			    DosePump_1_3_CalendarEvent(		ALMID_DOSEPUMP_1_3, 	  DosePump_1_3_OnTime, 	DosePump_Pulse_Time);

//  //Add the Alk. Dosing (1-4) Scheduler
Chronos::Mark::Daily    	DosePump_1_4_OnTime(17, 0, 0);
Chronos::Event 			    DosePump_1_4_CalendarEvent(		ALMID_DOSEPUMP_1_4, 	  DosePump_1_4_OnTime, 	DosePump_Pulse_Time);


//  //Add the Ca Doser (2-1) Scheduler
Chronos::Mark::Daily 		DosePump_2_1_OnTime(12, 0, 0);
Chronos::Event 			    DosePump_2_1_CalendarEvent(		ALMID_DOSEPUMP_2_1, 	  DosePump_2_1_OnTime, 	DosePump_Pulse_Time);

//  //Add the Ca Doser (2-2) Scheduler
Chronos::Mark::Daily    	DosePump_2_2_OnTime(14, 0, 0);
Chronos::Event 			    DosePump_2_2_CalendarEvent(		ALMID_DOSEPUMP_2_2, 	  DosePump_2_2_OnTime, 	DosePump_Pulse_Time);

//  //Add the Ca Doser (2-3) Scheduler
Chronos::Mark::Daily    	DosePump_2_3_OnTime(16, 0, 0);
Chronos::Event 			    DosePump_2_3_CalendarEvent(		ALMID_DOSEPUMP_2_3, 	  DosePump_2_3_OnTime, 	DosePump_Pulse_Time);

//  //Add the Ca Doser (2-4) Scheduler
Chronos::Mark::Daily 		DosePump_2_4_OnTime(18, 0, 0);
Chronos::Event 			    DosePump_2_4_CalendarEvent(		ALMID_DOSEPUMP_2_4, 	  DosePump_2_4_OnTime, 	DosePump_Pulse_Time);
