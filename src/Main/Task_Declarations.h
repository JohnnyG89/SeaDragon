#ifdef _DEBUG
#define _BASE_CYCLE_TIME 50
#else
#define _BASE_CYCLE_TIME 10
#endif

#define _HIGH_LOW_PRIORITY_MULTIPLIER 16

////Task Declarations
Scheduler ts;

//High Priority (machine control) tasks
Task ReadInputs(_BASE_CYCLE_TIME, TASK_FOREVER, &ioReadIn, &ts, true);
Task MyFirstTask(_BASE_CYCLE_TIME, TASK_FOREVER, &blink1CB, &ts, true);
Task ATO(_BASE_CYCLE_TIME, TASK_FOREVER, &ATOcb, &ts, true);
Task Alarms(_BASE_CYCLE_TIME, TASK_FOREVER, &Alarmscb, &ts, true);
Task CheckCommStatus(_BASE_CYCLE_TIME, TASK_FOREVER, &CommCheckcb, &ts, true);
Task WriteOutputs(_BASE_CYCLE_TIME, TASK_FOREVER, &ioWriteOut, &ts, true);

//Low-Priority Tasks
Task NTP(_BASE_CYCLE_TIME*_HIGH_LOW_PRIORITY_MULTIPLIER, TASK_FOREVER, &NTPcb, &ts, _ENABLE_COMMS);
Task MQTT(_BASE_CYCLE_TIME*_HIGH_LOW_PRIORITY_MULTIPLIER, TASK_FOREVER, &MQTTcb, &ts, _ENABLE_COMMS);
Task SendRandomMQTTValue(_BASE_CYCLE_TIME*_HIGH_LOW_PRIORITY_MULTIPLIER, TASK_FOREVER, &RandomMQTTValueGenerator, &ts, _ENABLE_COMMS);
