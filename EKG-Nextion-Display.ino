/******************************************************************************
Hardware:

  # AD8232 Heart Monitor Version: 1.0 -> Hardware Platform: Arduino Pro 3.3V/8MHz
      >OUT - pin A0
      >LO- - pin 5
      >LO+ - pin 6
      
  # SD Card:
      > CS   - pin 10 
      > MOSI - pin 11
      > MISO - pin 12
      > CLK  - pin 13
      
  # Buzzer - pin 2  

 Autor: Gabriel Borralho  Data: Setembro de 2017
******************************************************************************/
#include "Nextion.h"

#define LEVEL_LOW       (0)
#define CH0_OFFSET  (25) //altura do eixo normal
#define NOTE  1000

NexWaveform s0 = NexWaveform(0, 1, "s0");
NexDSButton bt0   = NexDSButton(0, 7, "bt0"); //Button Pause/Play
NexText txt_bpm  = NexText(0, 3, "t1"); //bpm
static uint8_t ch0_data = LEVEL_LOW;
int speakerPin=2;
int value=0;
int c=0;
uint32_t dual_state;

//***********************************************
int LastTime = 0;
bool BPMTiming = false;
bool BeatComplete = false;
int BPM = 0;    
#define UpperThreshold 550
#define LowerThreshold 490    
int Signal; // holds the incoming raw data. Signal value can range from 0-1024
char buffer[100] = {0};//bpm
//***********************************************

void setup() {
  //Serial.begin(9600);  // initialize the serial communication 
  pinMode(speakerPin,OUTPUT);
  nexInit();
  //dbSerialPrintln("setup done");
}

void loop() {
    value=analogRead(A0);
    ch0_data = value/5;
    s0.addValue(0, CH0_OFFSET + ch0_data);  
    
    if (value > UpperThreshold) {
      if (BeatComplete) {
        BPM = millis() - LastTime;
        BPM = int(60 / (float(BPM) / 1000));
        BPMTiming = false;
        BeatComplete = false;
      }
      if (BPMTiming == false) {
        LastTime = millis();
        BPMTiming = true;
      }
    }
    if ((value < LowerThreshold) & (BPMTiming))
      BeatComplete = true;
     
    Signal = analogRead(A0);
    if (Signal > UpperThreshold) {
      tone(speakerPin, NOTE);
    } else {
      noTone(speakerPin);
    }
    c++;

    if(c>100){// display bpm
      txt_bpm.setText(buffer); //bpm
      memset(buffer, 0, sizeof(buffer)); //bpm
      itoa(BPM, buffer, 10); //bpm
      c=0;
    } 
  
}

