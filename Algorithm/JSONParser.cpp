#include <map>
#include <string>
#include <iostream>
#include <fstream>

class JSONParser {
    std::string filename;
  public:
    JSONParser(std::string file);
    std::map <int, int> parse();
};

JSONParser::JSONParser (std::string file){
  filename=file;
}

std::map <int, int> parse () {
  std::map <int, int> m;
  std::ifstream file ("example.txt");
  if (file.is_open()) {
    char c;
    bool inkey=true;
    int quoteCounter=0;
    int key;
    bool inquote=false;
    std::string buffer;
    while ( file.get(c) ) {
      if (c=='\"'){
        if (inquote){
          if (inkey)
            key = atoi(buffer.c_str());
          else
            m[key] = atoi(buffer.c_str());
        }
        quoteCounter++;
        if (quoteCounter==1){
          inkey=true;
        }else if(quoteCounter==4){
          quoteCounter=0;
          inkey=false;
        }else{
          inkey=false;
        }
        buffer="";
        inquote=!inquote;
      }else{
        buffer+=c;
      }
    }
    file.close();
  }
  return m;
}