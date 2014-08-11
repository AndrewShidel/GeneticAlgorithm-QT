#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Maker.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <map>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <regex>

#include <curl/curl.h>

using namespace std;

Maker::Maker(std::string baseDir, std::string child, std::string parent, int mutationRate, int insertRate){
    path = baseDir;
    gen = child;
    oldGen = parent;
    mutRate = mutationRate;
    InsertRate = insertRate;
}

//Takes a path to a file (minus the filename), makes small changes, and creates a new file with the name "gen".
void Maker::make(string outName, string inName){

    //Seed the random number generator.
    unsigned int time_ui = static_cast<unsigned int>( time(NULL) );
    srand( time_ui );

    //Get the path for the output file.
    std::ostringstream oss;
    oss << path<<gen<<"/"<<outName;
    string dirPath=oss.str();

    //Create input and output streams.
    ofstream out((dirPath).c_str(), ios::out | ios::binary);
    std::ifstream istr ((path+oldGen+"/"+inName).c_str());
    
    //If the input file exists.
    if (istr) {
        //Get a stream buffer from the input stream.
        std::streambuf * pbuf = istr.rdbuf();
        do {
            //Get the current charactor.
            char ch = pbuf->sgetc();
            
            //Make an intentional mistake.
            if (1+rand()%mutRate==1){ ch=(char)(1+rand()%255);}
            if (1+rand()%InsertRate==1){ out << (char)(1+rand()%255);}

            //Write the charactor to the output file.
            out << ch;
        //Get next charactor, and loop.
        } while ( pbuf->snextc() != EOF );

        //Close the streams.
        istr.close();
        out.close();
    }
    
    //Give the new file excecute permissions.
    chmod(dirPath.c_str(), S_IRWXU);
    //exec((char*)( oss2.str() ).c_str());
    return;    
}

int Maker::split(string path, string filename, string outputDir){
    string buffer = "";
    vector<string> files, protoTypes;
    string globals;
    bool inGlobal = false;
    files.push_back(""); //Push the main file.
    ifstream fin;
    fin.open((path+filename).c_str());
    string str((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    int bracketCount = 0;
    bool inMain = false;
    
    for (int i = 0; i < str.length(); i++){
        char c = str[i];
        buffer.push_back(c);

        if (buffer == "#" && bracketCount == 0) 
            inGlobal = true;

        if (c == '{'){
            bracketCount++;
            if (bracketCount == 1){
                files.push_back("");
                protoTypes.push_back(buffer.substr(0, buffer.size()-1)+";");
            }
        } 
        else if (c == '}') {
            bracketCount--;
            buffer.push_back(str[++i]);

            if (!inMain)
                files[files.size()-1].append(buffer);
            else{
                files[0].append(buffer);
                if (bracketCount==0) inMain = false;
            }
            buffer = "";
            continue;       
        }

        //If not in a function.
        if (bracketCount==0){
            if (c=='\n'){
                if (inGlobal) {
                    globals.append(buffer);
                    inGlobal = false;
                }else
                    files[0].append(buffer);
                buffer = "";
            }
        }else if (bracketCount==1){
            if (c=='\n'){
                regex rx("\\s+main\\s*\\(");
                bool match = regex_search(buffer.begin(), buffer.end(), rx);
                if (match) {
                    inMain = true;
                    files.pop_back();
                }
                int index = 0;
                if (!match && !inMain) index = files.size()-1;
                files[index].append(buffer);
                buffer = "";
            }
        }
    }

    for (int i = 0; i < files.size(); i++){
        for(int j = 0; j<protoTypes.size(); j++){
            //std::stringstream sstm;
            //sstm << "#include \""<<j<<".hpp\"";
            //files[i].insert(0,sstm.str());
            files[i].insert(0,"extern "+protoTypes[j]);
        }
        files[i].insert(0, globals);

        string ext;
        if (i==0)
            ext = ".cpp";
        else
            ext = ".cpp";
        ofstream file;

        std::stringstream sstm;
        sstm << outputDir<<i<<ext;
        file.open (sstm.str());
        file << files[i];
        file.close();
    }
    return files.size();
}