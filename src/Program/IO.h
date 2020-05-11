//IO Bits - On the PLC itself
bool        iSwitchState;
bool        oLEDIndicator;

//Process IO
//Relay Outputs
uint8_t     oRelayOutputWord;
bool        oFreshwaterLight;
bool        oCabinetLight;
bool        oSkimmer;
bool        oWaveMaker;
bool        oPowerhead;
bool        oSaltwaterLight;
bool		oReturnPump;
bool		oFugeLight;

//Same as above, except for remotely controlled IO
bool        rmt_oFreshwaterLight;
bool        rmt_oCabinetLight;
bool        rmt_oSkimmer;
bool        rmt_oWaveMaker;
bool        rmt_oPowerhead;
bool        rmt_oSaltwaterLight;
bool		rmt_oReturnPump;
bool		rmt_oFugeLight;

//Discrete Outputs
uint8_t     oDiscreteOutputWord;
bool        oAutoFeeder[2];
bool        oDosePump[2];
bool		oATOPump;
bool        oIndicatorLight_Red;
bool        oIndicatorLight_Green;
bool        oIndicatorLight_Blue;

bool        rmt_oAutoFeeder[2];
bool        rmt_oDosePump[2];
bool		rmt_oATOPump;

//Discrete Inputs
uint8_t     iDiscreteInputWord;
bool        iCabinetDoorSensor;
bool        iATOReservoirEmpty;
bool        iPanelDoor;
bool        iTankOpticalSensor[3];
bool        iLeakDetector[2];

//Analog Inputs
int         iAnalogValue = -1;
float       smoothedSensorValueAvg;
double      ipHSensorReading;

//RTD Inputs
float       iTemperature;
float       iTemperature_Smoothed;

//Machine Mode Switch Control
uint8_t     iMachineModeSwitchWord;
bool        imode_Auto = 0;
bool        imode_manSkimmer = 0;
bool        imode_manWaveMaker = 0;
bool        imode_manPowerhead = 0;
bool        imode_manAutoFeeder = 0;
bool        imode_manFreshwaterLight = 0;
bool        imode_manDosePump = 0;
bool        imode_WebControl = 0;

/*
//Machine Mode Switch Control
uint8_t     iMachineModeSwitchWord[2];
bool        imode_Auto = 0;
bool        imode_WebControl = 0;
bool		imode_Feed = 0;
bool		imode_Storm = 0;

bool        imode_manSkimmer = 0;
bool        imode_manWaveMaker = 0;
bool        imode_manPowerhead = 0;
bool        imode_manFreshwaterLight = 0;
bool        imode_manAutoFeeder[2];
bool        imode_manDosePump[2];
*/


