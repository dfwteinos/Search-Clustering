CFLAGS+= -g3 -Wall -Wextra

all: lsh cube cluster

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

cluster_functions: cluster_functions.cpp hash_tables.h util.h
	g++ -g -c cluster_functions.cpp -o cluster_functions.o

lsh: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o -o lsh

cube: main arguments util hash_functions hash_tables cube_functions lsh_functions cluster_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o cube_functions.o lsh_functions.o cluster_functions.o -o cube

cluster: main arguments util hash_functions hash_tables cube_functions lsh_functions cluster_functions.cpp
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o cube_functions.o lsh_functions.o cluster_functions.o -o cluster

# gdblsh: g++ -g3 -o lsh arguments.cpp hash_functions.cp util.cpp main.cpp hash_tables.cpp && gdb --args lsh -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -o output.txt

clean:
	-rm -f all: lsh cube cluster *.o *.txt

gdblsh: main arguments util hash_functions hash_tables lsh_functions cube_functions
	g++ -g3 -o lsh arguments.o hash_functions.o util.o main.o hash_tables.o	lsh_functions.o cube_functions.o && gdb --args lsh -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -o output.txt

gdbcube: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -g3 -o cube arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && gdb --args cube -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -o output.txt

gdbclu: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -g3 -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o	lsh_functions.o cube_functions.o cluster_functions.o && gdb --args cluster -i train-images-idx3-ubyte -c cluster.conf -o output.txt -complete yes -m Classic

vallsh: main arguments util hash_functions hash_tables lsh_functions cube_functions
	g++ -o lsh arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o && valgrind ./lsh -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -o output.txt

valcube: main arguments util hash_functions hash_tables lsh_functions cube_functions
	g++ -o cube arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o && valgrind ./cube -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -o output.txt

valclu:  main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && valgrind ./cluster -i train-images-idx3-ubyte -c cluster.conf -o output.txt -complete yes -m Classic