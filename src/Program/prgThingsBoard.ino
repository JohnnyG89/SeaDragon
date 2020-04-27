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

void initThingsBoard(void) {
  prglog("Beginning ThingsBoard Initialization");

  if (!gEthernetConnectionActive) {
    prglog("ThingsBoardTask:Ethernet Connection Not Active-Returning to try again later");
    return;
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    prglog("Connecting to: ");
    prglog(THINGSBOARD_SERVER);
    prglog(" with token ");
    prglog(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      prglog("Failed to connect");
      return;
    } else {
      prglog(String("Connected To: " + String(THINGSBOARD_SERVER)).c_str());
    }
  }

  tskThingsBoard.setCallback(&cyclicThingsBoard);

  prglog("Finished ThingsBoard Initialization");
}

int Temperature, Humidity;

void cyclicThingsBoard(void) {
  logTaskTimer("ThingsBoard");

  if (!tb.connected()) {
    tskThingsBoard.setCallback(&initThingsBoard);
    prglog("Disconnected from ThingsBoard Server");
    return;
  }

  Temperature++;
  Humidity--;
  prglog(String("Sent Data: Temperature: " + String(Temperature) + " Humidity: " + String(Humidity) + ";").c_str());
  tb.sendTelemetryInt("temperature", Temperature);
  tb.sendTelemetryInt("pH", iAnalogValue);
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
