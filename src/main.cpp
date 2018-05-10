// Enable debug prints to serial monitor
#define MY_DEBUG

// Use a bit lower baudrate for serial prints on ESP8266 than default in MyConfig.h
//#define MY_BAUD_RATE 9600

// Enables and select radio type (if attached)
// RFM69
#define MY_RADIO_RFM69
#define MY_RFM69_NEW_DRIVER
//#define MY_RFM69_ATC_MODE_DISABLED
#define MY_RFM69_ATC_TARGET_RSSI_DBM  (-50)  // target RSSI -70dBm
#define MY_RFM69_MAX_POWER_LEVEL_DBM  (10)   // max. TX power 10dBm = 10mW
//#define MY_RFM69_TX_POWER_DBM (10)
// Lower bitrate to increase reliability
#define MY_RFM69_BITRATE_MSB (RFM69_BITRATEMSB_9600)
#define MY_RFM69_BITRATE_LSB (RFM69_BITRATELSB_9600)
#define MY_RFM69_ENABLE_ENCRYPTION
#define MY_RFM69_FREQUENCY            RFM69_433MHZ
//#define MY_IS_RFM69HW
#define MY_RF69_IRQ_PIN               (5)
#define MY_RF69_IRQ_NUM               MY_RF69_IRQ_PIN
#define MY_DEBUG_VERBOSE_RFM69

#define MY_GATEWAY_ESP8266

// Set the hostname for the WiFi Client. This is the hostname
// it will pass to the DHCP server if not static.
#define MY_ESP8266_HOSTNAME "gateway"

// Enable MY_IP_ADDRESS here if you want a static ip address (no DHCP)
#define MY_IP_ADDRESS 192,168,1,126

// If using static ip you can define Gateway and Subnet address as well
#define MY_IP_GATEWAY_ADDRESS 192,168,1,1
#define MY_IP_SUBNET_ADDRESS 255,255,255,0

// Enable inclusion mode
//#define MY_INCLUSION_MODE_FEATURE
// Enable Inclusion mode button on gateway
//#define MY_INCLUSION_BUTTON_FEATURE
// Set inclusion mode duration (in seconds)
//#define MY_INCLUSION_MODE_DURATION 60
// Digital pin used for inclusion mode button
//#define MY_INCLUSION_MODE_BUTTON_PIN  3

// Set blinking period
//#define MY_DEFAULT_LED_BLINK_PERIOD 300

// Flash leds on rx/tx/err
//#define MY_DEFAULT_ERR_LED_PIN 16  // Error led pin
//#define MY_DEFAULT_RX_LED_PIN  16  // Receive led pin
//#define MY_DEFAULT_TX_LED_PIN  16  // the PCB, on board LED

#include <ESP8266WiFi.h>
#include <MySensors.h>
#include <Automaton.h>
#include "MySensorsRFReceiver.h"

#define CHILD_ID_RF     (1)
#define CHILD_ID_RSSI_NODE_1   (2)
#define CHILD_ID_RSSI_NODE_2   (3)
#define CHILD_ID_RSSI_NODE_3   (4)
#define CHILD_ID_RSSI_NODE_4   (5)

#define RF_RECEIVE_PIN         (4)
#define TIMEOUT (1000) // require 1 second of silence between repetitions


// We use an automaton to ignore doubles
MySensorsRFReceiver receiver;

MyMessage recvMesg(CHILD_ID_RF, V_VAR1);
MyMessage msgRssi1(CHILD_ID_RSSI_NODE_1, V_LEVEL);
MyMessage msgRssi2(CHILD_ID_RSSI_NODE_2, V_LEVEL);
MyMessage msgRssi3(CHILD_ID_RSSI_NODE_3, V_LEVEL);
MyMessage msgRssi4(CHILD_ID_RSSI_NODE_4, V_LEVEL);


void presentation()
{
	// Present locally attached sensors here
  present(CHILD_ID_RF, S_CUSTOM, "RF Receiver");
  present(CHILD_ID_RSSI_NODE_1, S_SOUND, "RSSI");
  present(CHILD_ID_RSSI_NODE_2, S_SOUND, "RSSI");
  present(CHILD_ID_RSSI_NODE_3, S_SOUND, "RSSI");
  present(CHILD_ID_RSSI_NODE_4, S_SOUND, "RSSI");
}

void receiveCallBack(int idx, int v, int up) {
  // Send communications
  Serial.println("Sending!!");
  send(recvMesg.set(receiver.receivedValue));
}

void setup()
{
  receiver
    //.trace(Serial)
    .begin(RF_RECEIVE_PIN, TIMEOUT) // interrupt # is same as PIN #
    .onReceive(receiveCallBack);
}

void loop()
{
  automaton.run();
}

void receive(const MyMessage &message)
{
  if(message.sender == 1) {
    send(msgRssi1.set(RFM69_getReceivingRSSI()));
  } else if(message.sender == 2) {
    send(msgRssi2.set(RFM69_getReceivingRSSI()));
  } else if(message.sender == 3) {
    send(msgRssi3.set(RFM69_getReceivingRSSI()));
  } else if(message.sender == 4) {
    send(msgRssi4.set(RFM69_getReceivingRSSI()));
  }
}
