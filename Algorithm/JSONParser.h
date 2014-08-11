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

