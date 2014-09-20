#include <stdlib.h>
#include <string>
#include <map>

#include "Language.h"

class Maker{
	public:
		Maker();
		static void make(std::string *baseDir, std::string *child, std::string *parent, int mutationRate, int insertRate);
        static int make2(std::string *baseDir,  std::pair<std::string, std::map<int, int> > *child, std::pair<std::string, std::map<int, int> > *parent1, std::pair<std::string, std::map<int, int> > *parent2, int baseGeneLength, int numGenes, int mutationRate, int insertRate, Language *lan);

		static int split(std::string path, std::string filename, std::string outputDir);

};