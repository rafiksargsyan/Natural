OPTIONS = -c --debug

main: Natural.o main.o
	g++ -O3 Natural.o main.o -o main
Natural.o: Natural.cpp Natural.h
	g++ -O3 -c -std=c++0x Natural.cpp
main.o: main.cpp Natural.h
	g++ -c -std=c++0x main.cpp
clean:
	rm *.o

