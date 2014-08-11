#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <queue>
#include <unistd.h>
#include <string>
#include <boost/filesystem.hpp>


using namespace boost::filesystem;

#define NUM_THREADS 25

void* start(void *args);
std::string ask(std::string question);
std::string getQuestion();
std::string getAnswer(std::string question);
bool compare(std::string correct, std::string response);
void remove_directory(const char *path);

class GE{
	public:
		GE();
		void begin();
};

std::queue<std::string> queue;

typedef struct args_for_thread_t{
		  int threadID; 
		  int start; 
		  int end;
		  float *arr;
} ARGS_FOR_THREAD; 


//TODO: I may want to redesign this as a class.
int main(){
	pthread_t tid[NUM_THREADS];
	ARGS_FOR_THREAD *args;

	mkdir("./orgs/0", 0777);

	/*for (int i=0; i<NUM_THREADS; i++){
		pthread_create(&tid[i],NULL,start, NULL);
	}*/
}

void* start(void *args){
	while(queue.empty()){
		usleep(10000);
	}
	std::string id = queue.front();
	queue.pop();

	std::string question = getQuestion();
	std::string response = ask(question);
	std::string correctAnswer = getAnswer(question);

	bool result = compare(correctAnswer, response);

	if (result){
		//TODO: Find mate and run maker.
	}else{
		remove_directory(("./orgs/"+id).c_str());
		start(NULL);
	}
	return NULL;
}

//Get a question from the black box
std::string getQuestion(){
	return "";
}

//Ask the org a question
std::string ask(std::string question){
	return "";
}
//Get the answer from the black box
std::string getAnswer(std::string question){
	return "";
}

bool compare(std::string correct, std::string response){
	return false;
}

void remove_directory(const char *path) {
	boost::filesystem::path p (path);
	remove_all(p);
}
