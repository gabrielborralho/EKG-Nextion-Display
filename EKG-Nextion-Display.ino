/** ##############################################################
**
**  Filename  : EKG-Nextion-Display.ino
**  Project   : EKG-Nextion-Display
**  Processor : ATMega328/P
**  Version   : 1.0
**  Compiler  : Arduino IDE
**  Date/Time : 22/09/2017
**  Aluno     : Gabriel Borralho
**  Last Mod  : 27/09/2017
**
** ##############################################################*/

/*****************************************************************
Hardware:

  # AD8232
      >OUT - pin A0
      >LO- - pin 5
      >LO+ - pin 6
      
  #Nextion Display 3.2 - NX4024T032_011R
    >TX - pin RX0
    >RX - pin TX1
          
  # Buzzer - pin 2  
******************************************************************/
#include "Nextion.h"

#define LEVEL_LOW   (0)
#define CH0_OFFSET  (25) //Normal shaft height
#define NOTE  1000 //Tint set to buzzer

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
int Signal; // holds the incoming raw data. Signal value can range from 0-1024.
char buffer[100] = {0};//bpm
//***********************************************

void setup() {
  pinMode(speakerPin,OUTPUT);
  nexInit(); //Initializes communication with the Nextion Display.
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

