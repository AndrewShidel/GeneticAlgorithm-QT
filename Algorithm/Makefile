compileMaker:
	g++ -o Maker.o -c Maker.cpp
compileLib: compileMaker
	g++ -o GE.o -c GE.cpp
compile: compileLib
	g++ -o GEtester GEtester.cpp GE.o Maker.o -lboost_system -lboost_filesystem
run: compile
	./GEtester
