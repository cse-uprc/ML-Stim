// Neural Network

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
 
 
#define NMAX 1

typedef struct {
    float hBias[4];
    float outBias[11];
    float hWeightVector[11];
    float outWeightVector[4][11];
    float hNodes[4];
    float vExt[11];             // Array of Vext output values
} NeuralNetwork;

float genNormRand();
float sigmoid(float x);
void initNN(NeuralNetwork* NN);
void createVar(NeuralNetwork* NN1, NeuralNetwork* NN2);
void calcFwd(NeuralNetwork* NN);