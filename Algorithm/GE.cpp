#include "GE.h"
#include <pthread.h>
#include <vector>
#include <sstream>
#include <utility>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <signal.h>
#include "Maker.h"
#include "statTracker.h"
#include "Map.h"

void handler (int cause, siginfo_t * info, void *uap);


GE::GE(std::string _baseDir, std::string (*_blackBox)(std::string), std::string (*_getQuestions)(int), int _numQuestions){

    tracker.init();
    tracker.data.generations = 5;
    //Create the language.
    std::vector<std::string> files;
    files.push_back("/Users/andrew/Documents/language/language1.o");
    files.push_back("/Users/andrew/Documents/language/language1.o");
    files.push_back("/Users/andrew/Documents/language/language1.o");
    language = *new Language(files);
    language.run();

    //Initialize variables.
	blackBox = _blackBox;
	getQuestions = _getQuestions;
	baseDir=_baseDir;
	numQuestions = _numQuestions;
	ALPHEBET = "abcdefghijklmnopqrstuvwxyz0123456789";
	threadsWaiting = 0;

	unsigned int time_ui = static_cast<unsigned int>( time(NULL) );
	srand( time_ui );
}

void GE::start(){
	pthread_t tid[NUM_THREADS];

    //Compile the first executable.
	exec(("g++ -o "+baseDir+"0 "+baseDir+"main.cpp").c_str());

    //Make a copy of it.
    boost::filesystem::copy(baseDir+"0", baseDir+"1");

    //Add each to the queue.
	queue.push(emptyPair("0"));
    queue.push(emptyPair("1"));

    //Calculate the Gene length based off of the number of genes.
    GENE_LENGTH = (int)(GetFileSize(baseDir+"0")/NUM_GENES);

    //Ask each available question
    initialize();

    //Create each thread.
	for (int i=0; i<NUM_THREADS; i++){
        //pthread_create(&t, NULL, &Foo::runHelper, &foo);
		pthread_create(&tid[i],NULL, &GE::runHelper, this);
	}

    //Wait for each thread to finish.
	for (int i=0; i<NUM_THREADS; i++){
		pthread_join(tid[i], NULL);
	}
}

void GE::initialize(){
	std::cout << "Initializing:\n";
    std::vector<std::string> answers;
	for (int i=0; i<numQuestions; ++i){

		std::cout << "\r" << (int)((i/(float)numQuestions)*100) << "% completed: ";
		std::cout.flush();
		std::string question = getQuestions(i);
		std::string response = ask(question, "0");
        boost::split(answers, response, boost::is_any_of("|"));


		if (answers[0] == blackBox(question))
        winners.push_back(i);
		else
        lossers.push_back(i);
	}
}



void* GE::startThread(void){
	//GE *obj = (GE *)args;
	//bool waiting = false;
	std::vector<std::string> answers;

    std::cout << "Testing...";

    //Stores each organism
	Org org;

    //Stores the organism's mate.
    Org back;

    //Represents the child that will be produced.
    std::pair<std::string, std::map<int,int> > childMap;

	do{
        //Pop from the front of the queue, and save the result in 'org'.
        queue.pop(org);

        //Get a question, and ask it.
        std::string question = getQuestion();
        std::string response = ask(question, org.first);

        if (response.empty()){
            kill(org.first);
            continue;
        }

        //Get each part of the response (ans|mutationRate|insertRate)
        boost::split(answers, response, boost::is_any_of("|"));
        if (answers.size()<3){
            kill(org.first);
            continue;
        }
        //Get the correct ansewer, and compare.
        std::string correctAnswer = blackBox(question);
        bool result = compare(correctAnswer, answers[0]);

        if (result){
            //TODO: Find mate using a realistic process.
            childMap = emptyPair(nextName(org.first));
            if (childMap.first.empty())
                continue;
            back = queue.back();
            Maker::make2(&baseDir, &childMap, &org, &back, GENE_LENGTH, NUM_GENES,  std::stoi(answers[1]),  std::stoi(answers[2]), &language);
            queue.push(childMap);
            queue.push(org);
        }else{
            kill(org.first);
        }

	}while(true);
}

//Kills an organism.
void GE::kill(std::string name){
	if(boost::filesystem::exists(baseDir+name)){
        boost::filesystem::remove(baseDir+name);
        pthread_mutex_lock(&(lock));
        sibCounts.erase(name);
        pthread_mutex_unlock(&(lock));
    }
}

//Get the next filename based on the parent's name
//parent: The name of the parent's executable.
std::string GE::nextName(std::string parent){
    int sibCount;
    sibCount = sibCounts.get(parent);
    pthread_mutex_lock(&(lock));
	if (sibCount>=ALPHEBET.length()-1){
        parent = "-"+parent;
        sibCounts.set(parent, 0);
    }
    sibCounts.set(parent, ++sibCount);
    pthread_mutex_unlock(&(lock));
    return parent+ALPHEBET[sibCount];
}

void GE::pause(){

}

void GE::stop(){

}
void finish(){
	printf("Done");
	exit(0);
}

//Get a question from the black box
std::string GE::getQuestion(){
	//tracker.questionsAsked++;
	std::string question;
	int randNum = rand();
	if (queue.size()<3)
    return std::to_string(winners[randNum % winners.size()]);
	else if(randNum%50>queue.size()){
		question = std::to_string(winners[randNum % winners.size()]);
	}else{
		question = std::to_string(lossers[randNum % lossers.size()]);
	}
	if (queue.size()>100)
    finish();
	return question;
}



//Ask the org a question
std::string GE::ask(std::string question, std::string orgID){
    //Needs a timeout to kill Rogue processes.
    clock_t startTime = clock();
	return exec((baseDir+orgID+" "+question).c_str());
    std::cout << "exe time: " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
}

bool GE::compare(std::string correct, std::string response){
	return correct==response;
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

std::pair<std::string,std::map<int,int> > GE::emptyPair(std::string id){
	return std::pair<std::string,std::map<int,int> >(id, std::map<int,int>() );
}
long GE::GetFileSize(std::string filename){
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

