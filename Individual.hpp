#pragma once
#include "Evaluator.hpp"
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>
using namespace std;
class Individual
{
private:
    Evaluator& evaluator;
    int numOfGroups;
    int numOfCustomers;
    vector<int> genotype;
    double fitness;
    int whenWasFitnessCorrect;
    int mutThreshold;

public:
    Individual(int numOfGroups_, int numOfCustomers_,int mutThreshold_,Evaluator& evaluator_);
    void mutate(bool isAParent,int currentGen);
    void crossover(const Individual& other, Individual& child1, Individual& child2);
    double calcFitness(int currentGenId);
    double getFitness()const {return fitness;}
    string writeGenotype();
    void setFitness(double newFitness){fitness=newFitness;}
    void copyFrom(const Individual& other);

};
