//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

//Hello: If you are reading this, then congratulations!  Don't forget to enjoy your tank!

//Global Includes
#ifndef _GLOBAL_INCLUDES_H
#define _GLOBAL_INCLUDES_H

//User Program pre-compiler flags
#define _DEBUG                true
#define _ENABLE_WATCHDOG      true
#define _ENABLE_COMMS         true
#define _DUMP_TO_SD           true
#define LOG_FILE_NAME         "Log.txt"
#define _DEBUG_VERBOSE        false
#define _LOG_MQTT             true
#define _REQUIRE_HARDWARE     true
#define _ENABLE_ATO           true
#define _ENABLE_ALARMS        false
#define _ENABLE_NTP           true

//TaskScheduler Compiler Flags
#define _TASK_TIMECRITICAL          // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN     // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST        // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS             // Compile with support for wdt control points and task ids
#define _TASK_LTS_POINTER           // Compile with support for local task storage pointer
#define _TASK_PRIORITY              // Support for layered scheduling priority
// #define _TASK_MICRO_RES          // Support for microsecond resolution
// #define _TASK_STD_FUNCTION       // Support for std::function (ESP8266 ONLY)
#define _TASK_DEBUG                 // Make all methods and variables public for debug purposes
// #define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
#define _TASK_TIMEOUT               // Support for overall task timeout
// #define _TASK_OO_CALLBACKS       // Support for callbacks via inheritance
// #define _TASK_DEFINE_MILLIS      // Force forward declaration of millis() and micros() "C" style
#define _TASK_EXPOSE_CHAIN          // Methods to access tasks in the task chain

#define TimeZone -4
#define seventyYears  2208988800UL

#include <ThingsBoard.h>
#include <SD.h>
#include <P1AM.h>
//#include <plclib.h>
#include <TaskSchedulerSleepMethods.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>
#include <RTCZero.h>
#include <Ethernet.h>
#include <Smoothed.h>
#include <curveFitting.h>
#include <CronAlarms.h>
#include <ArduinoLog.h>

EthernetClient client;
ThingsBoard tb(client);
RTCZero rtc;
EthernetUDP Udp;


bool gTimeSet = false, gEthernetConnectionActive = false;

////IO Buffer
//uint32_t outDiscreteDataBuffer;
//uint32_t outRelayDataBuffer;
//uint32_t inDiscreteDataBuffer;

//IO Bits - On the PLC itself
bool iSwitchState;
bool oLEDIndicator;
bool iCabinetDoorSensor;
bool oCabinetLight;
bool oFreshwaterLightOutput;
int iAnalogValue = -1;
float smoothedSensorValueAvg;
double ipHSensorReading;
float iTemperature;
#endif

//
//    Log.notice   (  "Log as Info with integer values : %d, %d" CR                  , intValue1,  intValue2);
//    Log.notice   (F("Log as Info with hex values     : %x, %X" CR                 ), intValue1,  intValue1);
//    Log.notice   (  "Log as Info with hex values     : %x, %X" CR                  , intValue2,  intValue2);
//    Log.notice   (F("Log as Info with binary values  : %b, %B" CR                 ), intValue1,  intValue1);
//    Log.notice   (  "Log as Info with binary values  : %b, %B" CR                  , intValue2,  intValue2);
//    Log.notice   (F("Log as Info with long values    : %l, %l" CR                 ), longValue1, longValue2);
//    Log.notice   (  "Log as Info with bool values    : %t, %T" CR                  , boolValue1, boolValue2);
//    Log.notice   (F("Log as Info with string value   : %s" CR                     ), charArray);
//    Log.notice   (  "Log as Info with Flash string value   : %S" CR                , flashCharArray1);
//    Log.notice   (  "Log as Info with Flash string value   : %S" CR                , flashCharArray2);
//    Log.notice   (  "Log as Info with string value   : %s" CR                      , stringValue1.c_str());
//    Log.notice   (F("Log as Info with float value   : %F" CR                      ), floatValue);
//    Log.notice   (  "Log as Info with float value   : %F" CR                       , floatValue);
//    Log.notice   (F("Log as Info with double value   : %D" CR                     ), doubleValue);
//    Log.notice   (  "Log as Info with double value   : %D" CR                      , doubleValue);
//    Log.notice   (F("Log as Debug with mixed values  : %d, %d, %l, %l, %t, %T" CR ), intValue1 , intValue2,
//                longValue1, longValue2, boolValue1, boolValue2);
//    Log.trace    (  "Log as Trace with bool value    : %T" CR                      , boolValue1);
//    Log.warning  (  "Log as Warning with bool value  : %T" CR                      , boolValue1);
//    Log.error    (  "Log as Error with bool value    : %T" CR                      , boolValue1);
//    Log.fatal    (  "Log as Fatal with bool value    : %T" CR                      , boolValue1);
//    Log.verbose  (F("Log as Verbose with bool value   : %T" CR CR CR               ), boolValue2);
