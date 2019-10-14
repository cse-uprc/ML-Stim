all: ./bin/controller.exe ./bin/test.exe
	rm *.o

./bin/controller.exe: Controller.o Sim.o ML.o -lm
	gcc -g -o ./bin/controller.exe -fopenmp Controller.o Sim.o ML.o -lm

./bin/test.exe: test.o Sim.o ML.o
	gcc -g -o ./bin/test.exe test.o Sim.o ML.o -lm

test.o: ./src/test.c ./src/Sim.h
	gcc -c -g ./src/test.c

Sim.o: ./src/Sim.c ./src/Sim.h
	gcc -c -g ./src/Sim.c -lm

ML.o: ./src/ML.c ./src/ML.h
	gcc -c -g ./src/ML.c -lm

Controller.o : ./src/Controller.c ./src/Sim.h ./src/ML.h
	gcc -c -g -fopenmp ./src/Controller.c -lm
