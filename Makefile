controller: Controller.o Sim.o ML.o -lm
	gcc -g -o controller -fopenmp Controller.o Sim.o Ml.o -lm

test: test.o Sim.o ML.o
	gcc -g -o test test.o Sim.o ML.o -lm

test.o: test.c Sim.h
	gcc -c -g test.c

Sim.o: Sim.c Sim.h
	gcc -c -g Sim.c -lm

ML.o: ML.c ML.h
	gcc -c -g ML.c -lm

Controller.o : Controller.c Sim.h ML.h
	gcc -c -g -fopenmp Controller.c -lm
