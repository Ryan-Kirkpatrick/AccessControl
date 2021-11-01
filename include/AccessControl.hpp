#pragma once

// Uncomment the relevant device type for this device.
#define DOOR
//define INTERLOCK

// Uncomment for RFID reader types.
//#define OLD
#define RF125PS

// Uncomment to enable serial messaging debug.
#define SERIALDEBUG

// Include all the libraries we need for this.
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <WebSocketsServer.h>
#include <WiFiClientSecureBearSSL.h>

// Editable config values. Example given below but secrets should be stored outside Git's prying eyes.
extern const char* ssid; // Wifi SSID
extern const char* password; // Wifi Password
extern const char* host; // Host URL
extern const char* secret; // Secret to talk to the Host on.
extern const char* deviceName; // Device name. DOOR-DoorName or INT-InterlockName
extern const char* devicePassword; // Password for OTA on device.
extern const char* deviceType;
extern uint8_t contact; // Set default switch state, 1 for doors that are permanantly powered/fail-open.
  
extern uint8_t checkinRate; // How many seconds between standard server checkins.
extern uint8_t sessionCheckinRate; // How many seconds between interlock session checkins.
extern uint16_t rfidSquelchTime; // How long after checking a card with the server should we ignore reads.

// Configure our output pins.
extern const int switchPin; // This is the pin the relay is on in the TH10 board.
extern const int ledPin; // This is an onboard LED, just to show we're alive.
extern const int statePin; // This is the pin exposed on the TRRS plug on the sonoff, used for LED on interlocks.

// Initialise our base state vars.
extern uint8_t triggerFlag ; //State trigger for heartbeats and other useful blocking things.
extern uint32_t lastReadSuccess; // Set last read success base state. Setting to 5 seconds to make sure on boot it's going to ignore initial reads.
extern uint32_t lastId; // Set lastID to nothing.
extern String sessionID; // Set sessionID as null.
extern char currentColor; // Default interlock status led color is blue, let's start there.
extern String curCacheHash; // Current cache hash, pulled from cache file.
extern uint8_t useLocal; // Whether or not to use local cache, set by heartbeat failures or manually.
extern uint8_t tagsLoaded; // Whether or not we've loaded tags into memory.
extern int tagsArray[200]; // Where the int array of tags is loaded to from cache on heartbeat fail.

//Configure our objects.
extern HTTPClient client;
extern Adafruit_NeoPixel pixel;
extern ESP8266WebServer http;
extern WebSocketsServer webSocket;
extern Ticker heartbeat;
extern Ticker heartbeatSession;

// RFID Functions
void readTag();
void flushSerial();

// Door and Interlock Functions
void authCard(long);
void handleTag();
void pulseContact();
void checkStateMachine();

// Main fuctions
void log(String);
void stausLight(char);
void clearTags();
void checkIn();
void printTags();
void loadTags();
void printCache();
void getCache();
