#include <stdlib.h>
#include <string>

class Maker{
	public:
		Maker(std::string baseDir, std::string child, std::string parent, int mutationRate, int insertRate);
		void make(std::string resultID, std::string inputID);
		static int split(std::string path, std::string filename, std::string outputDir);
	private:
		std::string path;
		std::string gen;
		std::string oldGen;
		int mutRate;
		int InsertRate;
};