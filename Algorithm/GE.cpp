#include "GE.h"
#include <pthread.h>
#include <vector>
#include <boost/filesystem.hpp>
#include "Maker.h"

using namespace boost::filesystem;

GE::GE(std::string _baseDir, std::string (*_blackBox)(std::string), std::vector<std::string> _questions){
	blackBox = _blackBox;
	questions = _questions;
	baseDir=_baseDir;
}

void GE::start(){
	pthread_t tid[NUM_THREADS];

	mkdir("./orgs/1", 0777);
	Maker m("","","",0,0);
	m.make("", "");
	Maker::split("","","");
	initialize();

	for (int i=0; i<NUM_THREADS; i++){
		pthread_create(&tid[i],NULL, &GE::startThread, (void *)this);
	}
}

void GE::initialize(){
	std::cout << "Initializing:\n";
	for (int i=0; i<questions.size(); ++i){

		std::cout << "\r" << (int)((i/(float)questions.size())*100) << "% completed: ";
		std::cout.flush();

		std::string response = ask(questions[i], "0");
		if (response == blackBox(questions[i]))
			winners.push_back(i);
		else
			lossers.push_back(i);
	}
}


void* GE::startThread(void *args){
	GE *obj = (GE *)args;
	while(obj->queue.empty()){
		usleep(10000);
	}
	std::string id = obj->queue.front();
	obj->queue.pop();

	std::string question = obj->getQuestion();
	std::string response = obj->ask(question, id);
	std::string correctAnswer = obj->blackBox(question);

	bool result = obj->compare(correctAnswer, response);

	if (result){
		//TODO: Find mate and run maker.
	}else{
		obj->remove_directory(("./orgs/"+id).c_str());
		obj->startThread(NULL);
	}
	return NULL;
}

void GE::pause(){
	
}

void GE::stop(){

}

//Get a question from the black box
std::string GE::getQuestion(){
	return "";
}

//Ask the org a question
std::string GE::ask(std::string question, std::string orgID){
	return exec((baseDir+orgID+"main "+question).c_str());
}

bool GE::compare(std::string correct, std::string response){
	return false;
}

void GE::remove_directory(const char *path) {
	boost::filesystem::path p (path);
	remove_all(p);
}

//Executes a shell command.
std::string GE::exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}
