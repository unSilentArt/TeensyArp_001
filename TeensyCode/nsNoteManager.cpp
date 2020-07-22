#include "nsNoteManager.h"

#include <arduino.h>



NsNote::NsNote()
{
  clear();
}

void  NsNote::clear()
{
  mEventTime=0;
  mNote=0;
  mVelocity=0;
  mChannel=0;
}

int  NsNote::isClear()
{
  if(mEventTime==0)
    return 1;
  return 0;
}

int  NsNote::hasData()
{
  if(mEventTime==0)
    return 0;
  return 1;
}

void  NsNote::set(int n, int v, int c, unsigned long   t)
{
  mNote= n;
  mVelocity=v;
  mChannel=c;
  mEventTime=t;  
}


NoteQueue::NoteQueue()
{  
  mLastProcessTime=0;
}


void  NoteQueue::reset(void)
  {
  int i;
  
  for(i=0; i<mNoteQueueLength; i++)
    {
    mNoteQ[i].clear();
    }
   mLastProcessTime=0;
  }

void  NoteQueue::addNote(int noteNum, int velocity, int channel, int nTime)
  {
  int i;
  
  for(i=0; i<mNoteQueueLength; i++)
    {
      NsNote &note = mNoteQ[i];
      
      if(note.isClear())    
      {
        note.set(noteNum, velocity, channel, nTime);
        break;
      }
    }
  }


int   NoteQueue::getQueueLength(void)
  {
  int count=0;
  
  for(int i=0; i<mNoteQueueLength; i++)
    {
      if(mNoteQ[i].hasData())      
        count++;
    }

   return count;
  }

void  NoteQueue::repackQueue(void)
{
  int i,j;

  int topEntry = -1;

  // find final note in queue
  for(i=mNoteQueueLength-1;i>=0; i--)
    {
      if(mNoteQ[i].hasData())
      {
        topEntry = i;
        break;
      }
    }

  if(topEntry != -1)
  {
    for(i=0; i<=topEntry; i++)
    {
      if(mNoteQ[i].isClear())     // found an empty
      {
      for(j=i; j<=topEntry; j++)  // shuffle down
        {
        mNoteQ[j]=mNoteQ[j+1];
        }
      mNoteQ[topEntry].clear();
      topEntry--;
      }
    }

    
  }
    
}

void  NoteQueue::midiStop(usb_midi_class& player)
  {
    int i;
    for(i=0; i<mNoteQueueLength; i++)
    {
      NsNote &note = mNoteQ[i];
      
      if(note.hasData())
      {
        if(note.velocity()==0)
            player.sendNoteOff( note.note(), note.velocity(), note.channel() );
      }
    }

    reset();
  }

void  NoteQueue::processQueue(usb_midi_class& player, unsigned long songPos)
  {
    int i;
    bool needsRepack = false;

    if(mLastProcessTime==songPos) // don't check twice....
      return;

    mLastProcessTime = songPos;

    for(i=0; i<mNoteQueueLength; i++)
    {
      NsNote &note = mNoteQ[i];
      
      if(note.hasData())
      {
        if(note.time() <= songPos)
        {
          if(note.velocity()>0)
          {
            player.sendNoteOn( note.note(), note.velocity(), note.channel() );
          }
          else
          {
            player.sendNoteOff( note.note(), note.velocity(), note.channel() );
          }
          note.clear();
          needsRepack = true;
        }
      }
    }

  if(needsRepack)
  {
    //repackQueue();
  }
  }


