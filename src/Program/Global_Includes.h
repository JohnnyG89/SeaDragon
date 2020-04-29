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
#define _CHECK_MODULE_STATUS  false

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

//Set logging settings in debug vs. production/deployment (Debug Stripping)
#if _DEBUG
#define prglog(s) {LogEntry(F(s));}
#else
#define prglog(s)        //Direct the compiler to replace the prglog() function with void (performance optimization)
#endif

#define TimeZone -4
#define seventyYears  2208988800UL

#include <ThingsBoard.h>
#include <SD.h>
#include <P1AM.h>
#include <plclib.h>
#include <TaskSchedulerSleepMethods.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskScheduler.h>
#include <RTCZero.h>
#include <Ethernet.h>
#include <Smoothed.h>
#include <curveFitting.h>

EthernetClient client;
ThingsBoard tb(client);
RTCZero rtc;
EthernetUDP Udp;

byte mac[] = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x2E};

//Constants for the IO module slots
const uint8_t P1_SLOT_RTD =  1;
const uint8_t P1_SLOT_AIO =  2;
const uint8_t P1_SLOT_DO  =  3;
const uint8_t P1_SLOT_DI  =  4;
const uint8_t P1_SLOT_RO  =  5;

const uint8_t P1_MODULE_COUNT = 5;

const char* baseArray[] = { "P1-04RTD", "P1-4ADL2DAL-2", "P1-08TD2", "P1-08ND3", "P1-08TRS"}; //Expected Modules

bool gTimeSet = false, gEthernetConnectionActive = false;
//TODO:  Instead of passing pointer to the active task, just get it from the scheduler so you don't have to declare, schedule the task

//IO Buffer
uint32_t outDiscreteDataBuffer;
uint32_t outRelayDataBuffer;
uint32_t inDiscreteDataBuffer;

//IO Bits - On the PLC itself
bool iSwitchState;
bool oLEDIndicator;

//IO Bits- Wired into slices
bool iCabinetDoorSensor;
channelLabel bin_DoorSwitch = {P1_SLOT_DI, 0};

bool oCabinetLight;
channelLabel bout_CabinetLight = {P1_SLOT_RO, 2};

bool oFreshwaterLightOutput;
channelLabel bout_FreshwaterGooseneck = {P1_SLOT_RO, 1};

//AIO Data
int iAnalogValue = -1;
channelLabel ain_pH = {P1_SLOT_AIO, 1};
float smoothedSensorValueAvg;
double ipHSensorReading;

float iTemperature;
channelLabel ain_Temp = {P1_SLOT_RTD, 1};
const char P1_04RTD_CONFIG[] = { 0x40, 0x03, 0x60, 0x05, 0x20, 0x07, 0x80, 0x00 };

#endif
