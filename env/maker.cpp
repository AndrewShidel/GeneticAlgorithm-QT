#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <map>
#include <pthread.h>
#include <unistd.h>

#include <curl/curl.h>

using namespace std;

//Used for keeping logs of mutations.
void mutationPrint(string type){
    std::ostringstream oss;
    oss << "./mut/" << type << "_"<<(time(0)*1000);
    string dirPath=oss.str();
    ofstream myfile;
    myfile.open (dirPath.c_str());
    myfile << "Writing this to a file.\n";
    myfile.close();
}

//Executes a shell command.
std::string exec(char* cmd) {
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

//Takes a path to a file (minus the filename), makes small changes, and creates a new file with the name "gen".
void mutate(string path, string gen, string oldGen, int mutRate, int InsertRate){

    //Seed the random number generator.
    unsigned long tse = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    srand(tse);
    
    //Get the path for the output file.
    std::ostringstream oss;
    oss << path<<gen;
    string dirPath=oss.str();

    //Create input and output streams.
    ofstream out((dirPath).c_str(), ios::out | ios::binary);
    std::ifstream istr ((path+oldGen).c_str());
    
    //If the input file exists.
    if (istr) {
        //Get a stream buffer from the input stream.
        std::streambuf * pbuf = istr.rdbuf();
        do {
            //Get the current charactor.
            char ch = pbuf->sgetc();
            
            //Make an intentional mistake.
            if (1+rand()%mutRate==1){ ch=(char)(1+rand()%255); mutationPrint("alt");}
            if (1+rand()%InsertRate==1){ out << (char)(1+rand()%255); mutationPrint("add");};

            //Write the charactor to the output file.
            out << ch;
        //Get next charactor, and loop.
        } while ( pbuf->snextc() != EOF );

        //Close the streams.
        istr.close();
        out.close();
    }
    
    //Give the new file excecute permissions.
    std::ostringstream oss2;
    oss2 << "chmod +x "<<dirPath;
    exec((char*)( oss2.str() ).c_str());
    return;    
}



//Main entry point.
//argv[1] - path to the parent file minus the filename.
//argv[2] - the generation code for the child.
int main(int argc, const char * argv[])
{
    mutate(argv[1], argv[2], argv[3],atoi(argv[4]), atoi(argv[5]));
    return 0;
}