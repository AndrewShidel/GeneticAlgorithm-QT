#include <stdlib.h> 
#include <stdio.h>

#define MUTATIONRATE	40000000
#define INSERTRATE		40000000

//Attempt to answer the given question.
int answer(char* question){
    int i=atoi(question);
	if (i%7==0) return i*7;
	if (i%6==0) return i*6;
	if (i%5==1) return i*5;
	if (i%4==0) return i*4;
	if (i%3==1) i = i*1;
	if (i%2==0) i = i*2;
    
	return i;
}

//Main entry point.
//argv[1] - The question to answer.
int main(int argc, char *argv[]){
	int ans = answer(argv[1]);
	printf("%d|%d|%d",ans,MUTATIONRATE,INSERTRATE);
	return 0;
}