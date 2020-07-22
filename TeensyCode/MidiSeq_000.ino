#include <Bounce.h>

#include "nsDisplay.h"
#include "nsLED.h"
#include "nsNoteManager.h"
#include "nsArp.h"

const int ledPin = 13;
int ledState;
int loopCounter;

uint8_t draw_state = 0;
int ledCounter;

nsDisplay   myDisplay;
nsLED       myLEDs;
NsArp       theArp;

unsigned long songPosition;
unsigned long lastDisplayUpdate;
unsigned long ledTimer;


Bounce button1 = Bounce(33, 10);  // XXX ms debounce
Bounce button2 = Bounce(34, 10);  
Bounce button3 = Bounce(35, 10);  
Bounce button4 = Bounce(36, 10);  
Bounce button5 = Bounce(37, 10);  

NoteQueue noteQueue;

//----------------------------------------------------------------------------------------------------------
void setup() 
  {
  songPosition = 0;
  ledState=0;
  ledCounter=0;
  ledState=0;
  ledTimer=0;
  loopCounter=0;

 
  myDisplay.begin();

  pinMode(ledPin, OUTPUT);
  
  pinMode(33, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
  pinMode(37, INPUT_PULLUP);

  myLEDs.init();

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

  int i;
  for(i=0; i<8; i++)
    {
    myLEDs.setBrightness(i, 255);
    delay(25);
    }
  for(i=0; i<8; i++)
    {
    myLEDs.setBrightness(i, 0);
    delay(25);
    }
  delay(100);

  lastDisplayUpdate = 0;
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
      
    if((ledCounter>=39)&&(ledState==LOW))
    {
      ledState = HIGH;
      digitalWrite(ledPin, HIGH);   // set the LED on
    }
  }
}

void  panicReset()
{
    myDisplay.clearBuffer();
    myDisplay.prepare();
      
    myDisplay.drawString( 20, 10, "RESET");
    myDisplay.drawString( 20, 30, "RESET");
    myDisplay.drawString( 20, 50, "RESET");
    
    myDisplay.sendBuffer();

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
void loop() 
  {

  noteQueue.processQueue(usbMIDI,songPosition);

  if (button1.update()) 
    if (button1.fallingEdge()) 
    {
      panicReset();
    }
    
  usbMIDI.read(); // USB MIDI receive
  unsigned long timeNow = millis();

  if(timeNow > lastDisplayUpdate)
  {
    loopCounter++;
    if(loopCounter>999)
      loopCounter=0;
    lastDisplayUpdate += 100;
    
    myDisplay.clearBuffer();
    myDisplay.prepare();
  
    int beatNum = songPosition / 24;
    int barNum = 1 + int(songPosition) / 96;
    int barBeat = 1 + beatNum%4;

    myDisplay.drawString( 0, 0, "v2");
  
    char strBuff[32];
    dtostrf(songPosition, 5, 0, strBuff);   // char *  dtostrf (double __val, signed char __width, unsigned char __prec, char *__s)
    myDisplay.drawString( 20,0 , strBuff);
    
    dtostrf(barNum, 3, 0, strBuff);   
    myDisplay.drawString( 56,0 , strBuff);
  
    dtostrf(barBeat, 3, 0, strBuff);  
    myDisplay.drawString( 70,0 , strBuff);
  
    dtostrf(loopCounter, 6, 0, strBuff);  
    myDisplay.drawString( 92,0 , strBuff);

    theArp.updateDisplay(&myDisplay);

    dtostrf(noteQueue.getQueueLength(), 2, 0, strBuff);  
    myDisplay.drawString( 100,12, "B=");
    myDisplay.drawString( 116,12 , strBuff);
    
    myDisplay.sendBuffer();
    myLEDs.update();
  }
  
  doLed();
  //delay(01);
  }

//________________________________________________________________________________________________________________________________
void OnNoteOn(byte channel, byte note, byte velocity) 
{
  theArp.midiNoteOnReceived(note, velocity, channel,songPosition);
  myLEDs.setBrightness(note%7, 255);
}

//________________________________________________________________________________________________________________________________
void OnNoteOff(byte channel, byte note, byte velocity) 
  {
  theArp.midiNoteOffReceived(note, velocity, channel,songPosition);
  myLEDs.setBrightness(note%7, 0);
}

//________________________________________________________________________________________________________________________________
void OnVelocityChange(byte channel, byte note, byte velocity) {
  Serial.print("Velocity Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.print(velocity, DEC);
  Serial.println();
}

void OnControlChange(byte channel, byte control, byte value) {
  Serial.print("Control Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", control=");
  Serial.print(control, DEC);
  Serial.print(", value=");
  Serial.print(value, DEC);
  Serial.println();
}

void OnProgramChange(byte channel, byte program) {
  Serial.print("Program Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", program=");
  Serial.print(program, DEC);
  Serial.println();
}

void OnAfterTouch(byte channel, byte pressure) {
  Serial.print("After Touch, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pressure=");
  Serial.print(pressure, DEC);
  Serial.println();
}

void OnPitchChange(byte channel, int pitch) {
  Serial.print("Pitch Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pitch=");
  Serial.print(pitch, DEC);
  Serial.println();
}

void onTimeCodeQuarterFrame(uint8_t data)
{
  Serial.print("onTimeCodeQuarterFrame=");
  Serial.print(data, DEC);
  Serial.println();
}
void onHandleSongPosition(uint16_t beats)
{
  songPosition = beats*6;
  Serial.print("onHandleSongPosition=");
  Serial.print(beats, DEC);
  Serial.println();

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
  Serial.println("Start");
}

void onHandleContinue(void)
{    
  Serial.println("Start");
}

void onHandleStop(void)
{    
  noteQueue.midiStop(usbMIDI);
  theArp.midiStopReceived();
}

void onHandleActiveSensing(void)
{    
  Serial.println("onHandleActiveSensing");
}

void onHandleSystemReset(void)
{    
  onHandleStop();
}

void onHandleRealTimeSystem(uint8_t realtimebyte)
{    
  Serial.print("onHandleRealTimeSystem=");
  Serial.print(realtimebyte, DEC);
  Serial.println();

}
