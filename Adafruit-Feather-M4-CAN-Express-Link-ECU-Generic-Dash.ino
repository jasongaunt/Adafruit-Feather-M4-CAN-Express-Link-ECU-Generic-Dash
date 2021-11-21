//
// This script will attempt to parse Generic Dash format CAN messages from a Link G4 / G4+ / G4X ECU
// Information on these can be found in the "Device Specific CAN Information" section of the Link help file
// Or here if you are being lazy; https://www.akao.co.uk/manuals/Link%20G4+%20Manual/device_specific_can_informatio.htm#genericdash
//
// This code was written for use with the Adafruit Feather M4 CAN Express, more information here;
// https://learn.adafruit.com/adafruit-feather-m4-can-express
//
// Written by JBG 20211121
//

// Local header file with configuration and constants for each value
#include "Adafruit-Feather-M4-CAN-Express-Link-ECU-Generic-Dash.h"

// No user-serviceable parts below unless you REALLY know what you are doing

#include <CAN.h>               // CAN Adafruit Fork by adafruit v1.2.1
#include <Adafruit_NeoPixel.h> // Adafruit NeoPixel by Adafruit v2.0.2

Adafruit_NeoPixel strip(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

const uint32_t PIXEL_RED   = strip.Color(255, 0, 0);
const uint32_t PIXEL_GREEN = strip.Color(0, 255, 0);
const uint32_t PIXEL_OFF   = strip.Color(0, 0, 0);

#define maximumPacketSize 64
int nextCANByte;
unsigned char CANFrameBuffer[maximumPacketSize];
volatile unsigned char GenericDash[GenericDashFrames][GenericDashBytes];

unsigned long currentMillis = 0;
unsigned long updateDisplayMillis = 0;
unsigned long updateDisplayMilliRate = SERIAL_UPDATE_MILLISECONDS; // Milliseconds between serial updates

void setup() {
  // Init hardware
  Serial.begin(SERIAL_BAUD);
  while (!Serial);
  pinMode(PIN_CAN_STANDBY, OUTPUT);
  digitalWrite(PIN_CAN_STANDBY, false); // Turn off CAN IC standby mode
  pinMode(PIN_CAN_BOOSTEN, OUTPUT);
  digitalWrite(PIN_CAN_BOOSTEN, true); // Turn on CAN IC voltage booster
  strip.begin();
  strip.setBrightness(50);
  strip.clear();
  
  // start the CAN bus at 500 kbps
  if (!CAN.begin(CAN_BAUD)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // Filter out only what messages we are interested in
  CAN.filter(ECU_HEADER, 0x7FF);

  // Add callback function for when a message is received
  CAN.onReceive(CANReceiveCallback);

  // Print a message to say we are now running
  Serial.println("Generic Dash CAN receiver started.");
}

void CANReceiveCallback(int packetSize) {
  // We have received a CAN packet, process it below
  strip.setPixelColor(0, PIXEL_RED); strip.show();

  switch (CAN.packetId()) {
    case ECU_HEADER:
      for (int i = 0; i < packetSize; i++) {
        if (CAN.peek() == -1) { break; }
        CANFrameBuffer[i] = (char)CAN.read();
      }
      for (int i = 0; i < sizeof(GenericDash[0]); i++) {
        GenericDash[CANFrameBuffer[0]][i] = CANFrameBuffer[i];
      }
      break;
    default:
      Serial.print("Received unknown ");
      if (CAN.packetExtended()) { Serial.print("extended "); } // Extended packet (29-bit)
      if (CAN.packetRtr()) { Serial.print("RTR "); } // Remote transmission request, packet contains no data
      Serial.print("packet with id 0x");
      Serial.print(CAN.packetId(), HEX);
  
      if (CAN.packetRtr()) {
        Serial.print(" and requested length ");
        Serial.println(CAN.packetDlc());
      } else {
        Serial.print(" and length ");
        Serial.print(packetSize);
        Serial.print(": ");
        for (int i = 0; i < packetSize; i++) {
          if (CAN.peek() == -1) { break; }
          nextCANByte = CAN.read();
          for (int i = 0; i < 8; i++) {
            Serial.print((bool)bitRead(nextCANByte, i));
          }
        }
        Serial.println();
      }
      break;
  }
  strip.setPixelColor(0, PIXEL_OFF); strip.show();
}

void loop() {
  // Update our timer
  currentMillis = millis();

  // Call this every updateDisplayMilliRate milliseconds
  if (currentMillis - updateDisplayMillis > updateDisplayMilliRate) {
    updateDisplayMillis = currentMillis;
    strip.setPixelColor(0, PIXEL_GREEN); strip.show();
    Serial.print("RPM: "); Serial.print((signed int)getGenericDashValue(GenericDash, ECU_ENGINE_SPEED_RPM)); Serial.println(" RPM");
    Serial.print("ECT: "); Serial.print((signed int)getGenericDashValue(GenericDash, ECU_ENGINE_COOLANT_TEMPERATURE_DEGREES_C)); Serial.println(" °C");
    Serial.print("IAT: "); Serial.print((signed int)getGenericDashValue(GenericDash, ECU_INTAKE_AIR_TEMPERATURE_DEGRESS_C)); Serial.println(" °C");
    Serial.print("BAT: "); Serial.print(getGenericDashValue(GenericDash, ECU_BATTERY_VOLTAGE)); Serial.println(" volts");
    strip.setPixelColor(0, PIXEL_OFF); strip.show();
  }
}
