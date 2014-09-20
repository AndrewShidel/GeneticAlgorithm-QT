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

Maker::Maker(){
    
}

typedef struct org_struct{
    std::map<int, int> *offsetMap;
    std::streambuf * inputStream;
} org;

//TOTO: Try and call this from the main code.
int Maker::make2(std::string *baseDir,  std::pair<std::string, std::map<int, int> > *child, std::pair<std::string, std::map<int, int> > *parent1, std::pair<std::string, std::map<int, int> > *parent2, int baseGeneLength, int numGenes, int mutationRate, int insertRate, Language *lan){
    
    //Seed the random number generator.
    unsigned int time_ui = static_cast<unsigned int>( time(NULL) );
    srand( time_ui );
    
    //Get the path for the new file.
    string dirPath=*baseDir+child->first;
    
    //Child's output stream
    ofstream out(dirPath.c_str(), ios::out | ios::binary);
    
    //Create input streams for each parent.
    std::ifstream istr1 ((*baseDir + parent1->first).c_str());
    std::ifstream istr2 ((*baseDir + parent2->first).c_str());
    
    //Create parent 1's org object.
    org *parent_1 = (org *)malloc(sizeof(org));
    parent_1->offsetMap=&(parent1->second);
    parent_1->inputStream = istr1.rdbuf();
    
    //Create parent 2's org object.
    org *parent_2 = (org *)malloc(sizeof(org));
    parent_2->offsetMap=&(parent2->second);
    parent_2->inputStream = istr2.rdbuf();
    
    //Pointer to whichever org is currently in use.
    org *inUse = (org *)malloc(sizeof(org));
    org *notInUse = (org *)malloc(sizeof(org));
    
    //The genes covered so far, and the bits covered so far within a gene.
    int genesCovered=0, bitsCovered;
    
    //Marker offsets for both parents.
    int inUseOffset = 0;
    int notInUseOffset = 0;
    
    //An iterator for searching through each parent's offset Map.
    std::map<int,int>::iterator it;
    char ch;
    int nextc=0;
    
    if (istr1 && istr2){
        while (genesCovered<numGenes+1) {
            if (rand()%2==0){
                inUse = parent_1;
                notInUse = parent_2;
            }else{
                inUse = parent_2;
                notInUse = parent_1;
            }
            //Get the marker offset for the first parent.
            it = inUse->offsetMap->find(genesCovered);
            if (it!=inUse->offsetMap->end()){
                inUseOffset = (*it).second;
                (child->second)[genesCovered] = inUseOffset;
            }else{
                inUseOffset = 0;
            }
            //Get the maqker offset for the second parent.
            it = notInUse->offsetMap->find(genesCovered);
            if (it!=notInUse->offsetMap->end()){
                notInUseOffset =(*it).second;
            }else{
                notInUseOffset = 0;
            }
            
            bitsCovered = 0;
            //Get each byte of the gene.
            while (bitsCovered < baseGeneLength+inUseOffset && nextc!=EOF){
                //Get the next charactor.
                ch = inUse->inputStream->sgetc();

                //Make an intentional mistake.
                if (1+rand()%mutationRate==1){ ch=lan->randVal();}
                if (1+rand()%insertRate==1){
                    out << lan->randVal();
                    //Increment the offset.
                    ++((*(inUse->offsetMap))[genesCovered]);
                }
                
                //Write the char to the child's file.
                out << ch;

                ++bitsCovered;
                nextc = inUse->inputStream->snextc();
                
            }
            //Increment the unused inputStream to the next gene.
            notInUse->inputStream->pubseekoff( baseGeneLength+notInUseOffset, ios_base::cur);
            ++genesCovered;
        }
        
    }else{
        return 1;
    }
    //Give the child execute permissions.
    chmod(dirPath.c_str(), S_IRWXU);
    
    istr1.close();
    istr2.close();
    out.close();
    return 0;
}

//Takes a path to a file (minus the filename), makes small changes, and creates a new file with the name "gen".
void Maker::make(std::string *path, std::string *gen, std::string *oldGen, int mutRate, int InsertRate){
    
    //Seed the random number generator.
    unsigned int time_ui = static_cast<unsigned int>( time(NULL) );
    srand( time_ui );
    
    //Get the path for the new file.
    string dirPath=*path+*gen;
    
    //Create input and output streams.
    ofstream out(dirPath.c_str(), ios::out | ios::binary);
    std::ifstream istr ((*path+*oldGen).c_str());
    
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
