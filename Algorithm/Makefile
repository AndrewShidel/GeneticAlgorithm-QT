compileStats:
	g++ -o statTracker.o -c statTracker.cpp -lboost_system -lboost_filesystem
compileMaker:
	g++ -o Maker.o -c Maker.cpp -lboost_system -lboost_filesystem
compileLib: compileMaker compileStats
	g++ -o GE.o -c GE.cpp -lboost_system -lboost_filesystem
compile: compileLib
	g++ -o GEtester GEtester.cpp GE.o Maker.o statTracker.o -lboost_system -lboost_filesystem
run: compile
	./GEtester
clean:
	find ./orgs/ -perm +100 -type f -delete
