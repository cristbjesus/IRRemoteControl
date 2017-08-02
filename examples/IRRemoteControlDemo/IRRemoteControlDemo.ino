/*
 * IRsend: IRRemoteControlDemo - demonstrates receiving/sending IR codes.
 * Created by Cristiano Borges - STI - Fatesg
 * August, 2017
 */

#include "IRRemoteControl.h"

IRRecv irRecv;
IRSend irSend;

const int IR_RECV_PIN = 2;

const int MAX_IR_BUFFER_LENGTH = 650;
unsigned int irBuffer[MAX_IR_BUFFER_LENGTH];
int currentIrBufferLength = 0;

const int FREQ_KHZ = 40;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  irRecv.start(IR_RECV_PIN, irBuffer, MAX_IR_BUFFER_LENGTH, currentIrBufferLength);
}

void loop() {
  Serial.println(F("Press the remote control button now - only once"));
  delay(5000);
  
  if (currentIrBufferLength > 0) {
    irRecv.stop(IR_RECV_PIN);
    
    digitalWrite(LED_BUILTIN, HIGH);
    
    Serial.println();
    Serial.print(F("Code: "));
    
    for (int i = 0; i < currentIrBufferLength; i++) {
      if (i > 0) {
        Serial.print(F(", "));
      }
      Serial.print(irBuffer[i]);
    }
    
    Serial.println();
    Serial.println(F("Sending the code..."));
    delay(5000);
    
    irSend.sendRaw(irBuffer, currentIrBufferLength, FREQ_KHZ, false);
    
    Serial.println(F("Code sent!"));
    Serial.println();
    
    digitalWrite(LED_BUILTIN, LOW);
    
    irRecv.start(IR_RECV_PIN, irBuffer, MAX_IR_BUFFER_LENGTH, currentIrBufferLength);
  }
}
