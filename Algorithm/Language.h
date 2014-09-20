//
//  Language.h
//  GEDebug
//
//  Created by Andrew Shidel on 8/26/14.
//  Copyright (c) 2014 user. All rights reserved.
//

#ifndef GEDebug_Language_h
#define GEDebug_Language_h

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Language{
public:
    Language(){
        unsigned int time_ui = static_cast<unsigned int>( time(NULL) );
        srand( time_ui );
    }
    Language(std::vector<std::string> _files){
        files = _files;
        unsigned int time_ui = static_cast<unsigned int>( time(NULL) );
        srand( time_ui );
    }
    void run(){
        for (int i=0; i<files.size(); i++){
            std::ifstream istr (files[i]);
            
            if (istr) {
                std::streambuf * pbuf = istr.rdbuf();
                do {
                    char ch = pbuf->sgetc();
                    if (ch!='0')
                        lang[ch]++;
                } while ( pbuf->snextc() != EOF );
                
                istr.close();
            }
        }
        //delete &files;
        
        long int size=0;
        
        for(std::map<char, int>::iterator iterator = lang.begin(); iterator != lang.end(); iterator++) {
            size+=iterator->second;
            Pair *p = (Pair *)malloc(sizeof(Pair));
            p->prob = iterator->second;
            p->val = iterator->first;
            vals.push_back(*p);
            if (iterator->second > max)
                max = iterator->second;
            // iterator->first = key
            // iterator->second = value
        }
        std::sort(vals.begin(), vals.end(), sort_pred());
    }
    char randVal(){
        int randNum = rand()%max;
        for(std::vector<Pair>::iterator iterator = vals.begin(); iterator != vals.end(); iterator++) {
            if (randNum<iterator->prob)
                return iterator->val;
        }
        return '0';
    }
    
private:
    std::vector<std::string> files;
    std::map<char, int> lang;
    int max;
    typedef struct pair{
        int prob;
        char val;
    } Pair;
    std::vector<Pair> vals;
    struct sort_pred {
        bool operator()(const Pair &left, const Pair &right) {
            return left.prob < right.prob;
        }
    };
};

#endif
