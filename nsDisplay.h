#ifndef __NS_DISPLAY__
#define __NS_DISPLAY__

class nsDisplay
{
    public:
        nsDisplay();
        
        void    begin(void);
        void    clearBuffer(void);
        void    sendBuffer(void);
        void    prepare(void);

        void    drawFrame(int x, int y, int w, int h);
        void    drawBox(int x, int y, int w, int h);
        void    drawLine(int x1, int y1, int x2, int y2);
        void    drawString(int x, int y,  const char *s);

        void    setDrawColour(int c);                       //  0 = Black, 1 = white, 2 = XOR

};


#endif  //  __NS_DISPLAY__
