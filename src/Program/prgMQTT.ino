//                      ____                                                                  _____       ______
//              ..'''' |                  .'.       |``````.  |`````````,       .'.        .-~     ~.   .~      ~.  |..          |
//           .''       |______          .''```.     |       | |'''|'''''      .''```.     :            |          | |  ``..      |
//        ..'          |              .'       `.   |       | |    `.       .'       `.   :     _____  |          | |      ``..  |
//  ....''             |___________ .'           `. |......'  |      `.   .'           `.  `-._____.'|  `.______.'  |          ``|
//
//                                                          Reef On

#define _USE_SSL false
#define _LOG_MQTT true
#define MQTT_CHANNEL_COUNT 20

#include <ArduinoMqttClient.h>

#if _USE_SSL
#include <EthernetLarge.h>
#include <SSLClient.h>
#include "trust_anchors.h"
EthernetClient client;
SSLClient sslclient(client, TAs, (size_t)TAs_NUM, 39284);
MqttClient mqttClient(sslclient);

#else
#include <Ethernet.h>
EthernetClient client;
MqttClient mqttClient(client);

#endif


byte mac[] = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x2E};
const char broker[]      = "io.adafruit.com";
const int port = 1883;
const char UserName[] = "JohnnyG89";
const char Password[] = "Lol did you think I'd put my password here?";
//const char ChannelName[] = "testtopic/Topic2/Hello";
const char ChannelName[MQTT_CHANNEL_COUNT][50] = {
  {"JohnnyG89/feeds/Feed1"},
  {"testtopic/Topic2/Ch1"},
  {"testtopic/Topic2/Ch2"},
  {"testtopic/Topic2/Ch3"},
  {"testtopic/Topic2/Ch4"},
  {"testtopic/Topic2/Ch5"},
  {"testtopic/Topic2/Ch6"},
  {"testtopic/Topic2/Ch7"},
  {"testtopic/Topic2/Ch8"},
  {"testtopic/Topic2/Ch9"},
  {"testtopic/Topic2/Ch10"},
  {"testtopic/Topic2/Ch11"},
  {"testtopic/Topic2/Ch12"},
  {"testtopic/Topic2/Ch13"},
  {"testtopic/Topic2/Ch14"},
  {"testtopic/Topic2/Ch15"},
  {"testtopic/Topic2/Ch16"},
  {"testtopic/Topic2/Ch17"},
  {"testtopic/Topic2/Ch18"},
  {"testtopic/Topic2/Ch19"}
};

int outMQTTArray[MQTT_CHANNEL_COUNT];
int _outMQTTArray[MQTT_CHANNEL_COUNT];
int inMQTTArray[MQTT_CHANNEL_COUNT];
bool switchState, _switchState;

void initMQTT(void) {
  log("Beginning Ethernet Configuration");

  Ethernet.init(5);   //CS pin for P1AM-ETH
  Ethernet.begin(mac);  // Get IP from DHCP
  Serial.println(Ethernet.localIP());

  mqttClient.setUsernamePassword(UserName, Password);  // Username and Password tokens for Shiftr.io namespace. These can be found in the namespace settings.

  log(String("Connecting to the MQTT broker: " + String(broker)).c_str());
  if (!mqttClient.connect(broker, port)) {
    log(String("MQTT connection failed! Error code = " + String(mqttClient.connectError())).c_str());
  } else {
    log("You're connected to the MQTT broker!");

    for (int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++) {
      mqttClient.subscribe(ChannelName[ii_MQTT]); //Subscribe to "InputReading" topic
      log(String("Subscribed To Channel: " + String(ChannelName[ii_MQTT])).c_str());
    }
  }

  tskMQTT.setCallback(&cyclicMQTT);
  log("Initialized MQTT Task");
}

void cyclicMQTT(void) {
  logTaskTimer(&ts, "MQTT");

  for (int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++) {
    if (outMQTTArray[ii_MQTT] != _outMQTTArray[ii_MQTT]) {
      mqttClient.beginMessage(ChannelName[ii_MQTT]);  //Topic name
      mqttClient.print(String(outMQTTArray[ii_MQTT])); //Value to send
      mqttClient.endMessage();
      if (_LOG_MQTT) {
        log(String("Sent Message On Topic: " + String(ChannelName[ii_MQTT]) + " With Value: " + String(outMQTTArray[ii_MQTT])).c_str());
      }
    }
    _outMQTTArray[ii_MQTT] = outMQTTArray[ii_MQTT];
  }
  _switchState = switchState; //Update our last sent reading
  int mqttValue = checkBroker();  //Check for new messages
  if (mqttValue != -1) { // -1 means we didn't get a new message
  }
}


int checkBroker() {
  String mqttMessage = "";
  int messageValue = 0;

  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    //log(String("Msg Rec'd On Channel: " + String(mqttClient.messageTopic())).c_str());
    for (int ii_MQTT = 0; ii_MQTT < MQTT_CHANNEL_COUNT; ii_MQTT++) {
      if (mqttClient.messageTopic() == ChannelName[ii_MQTT]) {
        while (mqttClient.available()) {
          mqttMessage += (char)mqttClient.read();
        }
        messageValue =  mqttMessage.toInt();
        if (_LOG_MQTT) {
          log(String("Message From Topic: " + String(ChannelName[ii_MQTT]) + " Value: " + String(mqttMessage)).c_str());
        }
      }
    }
  } else {
    messageValue =  -1;
  }
  return messageValue;
}
