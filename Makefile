main: main.o knn.o
	g++ main.o knn.o -o main && rm *.o

main.o: main.cpp
	g++ -c main.cpp

knn.o: knn.cpp knn.h
	g++ -c knn.cpp

clean:
	rm main
