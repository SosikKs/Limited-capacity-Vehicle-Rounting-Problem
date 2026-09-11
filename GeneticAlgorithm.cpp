#include "Evaluator.hpp"
#include "GeneticAlgorithm.hpp"
#include "Individual.hpp"
#include <vector>
#include <cstdlib>
using namespace std;
GeneticAlgorithm::GeneticAlgorithm(int popSize_,double crossProb_,double mutProb_,int limitGen_,
                     int numGroups_,int numCustomers_, Evaluator& evaluator_)
        : popSize(popSize_),
          mutThreshold((int)(mutProb_ * (RAND_MAX + 1.0))),
          crossThreshold((int)(crossProb_ * (RAND_MAX + 1.0))),
          currentGeneration(0),
          newGeneration(1),
          limitGen(limitGen_)
    {
        gen[0].reserve(popSize_);
        gen[1].reserve(popSize_);

        for (int i = 0; i < popSize; ++i) {
            gen[0].emplace_back(numGroups_, numCustomers_, mutThreshold, evaluator_);
            gen[1].emplace_back(numGroups_, numCustomers_, mutThreshold, evaluator_);
        }
    }

void GeneticAlgorithm::checkIfBest(Individual& bestIndividualEver,int& generationNumber,int currentGenId,Individual& cand)
{
    if(bestIndividualEver.getFitness()>cand.calcFitness(currentGenId+1))
    {
        bestIndividualEver.copyFrom(cand);
        generationNumber=currentGenId+1;
    }
}

void GeneticAlgorithm::run(Individual& bestIndividualEver,int& generationNumber)
{

    bestIndividualEver.copyFrom(gen[currentGeneration][0]);
    bestIndividualEver.calcFitness(0);
    generationNumber = 0;

    for(int currentGenId=0; currentGenId<limitGen;++currentGenId)
    {
        if(currentGenId%10==0)
        for(int i=0; i< popSize/2;++i)
        {


            //selecting parents
            int ind1 = rand()%popSize;
            int ind2 = rand()%popSize;
            double fit1 = gen[currentGeneration][ind1].calcFitness(currentGenId);
            double fit2 = gen[currentGeneration][ind2].calcFitness(currentGenId);
            int parent1;
            parent1 = (fit1<fit2) ? ind1 : ind2;

            int ind3 = rand()%popSize;
            int ind4 = rand()%popSize;
            double fit3 = gen[currentGeneration][ind3].calcFitness(currentGenId);
            double fit4 = gen[currentGeneration][ind4].calcFitness(currentGenId);
            int parent2;
            parent2 = (fit3<fit4) ? ind3 : ind4;

            //children will be Individuals form the otherGen on ids i and i+popSize/2
            if(rand()<crossThreshold)
            {
                gen[currentGeneration][parent1].crossover(
                    gen[currentGeneration][parent2],
                    gen[newGeneration][i],
                    gen[newGeneration][i + popSize / 2]
                );

                gen[newGeneration][i].mutate(false,currentGenId);
                gen[newGeneration][i+popSize/2].mutate(false,currentGenId);
            }
            else
            {
                gen[newGeneration][i].copyFrom(gen[currentGeneration][parent1]);
                gen[newGeneration][i+popSize/2].copyFrom(gen[currentGeneration][parent2]);
                gen[newGeneration][i].mutate(true,currentGenId);
                gen[newGeneration][i+popSize/2].mutate(true,currentGenId);
            }

            checkIfBest(bestIndividualEver,generationNumber,currentGenId,gen[newGeneration][i]);
            checkIfBest(bestIndividualEver,generationNumber,currentGenId,gen[newGeneration][i+popSize/2]);




        }
        currentGeneration = newGeneration;
        newGeneration = 1-newGeneration;
    }
}
