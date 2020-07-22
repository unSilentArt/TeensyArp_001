#ifndef __NS_nsNoteManager__
#define __NS_nsNoteManager__

#include "usb_midi.h"

class NsNote
{
    public:
        NsNote();

        void  clear();
        int   isClear();
        int   hasData();

        unsigned long   time(void)  {return mEventTime;}

        int note(void)      {return mNote;}
        int velocity(void)  {return mVelocity;}
        int channel(void)   {return mChannel;}

        void  set(int note, int velocity, int channel, unsigned long   eventTime);


      private:
        unsigned long   mEventTime;
        int             mNote;
        int             mVelocity;
        int             mChannel;
};



class NoteQueue
{
  public:
    NoteQueue();

    void  reset(void);
    void  addNote(int note, int velocity, int channel, int nTime=1);
    void  processQueue(usb_midi_class&player, unsigned long songPos );
    void  midiStop(usb_midi_class& player);
    int   getQueueLength(void);

  private:
    static const int mNoteQueueLength=128;

    void  repackQueue(void);

    unsigned long   mLastProcessTime;
    NsNote mNoteQ[mNoteQueueLength];
    
};


#endif  //  __NS_nsNoteManager__
