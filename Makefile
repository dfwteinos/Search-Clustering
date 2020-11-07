CFLAGS+= -g3 -Wall -Wextra

all: lsh cube cluster

main:  src/main.cpp include/arguments.h include/util.h include/hash_functions.h include/hash_tables.h
	g++ -g -c src/main.cpp -o main.o

arguments: src/arguments.cpp include/arguments.h
	g++ -g -c src/arguments.cpp -o arguments.o

util: src/util.cpp include/util.h
	g++ -g -c src/util.cpp -o util.o

hash_functions: src/hash_functions.cpp include/hash_functions.h
	g++ -g -c src/hash_functions.cpp -o hash_functions.o

hash_tables: src/hash_tables.cpp include/hash_tables.h
	g++ -g -c src/hash_tables.cpp -o hash_tables.o

lsh_functions: src/lsh_functions.cpp include/hash_tables.h
	g++ -g -c src/lsh_functions.cpp -o lsh_functions.o

cube_functions:src/cube_functions.cpp include/hash_tables.h
	g++ -g -c src/cube_functions.cpp -o cube_functions.o

cluster_functions: src/cluster_functions.cpp include/hash_tables.h include/util.h
	g++ -g -c src/cluster_functions.cpp -o cluster_functions.o

lsh: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o -o lsh

cube: main arguments util hash_functions hash_tables cube_functions lsh_functions cluster_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o cube_functions.o lsh_functions.o cluster_functions.o -o cube

cluster: main arguments util hash_functions hash_tables cube_functions lsh_functions cluster_functions
	g++ main.o arguments.o util.o hash_functions.o hash_tables.o cube_functions.o lsh_functions.o cluster_functions.o -o cluster

# gdblsh: g++ -g3 -o lsh arguments.cpp hash_functions.cp util.cpp main.cpp hash_tables.cpp && gdb --args lsh -d train-images-idx3-ubyte -q t10k-images-idx3-ubyte -o output.txt

clean:
	-rm -f all: lsh cube cluster *.o *.txt

gdblsh: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -g3 -o lsh arguments.o hash_functions.o util.o main.o hash_tables.o	lsh_functions.o cube_functions.o cluster_functions.o && gdb --args lsh -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output.txt

gdbcube: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -g3 -o cube arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && gdb --args cube -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output.txt

gdbclu: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -g3 -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o	lsh_functions.o cube_functions.o cluster_functions.o && gdb --args cluster -i data/t10k-images-idx3-ubyte -c conf/cluster.conf -o output.txt -complete yes -m Classic

vallsh: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o lsh arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && valgrind ./lsh -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output.txt

valcube: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cube arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && valgrind ./cube -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output.txt

valclu:  main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && valgrind ./cluster -i data/train-images-idx3-ubyte -c conf/cluster.conf -o output.txt -complete yes -m Classic

# Recommended running options, usign default values

lshdef: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o lsh arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && ./lsh -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output.txt

cubedef: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cube arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && ./cube -d data/train-images-idx3-ubyte -q data/t10k-images-idx3-ubyte -o output.txt

clucla: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && ./cluster -i data/train-images-idx3-ubyte -c conf/cluster.conf -o output.txt -complete yes -m Classic

clulsh: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && ./cluster -i data/train-images-idx3-ubyte -c conf/cluster.conf -o output.txt -complete yes -m LSH

cluhpc: main arguments util hash_functions hash_tables lsh_functions cube_functions cluster_functions
	g++ -o cluster arguments.o hash_functions.o util.o main.o hash_tables.o lsh_functions.o cube_functions.o cluster_functions.o && ./cluster -i data/train-images-idx3-ubyte -c conf/cluster.conf -o output.txt -complete yes -m HyperCube

