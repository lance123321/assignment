#include <Arduino.h>
const int OUTPUT_ENABLE_PIN = 25;    // Connected to OUTPUT_ENABLE (PB1)
const int OUTPUT_SELECT_PIN = 35;    // Connected to OUTPUT_SELECT (PB2)
const int DATA_OUTPUT_PIN = 18;      // Connected to DATA (Signal A) output pin
const int SYNC_OUTPUT_PIN = 19;      // Connected to SYNC (Signal B) output pin

const int TON_FIRST_PULSE = 3*1000;      // a   corresponding to c
const int TON_INCREMENT = 8*1000;        // b   corresponding to h
const int SYNC_PULSE_ON_TIME = 50*1000;  
const int IDLE_TIME = 13*1000;           // d   corresponding to n

bool outputEnabled = false;
bool reversedWaveform = false;
bool outputEnablePressed = false;
bool outputSelectPressed = false;

void generateDataWaveform() {
  int pulseCount =  5;  // c    corresponding to e
  int pulseOnTime = reversedWaveform ? (TON_FIRST_PULSE + (pulseCount - 1) * TON_INCREMENT) : TON_FIRST_PULSE;
  int pulseOnTimeDecrement = reversedWaveform ? -TON_INCREMENT : TON_INCREMENT;
  
  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(DATA_OUTPUT_PIN, HIGH);  
    delayMicroseconds(pulseOnTime);       
    
    digitalWrite(DATA_OUTPUT_PIN, LOW);  
    delayMicroseconds(TON_INCREMENT);    
    
    pulseOnTime += pulseOnTimeDecrement;  
  }
  
  delayMicroseconds(IDLE_TIME);           
}


void generateSyncPulse() {
  digitalWrite(SYNC_OUTPUT_PIN, HIGH);   
  delayMicroseconds(SYNC_PULSE_ON_TIME);  
  
  digitalWrite(SYNC_OUTPUT_PIN, LOW);     
}

void setup() {
  pinMode(OUTPUT_ENABLE_PIN, INPUT_PULLUP);
  pinMode(OUTPUT_SELECT_PIN, INPUT_PULLUP);
  pinMode(DATA_OUTPUT_PIN, OUTPUT);
  pinMode(SYNC_OUTPUT_PIN, OUTPUT);
  
  digitalWrite(DATA_OUTPUT_PIN, LOW);
  digitalWrite(SYNC_OUTPUT_PIN, LOW);

}

void loop() {
  
  bool outputEnablePressed = digitalRead(OUTPUT_ENABLE_PIN) == LOW;
  bool outputSelectPressed = digitalRead(OUTPUT_SELECT_PIN) == LOW;
  
  if (outputEnablePressed && !outputEnabled) {
    outputEnabled = true;
  } else if (!outputEnablePressed && outputEnabled) {
    outputEnabled = false;
    digitalWrite(DATA_OUTPUT_PIN, LOW); 
  }
  
  if (outputSelectPressed) {
    reversedWaveform = false;  
  }

  if (outputEnabled) {
    generateDataWaveform();
    generateSyncPulse();
    reversedWaveform = !reversedWaveform;  
  }

}
