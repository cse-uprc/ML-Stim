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

    // run through 100 generation of networks
    for(int genNum=0; genNum<100; genNum++){
        printf("GEN NUM: %d\n",genNum);
        double scores[genSize];
        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < genSize; i++)
            {
                printf("SPECIES: %d\n", i);
                scores[i] = testNetwork(&gen[i], data);
            }
        }
        
        for (int i = 1; i < genSize; i++)
        {
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
        char buf[20];
        snprintf(buf, 20, "gen%02d.csv", genNum);
        writeToFile(data, buf);
    }    
}

double testNetwork(NeuralNetwork *NN, float data[10000][205])
{   // testNetwork takes in a neural network that is meant to generate a set of extracellular voltages
    //  and gives it a score based on how well it blocks APs
    // This is done by having a continuous injected current (to cause APs) and then
    //  checking for spikes on the opposite end of the axon. Every AP measured means the NN loses a point

    // Setup the environment
    double score = 0;
    struct simEnv env;
    for (int i = 0; i < 51; i++)
    {
        // set initial conditions for each compartment
        // init v=-65,m=.052,h=.596,n=.317
        env.compartments[i] = (struct compartment *)malloc(sizeof(struct compartment));

        env.compartments[i]->v = -65.0;

        env.compartments[i]->m = .052;
        env.compartments[i]->h = .596;
        env.compartments[i]->n = .317;

        env.compartments[i]->vext = 0;
    }
    for (int i = 0; i < 51; i++)
    {
        env.compartments[i]->left = (i > 0) ? env.compartments[i - 1] : NULL;
        env.compartments[i]->right = (i < 50) ? env.compartments[i + 1] : NULL;
    }

    // Define the injected current
    float inject[51];
    for (int i = 0; i < 51; i++)
    {
        inject[i] = 0.0;
    }
    inject[0] = 10.0;

    // Run the trial
    bool firing = false;
    for (int timeSteps = 0; timeSteps < 10000; timeSteps++)
    {
        // calculate the extracellular voltages
        calcFwd(NN);
        
        // Set the extracellular voltage for center 11 compartments
        for (int i = 0; i <= 10; i++)
        {
            env.compartments[i + 20]->vext = NN->vExt[i];
        }

        // take a single time step (.01ms)
        takeTimeStep(env, .01, inject, timeSteps, data);

        // check for firing in the final compartment
        if (!firing && env.compartments[50]->v > 0)
        {
            firing = true;
            score -= 1;
        }
        else if (firing && env.compartments[50]->v < 0)
        {
            firing = false;
        }
    }
    return score;
}
