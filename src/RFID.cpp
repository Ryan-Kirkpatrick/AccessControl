#include "AccessControl.hpp"

//What kind of RFID Reader do we have here?
#ifdef OLD
void readTag() {
  char tagBytes[6];

  //  while (!Serial.available()) { delay(10); }

  if (Serial.readBytes(tagBytes, 5) == 5)
  {
    uint8_t checksum = 0;
    uint32_t cardId = 0;

    tagBytes[6] = 0;

    //    log("Raw Tag:");
    for (int i = 0; i < 4; i++)
    {
      checksum ^= tagBytes[i];
      cardId = cardId << 8 | tagBytes[i];
      //     Serial.println(tagBytes[i], HEX);
    }

    if (checksum == tagBytes[4])
    {
      log("[AUTH] Tag Number:" + String(cardId));
      flushSerial();
      handleCard(cardId);
      lastReadSuccess = millis();
    } else {
      flushSerial();
      log("[AUTH] incomplete or corrupted RFID read, sorry. ");
    }
  }
}
#endif

#ifdef RF125PS
void readTag() {
  log("[TAG] Reading tag...");
  char tagData[8]; // Stores raw tag bytes
    unsigned char byteCounter = 0; // Count how many bytes have been read from serial
    unsigned char tagDataIndex = 0;
    while (Serial.available()) {
      // Skip unused bytes and flush and extra bytes
      if (byteCounter < 3 || byteCounter > 10) {
        byteCounter++;
        Serial.read();
        continue;  
      }
  
      // Load useful bytes into tagData
      tagData[tagDataIndex] = Serial.read();
  
      // Increment
      tagDataIndex++;
      byteCounter++;
    }
  
    // Convert raw bytes to hex number represented in ASCII string
    String tagStr = String(tagData);
  
    // Convert hex number to decimal
    long tagNumber = strtol(tagStr.c_str(), nullptr, 16);

    log("[TAG] Read tag: " + tagNumber);
    authCard(tagNumber);
}

#endif

// Serial clearing function.

void flushSerial () {
  int flushCount = 0;
  while (  Serial.available() ) {
    Serial.read();  // flush any remaining bytes.
    flushCount++;
    // Serial.println("flushed a byte");
  }
  if (flushCount > 0) {
    log("[DEBUG] Flushed " + String(flushCount + 1) + " bytes.");
    flushCount = 0;
  }
}