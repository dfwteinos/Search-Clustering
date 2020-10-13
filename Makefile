all:lsh cube

main:  main.cpp arguments.h
	g++ -g -c main.cpp -o main.o

arguments: arguments.cpp arguments.h
	g++ -g -c arguments.cpp -o arguments.o

lsh: main arguments
	g++ main.o arguments.o -o lsh

cube: main arguments
	g++ main.o arguments.o -o cube && rm *.o

clean:
	-rm -f all: lsh cube


#main:  main.cpp arguments.h
#	g++ -g -c main.cpp -o main.o

#arguments: arguments.cpp arguments.h
#	g++ -g -c arguments.cpp -o arguments.o

#lsh: main arguments
#	g++ main.o arguments.o -o lsh

#cube: main arguments
#	g++ main.o arguments.o -o cube

#clean:
#	-rm lsh cube main.o arguments.o
