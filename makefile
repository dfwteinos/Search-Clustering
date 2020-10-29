all: lsh cube 

main:  main.cpp arguments.h util.h hash_functions.h hash_tables.h
	g++ -g -c main.cpp -o main.o

arguments: arguments.cpp arguments.h
	g++ -g -c arguments.cpp -o arguments.o

util: util.cpp util.h
	g++ -g -c util.cpp -o util.o

hash_functions: hash_functions.cpp hash_functions.h
	g++ -g -c hash_functions.cpp -o hash_functions.o

hash_tables: hash_tables.cpp hash_tables.h
	g++ -g -c hash_tables.cpp -o hash_tables.o

lsh_functions:lsh_functions.cpp hash_tables.h
	g++ -g -c lsh_functions.cpp -o lsh_functions.o

cube_functions:cube_functions.cpp hash_tables.h
	g++ -g -c cube_functions.cpp -o cube_functions.o

lsh: main arguments util hash_functions hash_tables lsh_functions cube_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o lsh_functions.o cube_functions.o -o lsh

cube: main arguments util hash_functions hash_tables cube_functions lsh_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o cube_functions.o lsh_functions.o -o cube

clean:
	-rm -f all main.o arguments.o util.o hash_functions.o hash_tables.o lsh_functions.o cube_functions.o
