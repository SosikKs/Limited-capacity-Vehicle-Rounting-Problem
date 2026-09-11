#pragma once
#include "Evaluator.hpp"
#include "Individual.hpp"
#include <vector>
#include <cstdlib>
using namespace std;
class GeneticAlgorithm
{
private:
    int popSize;
    int mutThreshold;
    int crossThreshold;

    vector<Individual> gen[2];
    int limitGen;
    int currentGeneration;
    int newGeneration;


public:
    GeneticAlgorithm(int popSize_,double crossProb_,double mutProb_,int limitGen_,
                     int numGroups_,int numCustomers_, Evaluator& evaluator_);
    void run(Individual& bestIndividualEver,int& generationNumber);
    void checkIfBest(Individual& bestIndividualEver,int& generationNumber,int currentGenId,Individual& cand);



};

