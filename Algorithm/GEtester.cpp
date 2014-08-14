#include <string>
#include <vector>
#include "GE.h"

#define NUM_QUESTIONS 100;

int iBlackBox(int i){
	if (i%7==1) return i*7;
	if (i%6==0) return i*6;
	if (i%5==1) return i*5;
	if (i%4==0) return i*4;
	if (i%3==1) i = i*3;
	if (i%2==0) i = i*2;
	return i;
}

//TOTO: This needs to return string.
std::string BlackBox(std::string question){
	int i = atoi(question.c_str());
	return std::to_string(iBlackBox(i)); //Ignore this error.
}
std::string getQuestion(int i){
	return "";
}

int main(){
	std::vector<std::string> questions;
	GE test = GE("./orgs/", &BlackBox, &getQuestion, NUM_QUESTIONS);
	test.start();
	return 0;
}
