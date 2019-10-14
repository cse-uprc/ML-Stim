#include "Sim.h"
#include "ML.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define genSize 300

double testNetwork(NeuralNetwork *NN, float data[10000][205]);

int main()
{
    printf("WORK\n");
    // Welcome to our main!
    //
    float data[10000][205];

    NeuralNetwork gen[genSize];
    for (int i = 0; i < genSize; i++)
    {
        initNN(&gen[i]);
    }

    int highest = 0;

    //printf("Hey1\n");
    for(int genNum=0; genNum<100; genNum++){
        printf("GEN NUM: %d\n",genNum);
        double scores[genSize];
        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < genSize; i++)
            {
                printf("SPECIES: %d\n", i);
                //printf("Hey 123\ns");
                scores[i] = testNetwork(&gen[i], data);
            }
        }
        

        //printf("Hey2\n");

        
        for (int i = 1; i < genSize; i++)
        {
            //printf("checking %d against %d\n", i, highest);
            //printf("scores %f and %f\n", scores[i], scores[highest]);
            if (scores[i] > scores[highest])
            {
                highest = i;
            }
        }

        for(int i=0; i<genSize; i++){
            if(i!=highest){
                createVar(&(gen[highest]), &(gen[i]));
            }
        }

        double bestScore = testNetwork(&(gen[highest]), data);
        printf("READY TO WRITE TO FILE\n");
        printf("# APs, %f\n", bestScore);
        // MAKE SURE THE FILE IS CLOSED!
        char buf[20];
        snprintf(buf, 20, "gen%02d.csv", genNum);
        writeToFile(data, buf);
    }    
}

double testNetwork(NeuralNetwork *NN, float data[10000][205])
{
    double score = 0;
    struct simEnv env;
    //env.compartments = malloc(51*sizeof(struct compartment));
    for (int i = 0; i < 51; i++)
    {
        // set initial conditions for each compartment
        // init v=-65,m=.052,h=.596,n=.317
        // env.compartments[i];
        //printf("%d\n",i);
        env.compartments[i] = (struct compartment *)malloc(sizeof(struct compartment));

        env.compartments[i]->v = -65.0;

        env.compartments[i]->m = .052;
        env.compartments[i]->h = .596;
        env.compartments[i]->n = .317;

        env.compartments[i]->vext = 0;
        //printf("sad\n");
    }

    
    //sprintf("Hey 5\n");

    for (int i = 0; i < 51; i++)
    {
        env.compartments[i]->left = (i > 0) ? env.compartments[i - 1] : NULL;
        env.compartments[i]->right = (i < 50) ? env.compartments[i + 1] : NULL;
    }

    //printf("Hey 67\n");

    // TEST 1
    // MINIMUM FIRING FROM WAVEFORM
    float inject[51];
    for (int i = 0; i < 51; i++)
    {
        inject[i] = 0.0;
    }
    inject[0] = 10.0;

    //printf("Hey 59\n");
    bool firing = false;
    for (int timeSteps = 0; timeSteps < 10000; timeSteps++)
    {
        //printf("Hey 60\n");
        calcFwd(NN);
        for (int i = 0; i <= 10; i++)
        {
            env.compartments[i + 20]->vext = NN->vExt[i];
        }
        //printf("Hey 61\n");
        takeTimeStep(env, .01, inject, timeSteps, data);
        //printf("Hey 104\n");
        if (!firing && env.compartments[50]->v > 0)
        {
            firing = true;
            score -= 1;
        }
        else if (firing && env.compartments[50]->v < 0)
        {
            firing = false;
        }
        //printf("Hey 62\n");
    }
    return score;
}