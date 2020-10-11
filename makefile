all: lsh cube 

main:  main.cpp arguments.h util.h
	g++ -g -c main.cpp -o main.o

arguments: arguments.cpp arguments.h
	g++ -g -c arguments.cpp -o arguments.o

util: util.cpp util.h
	g++ -g -c util.cpp -o util.o

lsh: main arguments util
	g++ main.o arguments.o util.o -o lsh

cube: main arguments util
	g++ main.o arguments.o util.o -o cube

clean:
	-rm -f all main.o arguments.o util.o