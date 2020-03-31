//Define the preprocessor symbols for compile Options//
#define _DEBUG
//#define _DEBUG_VERBOSE
//#define _DUMP_TO_SD
#define _ENABLE_COMMS true
#define _REQUIRE_HARDWARE true

#ifdef _DEBUG
#define log(s) {LogEntry(F(s));}
#else
#define log(s)
#endif



//TaskScheduler Compiler Flags
#define _TASK_TIMECRITICAL          // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN     // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST        // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS            // Compile with support for wdt control points and task ids
#define _TASK_LTS_POINTER           // Compile with support for local task storage pointer
// #define _TASK_PRIORITY           // Support for layered scheduling priority
// #define _TASK_MICRO_RES          // Support for microsecond resolution
// #define _TASK_STD_FUNCTION       // Support for std::function (ESP8266 ONLY)
#define _TASK_DEBUG                 // Make all methods and variables public for debug purposes
// #define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
#define _TASK_TIMEOUT               // Support for overall task timeout
// #define _TASK_OO_CALLBACKS       // Support for callbacks via inheritance
// #define _TASK_DEFINE_MILLIS      // Force forward declaration of millis() and micros() "C" style
#define _TASK_EXPOSE_CHAIN          // Methods to access tasks in the task chain

#define TimeZone -4
#define MQTT_CHANNEL_COUNT 20
#define LOG_FILE_NAME "Log.txt"

#define seventyYears  2208988800UL
