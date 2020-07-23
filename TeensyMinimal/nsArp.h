// nsArp.h

#pragma once

class nsDisplay;
class NoteQueue;

class NsArp
{
    public:
        NsArp();

        void    midiNoteOnReceived(int note, int vel, int channel,unsigned long songPosition);
        void    midiNoteOffReceived(int note, int vel, int channel,unsigned long songPosition);
        void    midiClockReceived(unsigned long songPosition,NoteQueue* pQueue);
        void    midiStopReceived(void);
        void    reset(void);

        void    playNextNote(unsigned long songPosition,NoteQueue* pQueue);

    private:
        static const int mKeyListSize=128;
        
        int mKeyVelocity[mKeyListSize];
        int mKeyTime[mKeyListSize];
        
        int mLastNotePlayedIdx;
        int mNumKeysCurrentlyDown;
        int mMidiChannel;

};
