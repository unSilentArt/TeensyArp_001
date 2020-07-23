
#include "nsNoteManager.h"
#include "nsArp.h"

const int ledPin = 13;
int ledState;
int loopCounter;

uint8_t draw_state = 0;
int 	ledCounter;

NsArp       theArp;
NoteQueue 	noteQueue;

unsigned long songPosition;
unsigned long lastDisplayUpdate;
unsigned long ledTimer;



//----------------------------------------------------------------------------------------------------------
void setup()
  {
  songPosition = 0;
  ledState=0;
  ledCounter=0;
  ledState=0;
  ledTimer=0;
  loopCounter=0;


  pinMode(ledPin, OUTPUT);

  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleVelocityChange(OnVelocityChange);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleProgramChange(OnProgramChange);
  usbMIDI.setHandleAfterTouch(OnAfterTouch);
  usbMIDI.setHandlePitchChange(OnPitchChange);


  usbMIDI.setHandleTimeCodeQuarterFrame(onTimeCodeQuarterFrame);
  usbMIDI.setHandleSongPosition(onHandleSongPosition);
  usbMIDI.setHandleClock(onHandleClock);
  usbMIDI.setHandleStart(onStart);
  usbMIDI.setHandleContinue(onHandleContinue);
  usbMIDI.setHandleStop(onHandleStop);
  usbMIDI.setHandleActiveSensing(onHandleActiveSensing);
  usbMIDI.setHandleSystemReset(onHandleSystemReset);
  usbMIDI.setHandleRealTimeSystem(onHandleRealTimeSystem);
  }

// TEENSY LED______________________________________________________________________________
void doLed()
{
  unsigned long timeNow = millis();
  if(timeNow > ledTimer)
  {
    ledTimer += 250;
    ledCounter++;
    if(ledCounter>=40)
    {
      ledCounter=0;
      digitalWrite(ledPin, LOW);   // set the LED off
      ledState = LOW;
    }
      
    if((ledCounter>=30)&&(ledState==LOW))
    {
      ledState = HIGH;
      digitalWrite(ledPin, HIGH);   // set the LED on
    }
  }
}

void  panicReset()
{
    songPosition = 0;
    ledState=0;
    ledCounter=0;
    ledState=0;
    ledTimer=0;
    loopCounter=0;

    noteQueue.reset();
    theArp.reset();

    delay(500);

    int nNum;
  
    for(int n=0; n<13; n++)
    {
      nNum=60;

      if(n%3==1)
        nNum=64;
        
      if(n%3==2)
        nNum=67;
        
      noteQueue.addNote(nNum, 90, 1, 24+1+n*24);
      noteQueue.addNote(nNum,  0, 1, 24+1+(n+1)*24);
    }
  }

//________________________________________________________________________________________________________________________________
//
//	MIDI event processing
//________________________________________________________________________________________________________________________________
void loop()
  {
  noteQueue.processQueue(usbMIDI,songPosition);
  usbMIDI.read();
  }

//________________________________________________________________________________________________________________________________
void OnNoteOn(byte channel, byte note, byte velocity) 
  {
  theArp.midiNoteOnReceived(note, velocity, channel,songPosition);
  }

//________________________________________________________________________________________________________________________________
void OnNoteOff(byte channel, byte note, byte velocity) 
  {
  theArp.midiNoteOffReceived(note, velocity, channel,songPosition);
  }

//________________________________________________________________________________________________________________________________
void OnVelocityChange(byte channel, byte note, byte velocity) 
{
}

void OnControlChange(byte channel, byte control, byte value) 
{
}

void OnProgramChange(byte channel, byte program) 
{
}

void OnAfterTouch(byte channel, byte pressure)
{
}

void OnPitchChange(byte channel, int pitch)
{
}

void onTimeCodeQuarterFrame(uint8_t data)
{
}
void onHandleSongPosition(uint16_t beats)
{
  songPosition = beats*6;

}
void onHandleClock(void)
{
  theArp.midiClockReceived(songPosition, &noteQueue);
  songPosition++;

/*
  int beatNum = songPosition / 24;
  int barNum = 1 + int(songPosition) / 96;
  int barBeat = 1 + beatNum%4;
*/
}

void onStart(void)
{
  songPosition = 0;
}

void onHandleContinue(void)
{
}

void onHandleStop(void)
{
  noteQueue.midiStop(usbMIDI);
  theArp.midiStopReceived();
}

void onHandleActiveSensing(void)
{
}

void onHandleSystemReset(void)
{
  onHandleStop();
}

void onHandleRealTimeSystem(uint8_t realtimebyte)
{
}
