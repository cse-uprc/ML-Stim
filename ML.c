// Neural Network

#include <stdio.h>
#include "ML.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
 
 
#define NMAX 1
#define NSUM 15

float genNormRand(){
    // genNormRand() generates a random floating value between -1 and 1 based on a normal curve.

    // It has something to do with the sinusoidal functions... just accept it.
    float U = ((float)(rand() % 100) / 100.0);
    float V = ((float)(rand() % 100) / 100.0);

    float num = sqrt(-2.0 * log(U)) * sin(2.0 * 3.1415926 * V);
    //printf("%f\n",num);
    return num;
}

double gaussrand()
{
	double x = 0;
	int i;
	for(i = 0; i < NSUM; i++)
		x += (double)rand() / RAND_MAX;

	x /= NSUM;
    x = x*2-1;
    // printf("%f\n",x);
	return x;
}

float sigmoid(float x){
    // 

     float exp_value;
     float return_value;

     /*** Exponential calculation ***/
     exp_value = exp((double) -x);

     /*** Final sigmoid value ***/
     return_value = 1 / (1 + exp_value);

     return (return_value * 2.0) - 1.0;
}

void initNN(NeuralNetwork* NN){
    // Initialization function for the Nueral Network

    srand(time(0));

    // For each vector, generate normal random values between -1 and 1
    for (int i = 0; i < 4; i++)
    {
        NN->hBias[i] = gaussrand();
        NN->hWeightVector[i] = gaussrand();
        for (int j = 0; j < 11; j++)
        {
            NN->outWeightVector[i][j] = gaussrand();
        }
    }

    for (int i = 0; i < 11; i++)
    {
        NN->outBias[i] = gaussrand();
    }
};

void createVar(NeuralNetwork* NN1, NeuralNetwork* NN2)
{
    //
    // printf("%f\n\n", (1.0 + (((float)(rand() % 1000) - 500.0) / 10000.0)));
    //
    for (int i = 0; i < 4; i++)
    {
        NN2->hBias[i] = NN1->hBias[i] * (1.0 + (((float)(rand() % 2000) - 1000.0) / 10000.0));
        // printf("hBias - %f\n",NN2->hBias[i]);
        for (int j = 0; j < 11; j++)
        {
            NN2->outWeightVector[i][j] = NN1->outWeightVector[i][j] * (1.0 + (((float)(rand() % 2000) - 1000.0) / 10000.0));
            // printf("outWeightVec - %f\n", NN2->outWeightVector[i][j]);
        }
        NN2->hWeightVector[i] = NN1->hWeightVector[i] * (1.0 + (((float)(rand() % 2000) - 1000.0) / 10000.0));
        // printf("hWeightVec - %f\n", NN2->hWeightVector[i]);
    }

    for (int i = 0; i < 11; i++)
    {
        NN2->outBias[i] =  NN1->outBias[i] * (1.0 + (((float)(rand() % 2000) - 1000.0) / 10000.0));
        // printf("outBias - %f\n", NN2->outBias[i]);
    }
}

void calcFwd(NeuralNetwork* NN){
    // Calculate forward propogation on Neural Network

    // Generate a random number (randomly chosen)
    double initVal = gaussrand();

    // node * weight + bias
    for (int i = 0; i < 4; i++)
    {
        NN->hNodes[i] = sigmoid((initVal * NN->hWeightVector[i]) + NN->hBias[i]);
    }

    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 11; i++)
        {
            NN->vExt[i] = sigmoid((NN->hNodes[j] * NN->outWeightVector[j][i]) + NN->outBias[i]);
            NN->vExt[i] = NN->vExt[i] * 50.0;
        }
    }
}