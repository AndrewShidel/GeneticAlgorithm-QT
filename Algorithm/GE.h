#ifndef GE_GE_h
#define GE_GE_h

#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Queue.h"
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include "Map.h"
#include <utility>

#include "statTracker.h"
#include "Language.h"

#define NUM_THREADS 8


class GE{
    public:
    GE(std::string _baseDir, std::string (*_blackBox)(std::string), std::string (*_getQuestions)(int), int _numQuestions);
    void start();
    void pause();
    void stop();
    void* startThread(void);
    typedef std::pair<std::string, std::map<int, int> > Org;
    typedef  Queue<Org> OrgQueue;
    static void *runHelper(void *classRef)
    {
        return ((GE *)classRef)->startThread();
    }
    private:
    //Functions
    void initialize();
    std::string ask(std::string question, std::string orgID);
    std::string getQuestion();
    std::string nextName(std::string parent);
    void* start(void *args);
    bool compare(std::string correct, std::string response);
    void remove_directory(const char *path);
    Org emptyPair(std::string id);
    void kill(std::string name);
    long GetFileSize(std::string filename);
    int GENE_LENGTH;
    
    
    //Pointer functions
    std::string (*blackBox)(std::string);
    std::string (*getQuestions)(int);
    
    //Properties
    std::string ALPHEBET;
    OrgQueue queue;
    int numQuestions;
    std::vector<int> winners;
    std::vector<int> lossers;
    std::string exec(const char* cmd);
    std::string baseDir;
    Map<std::string, int> sibCounts;
    pthread_mutex_t lock;
    int threadsWaiting;
    float difficulty;
    int NUM_GENES = 100;
    
    Tracker tracker;
    Language language;
};
#endif

