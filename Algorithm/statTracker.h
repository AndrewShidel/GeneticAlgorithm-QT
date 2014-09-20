#ifndef _STAT_TRACKER_
#define _STAT_TRACKER_

class Tracker{
public:
    Tracker();
    void init();
    void load();
    
    typedef struct _data{
        unsigned long int questionsAsked;
        unsigned int generations;
        unsigned int population;
        unsigned int total;
        unsigned int progressions;
        unsigned int regressions;
    } Data;
    Data data;

};

#endif