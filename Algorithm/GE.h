#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <queue>
#include <unistd.h>
#include <string>
#include <vector>

#define NUM_THREADS 25


class GE{
	public:
		GE(std::string _baseDir, std::string (*_blackBox)(std::string), std::vector<std::string> questions);
		void start();
		void pause();
		void stop();
	private:
		void* start(void *args);
		std::string ask(std::string question, std::string orgID);
		std::string getQuestion();
		bool compare(std::string correct, std::string response);
		void remove_directory(const char *path);
		std::queue<std::string> queue;
		static void* startThread(void *args);
		std::string (*blackBox)(std::string);
		void initialize();
		std::vector<std::string> questions;
		std::vector<int> winners;
		std::vector<int> lossers;
		std::string exec(const char* cmd);
		std::string baseDir;
};



typedef struct args_for_thread_t{
		  int threadID; 
		  int start; 
		  int end;
		  float *arr;
} ARGS_FOR_THREAD; 


