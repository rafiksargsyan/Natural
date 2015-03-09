OPTIONS = -c --debug

main: Natural.o main.o
	g++ -g Natural.o main.o -o main
Natural.o: Natural.cpp Natural.h
	g++ -g -c -std=c++0x Natural.cpp
main.o: main.cpp Natural.h
	g++ -g -c -std=c++0x main.cpp
clean:
	rm *.o

