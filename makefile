OPTIONS = -c --debug

main: Natural.o main.o
	g++  Natural.o main.o -o main
Natural.o: Natural.cpp Natural.h
	g++ -c --debug -std=c++0x Natural.cpp
main.o: main.cpp Natural.h
	g++ -c --debug -std=c++0x main.cpp
clean:
	rm *.o

