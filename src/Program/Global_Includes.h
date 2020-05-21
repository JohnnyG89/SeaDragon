//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
//Hello: If you are reading this, then congratulations!  Don't forget to enjoy your tank!
//This is my "v1.0" release.

//Global Includes
#ifndef _GLOBAL_INCLUDES_H
#define _GLOBAL_INCLUDES_H

const char* myName = "SeaDragon_Alpha";

//User Program pre-compiler flags
#define _ENABLE_WATCHDOG       true
#define _ENABLE_COMMS          true
#define _DUMP_TO_SD            false
#define LOG_FILE_NAME          "Log_%s.txt"
#define _REQUIRE_HARDWARE      true
#define _ENABLE_ALARMS         true
#define _ENABLE_NTP            true
#define WATCHDOG_TIMEOUT	     5000
#define	_USE_THINGSBOARD		   false
#define _USE_PUBSUB					   true
#define TimeZone               -4
#define seventyYears           2208988800UL

//TaskScheduler Compiler Flags
#define _TASK_TIMECRITICAL       // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN  // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
//#define _TASK_STATUS_REQUEST   // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
//#define _TASK_WDT_IDS          // Compile with support for wdt control points and task ids
//#define _TASK_LTS_POINTER      // Compile with support for local task storage pointer
#define _TASK_PRIORITY           // Support for layered scheduling priority
// #define _TASK_MICRO_RES       // Support for microsecond resolution
// #define _TASK_STD_FUNCTION    // Support for std::function (ESP8266 ONLY)
#define _TASK_DEBUG              // Make all methods and variables public for debug purposes
#define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
//#define _TASK_TIMEOUT          // Support for overall task timeout
// #define _TASK_OO_CALLBACKS    // Support for callbacks via inheritance
// #define _TASK_DEFINE_MILLIS   // Force forward declaration of millis() and micros() "C" style
//#define _TASK_EXPOSE_CHAIN       // Methods to access tasks in the task chain


#include <Time.h>
#include <TimeLib.h>
#include <Chronos.h>
#include <ThingsBoard.h>
#include <SD.h>
#include <P1AM.h>
#include <plclib.h>
#include <TaskSchedulerSleepMethods.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>
#include <Ethernet.h>
#include <Smoothed.h>
#include <curveFitting.h>
#include <ArduinoLog.h>
#include <StreamLib.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <MemoryFree.h>;

//Include the user-generated headers
#include "IO.h"

byte     mac[]     = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x2E};

//Global Variables
bool      gTimeSet                    = false;
bool      gEthernetConnectionActive   = false;
uint32_t	gFaultWord					= 0;

//Fault declarations:
const uint8_t FLT_HARDWARE_TIMEOUT = 0;
const uint8_t FLT_SKIMMER_LEVEL = 1;
const uint8_t FLT_ATO_OPTICAL = 1;
#endif
