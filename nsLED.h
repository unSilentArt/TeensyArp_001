#ifndef __NS_LED__
#define __NS_LED__

class nsLED
{
    public:
        nsLED();

        void    init(void);
        void    update(void);
        void    setBrightness(int led, int bright);
        int     getBrightness(int led)  {return mBrightness[led];}

    private:
        unsigned long   mLastUpdate;
        int             mCount;

        int             mBrightness[8];
};


#endif  //  __NS_LED__
