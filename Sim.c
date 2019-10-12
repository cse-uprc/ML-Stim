// Simulation environment
#include "Sim.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fenv.h>
#include <omp.h>

void takeTimeStep(struct simEnv sim, float dt, float injected[], int rowNum, float data[10000][205])
{
    // loop through compartments
    // printf("%f,",sim.compartments[25]->v);

    for (int i = 0; i < 51; i++)
    {
        struct compartment *current = sim.compartments[i];

        // values for voltage at internal node need to be in reference to ground
        float vinLeft = (current->left) ? current->left->v + current->left->vext : current->v + current->vext;
        float vinRight = (current->right) ? current->right->v + current->right->vext : current->v + current->vext;

        // forward euler all vars to get the next point
        current->fev = current->v + dt * derivV(current->v, current->m, current->h, current->n, injected[i], current->vext + current->v, vinLeft, vinRight);
        //if(i==25) printf("%f,",current->fev);
        current->fen = current->n + dt * derivN(current->v, current->n);
        current->fem = current->m + dt * derivM(current->v, current->m);
        current->feh = current->h + dt * derivH(current->v, current->h);
        // if (i == 0)
        // {
        //     printf("%f,%f,%f,%f\n", current->fen, current->fem, current->feh, current->fev);
        // }
    }

    for (int i = 0; i < 51; i++)
    {
        struct compartment *current = sim.compartments[i];

        // values for voltage at internal node need to be in reference to ground
        float vinLeft = (current->left) ? current->left->fev + current->left->vext : current->fev + current->vext;
        float vinRight = (current->right) ? current->right->fev + current->right->vext : current->fev + current->vext;

        // do backward euler approximation (using the values approximated from forward euler)
        current->bev = current->v + dt * derivV(current->fev, current->fem, current->feh, current->fen, injected[i], current->vext + current->fev, vinLeft, vinRight);
        //if(i==25) printf("%f\n",current->bev);
        current->ben = current->n + dt * derivN(current->fev, current->fen);
        current->bem = current->m + dt * derivM(current->fev, current->fem);
        current->beh = current->h + dt * derivH(current->fev, current->feh);
        // if(i==0){
        //     printf("%f,%f,%f,%f\n",current->ben, current->bem, current->beh, current->bev);
        // }
    }

    data[rowNum][0] = rowNum*dt; 
    for (int i = 0; i < 51; i++)
    {
        struct compartment *current = sim.compartments[i];

        // set normal array to the backward euler array
        current->v = current->bev;
        data[rowNum][1+i] = current->v;
        current->n = current->ben;
        data[rowNum][52+i] = current->n;
        current->m = current->bem;
        data[rowNum][103 + i] = current->m;
        current->h = current->beh;
        data[rowNum][154 + i] = current->h;
    }
    for(int i=0; i<11; i++){
        data[rowNum][205+i] = sim.compartments[20+i]->vext;
    }
    //printf("V[50] = %f", sim.compartments[50]->v);
    // for(int i=0; i<15; i++){
    //     printf("%f,", sim.compartments[i]->v);
    // }
    // printf("\n");
    
}
// derivatives were calculated using a axon diameter of 10um and compartment length of 5.1/pi cm
//  this gives compartment surface area of .001cm^2
//  Ra between compartments is 48.7014 ohms
float derivV(float v, float m, float h, float n, float i, float vinCenter, float vinLeft, float vinRight)
{
    // calculates the voltage derivative given current state values
    // if(NUM==0){
    //     printf("%f,%f\n", .0012 * powf(m, 3.0) * h * (v - 55.17), 36 * powf(n, 4.0) * (v + 72.14));
    // }

    //dv/dt=1/cm*(i -          ina                 -       ik                    -     il            +            ileft            +        iright)
    return 1 * (i - 120.0F * powf(m, 3.0F) * h * (v - 55.17F) - 36.0F * powf(n, 4.0F) * (v + 72.14F) - .3F * (v + 49.42F) + (vinLeft - vinCenter) / 1.0F + (vinRight - vinCenter) / 1.0F);
}
float derivN(float v, float n)
{
    // calculate the derivative of n given state values
    //dn/dt=    alpha(v)                     * (1-n) -      beta(v)*n
    return (.01F * (v + 55.0F) / (1.0F - expf(-.1F * (v + 55.0F)))) * (1.0F - n) - .125F * expf(-.0125F * (v + 65.0F)) * n;
}
float derivM(float v, float m)
{
    // calculate the derivative of m given state values
    //dm/dt=    alpha(v)                    * (1-m) -      beta(v)*m
    return (.1F * (v + 40.0F) / (1.0F - expf(-.1F * (v + 40.0F)))) * (1.0F - m) - 4.0F * expf(-.0556F * (v + 65.0F)) * m;
}
float derivH(float v, float h)
{
    // calculate the derivative of h given state values
    //dh/dt=    alpha(v)             * (1-h) -      beta(v)*h
    return (.07F * expf(-.05F * (v + 65.0F))) * (1.0F - h) - 1.0F / (1.0F + expf(-.1F * (v + 35.0F))) * h;
}

void writeToFile(float data[10000][205], char* name)
{
    FILE* f = fopen(name, "w");
    for(int r = 0; r < 10000; r++){
        for(int c = 0; c<205; c++){
            //printf("(%d,%d)\n",r,c);
            fprintf(f, "%.4f,", data[r][c]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}