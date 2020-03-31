void initP1AMHardware(void){
  log("Initializing Hardware And Communications");

  if(_REQUIRE_HARDWARE){
    while (!P1.init()) {
      log("Waiting For P1 Modules");
      delay(100);
    }
    //P1.printModules();                              //print out all currently signed-on modules to the console
    P1.getFwVersion();                              //print the Base Controller's firmware version to the console
    pinMode(LED_BUILTIN, OUTPUT);                   //Set our LED (Pin 32) to be an output
    pinMode(SWITCH_BUILTIN, INPUT);                 //Set our Switch (Pin 31) to be an input
    int baseErrors = P1.rollCall(baseArray, 3);     //Pass in list of modules and the number to check
    log(String("Base Errors: " + String(baseErrors)).c_str());
    //P1.configWD(10000, TOGGLE);
  }
  
  
  log("Initialized P1AM-100 Unit.");
}
