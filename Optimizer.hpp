#pragma once
#include "Evaluator.hpp"
#include "GeneticAlgorithm.hpp"
#include "Individual.hpp"
#include <string>
#include <vector>
using namespace std;
class Optimizer
{
private:
    const int PopSize = 1000;
    const double CrossProb = 0.6;
    const double MutProb = 0.1;
    const int LimitGen = 10000;

    Evaluator& evaluator;
    int correctness;
    Individual bestIndividualEver;
    int genOfBestIndividualEver;
    GeneticAlgorithm geneticAlgorithm;
public:
    Optimizer(Evaluator& evaluator_);
    void start();

    Individual getBestIndividual(){return bestIndividualEver;}
    double getBestFitness(){return bestIndividualEver.getFitness();}
    int getGenOfBestIndividualEver(){return genOfBestIndividualEver;}

};
