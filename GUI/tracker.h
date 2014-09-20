#ifndef TRACKER_H
#define TRACKER_H

class Tracker
{
public:
    Tracker();
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

#endif // TRACKER_H
