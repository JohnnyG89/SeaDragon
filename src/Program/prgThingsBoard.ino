//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On
#include "Global_Includes.h"

#define TOKEN               "VqSbL0ENWMvPUA7GTLBd"
#define THINGSBOARD_SERVER  "192.168.1.200"

R_TRIG TriggerTelemetryData;
int ii;

void initThingsBoard(void) {
  //  tskThingsBoard.disable();
  //  return;
  prglog("ThingsBoard::Beginning ThingsBoard Initialization");


  if (!gEthernetConnectionActive) {
    prglog("ThingsBoard:: Ethernet Connection Not Active-Returning to try again later");
    return;
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    prglog("ThingsBoard::Connecting to: ");
    prglog(THINGSBOARD_SERVER);
    prglog(" with token ");
    prglog(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      prglog("ThingsBoard::Failed to connect");
      return;
    } else {
      prglog(String("ThingsBoard::Connected To: " + String(THINGSBOARD_SERVER)).c_str());
    }
  }

  ii = 0;

  tskThingsBoard.setCallback(&cyclicThingsBoard);

  prglog("ThingsBoard::Finished ThingsBoard Initialization");
}

void cyclicThingsBoard(void) {
  logTaskTimer("ThingsBoard");

  if (!tb.connected()) {
    tskThingsBoard.setCallback(&initThingsBoard);
    prglog("ThingsBoard::Disconnected from ThingsBoard Server");
    return;
  }

  tb.sendTelemetryInt("MessageIndex", ii);
  switch (ii % 8) {
    case 0:
      tb.sendTelemetryInt("iTemperature", iTemperature);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "iTemperature" + " :: Value " + String(iTemperature) + " ::").c_str());
      break;
    case 1:
      tb.sendTelemetryFloat("iAnalogInput_Raw", iAnalogValue);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "iAnalogInput_Raw" + " :: Value " + String(iAnalogValue) + " ::").c_str());
      break;
    case 2:
      tb.sendTelemetryFloat("iAnalogInput_Filtered", smoothedSensorValueAvg);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "iAnalogInput_Filtered" + " :: Value " + String(smoothedSensorValueAvg) + " ::").c_str());
      break;
    case 3:
      tb.sendTelemetryFloat("ipH",  ipHSensorReading);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "ipH" + " :: Value " + String(ipHSensorReading) + " ::").c_str());
      break;
    case 4:
      tb.sendTelemetryBool("iSensor_CabinetDoor",  iCabinetDoorSensor);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "iSensor_CabinetDoor" + " :: Value " + String(iCabinetDoorSensor) + " ::").c_str());
      break;
    case 5:
      tb.sendTelemetryBool("iSwitch_PLC", iSwitchState);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "iSwitch_PLC" + " :: Value " + String(iSwitchState) + " ::").c_str());
      break;
    case 6:
      tb.sendTelemetryBool("oLight_Freshwater", oFreshwaterLightOutput);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "oLight_Freshwater" + " :: Value " + String(oFreshwaterLightOutput) + " ::").c_str());
      break;
    case 7:
      tb.sendTelemetryBool("oLight_Cabinet",  oCabinetLight);
      prglog(String("ThingsBoard:: id " + String(ii) + " :: Sent Data to " + String(THINGSBOARD_SERVER) + ":: Channel " + "oLight_Cabinet" + " :: Value " + String(oCabinetLight) + " ::").c_str());
      break;
  }
  ii++;

  tb.loop();
}

//
//// Processes function for RPC call "example_set_temperature"
//// RPC_Data is a JSON variant, that can be queried using operator[]
//// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
//RPC_Response processTemperatureChange(const RPC_Data &data)
//{
//  Serial.println("Received the set temperature RPC method");
//
//  // Process data
//
//  float example_temperature = data["temp"];
//
//  Serial.print("Example temperature: ");
//  Serial.println(example_temperature);
//
//  // Just an response example
//  return RPC_Response("example_response", 42);
//}

//
//// Processes function for RPC call "example_set_switch"
//// RPC_Data is a JSON variant, that can be queried using operator[]
//// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
//RPC_Response processSwitchChange(const RPC_Data &data)
//{
//  Serial.println("Received the set switch method");
//
//  // Process data
//
//  bool switch_state = data["switch"];
//
//  Serial.print("Example switch state: ");
//  Serial.println(switch_state);
//
//  // Just an response example
//  return RPC_Response("example_response", 22.02);
//}
//
//const size_t callbacks_size = 2;
//RPC_Callback callbacks[callbacks_size] = {
//  { "example_set_temperature",    processTemperatureChange },
//  { "example_set_switch",         processSwitchChange }
//};
//
//bool subscribed = false;
//
//void loop() {
//  delay(100);
//
//  if (WiFi.status() == WL_IDLE_STATUS) {
//    // Still connecting
//    return;
//  }
//
//  if (WiFi.status() != WL_CONNECTED) {
//    Serial.println("Connecting to AP ...");
//    Serial.print("Attempting to connect to WPA SSID: ");
//    Serial.println(WIFI_AP);
//    // Connect to WPA/WPA2 network
//    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
//    return;
//  }
//
//  if (!tb.connected()) {
//    subscribed = false;
//
//    // Connect to the ThingsBoard
//    Serial.print("Connecting to: ");
//    Serial.print(THINGSBOARD_SERVER);
//    Serial.print(" with token ");
//    Serial.println(TOKEN);
//    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
//      Serial.println("Failed to connect, retrying ...");
//      return;
//    }
//  }
//
//  if (!subscribed) {
//    Serial.println("Subscribing for RPC...");
//
//    // Perform a subscription. All consequent data processing will happen in
//    // processTemperatureChange() and processSwitchChange() functions,
//    // as denoted by callbacks[] array.
//    if (!tb.RPC_Subscribe(callbacks, callbacks_size)) {
//      Serial.println("Failed to subscribe for RPC");
//      return;
//    }
//
//    Serial.println("Subscribe done");
//    subscribed = true;
//  }
//
//  Serial.println("Waiting for data...");
//  tb.loop();
//}
