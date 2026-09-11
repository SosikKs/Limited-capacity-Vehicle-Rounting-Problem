#include "Evaluator.hpp"
#include "Individual.hpp"
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;
Individual::Individual(int numOfGroups_, int numOfCustomers_,int mutThreshold_,Evaluator& evaluator_)
    : numOfGroups(numOfGroups_),
      numOfCustomers(numOfCustomers_),
      genotype(numOfCustomers_,0),
      fitness(-1.0),
      whenWasFitnessCorrect(-1),
      evaluator(evaluator_),
      mutThreshold(mutThreshold_)
      {
        for(int i=0;i<numOfCustomers_;i++)
        {
            genotype[i]=rand()%numOfGroups_;
        }
      }

void Individual::mutate(bool isAParent,int currentGen)
{
    bool wasMutated=false;
    for(int i=0;i<numOfCustomers;i++)
    {
        if(rand()<mutThreshold)
        {
            genotype[i]=rand()%numOfGroups;
            wasMutated=true;
        }
    }
    if(!wasMutated && isAParent)
    {
        //if wasn't mutated, and is a copy of its parent, its fitness stays the same
        // so we can flag it as calculated in the next Gen
        whenWasFitnessCorrect=currentGen+1;
    }

}
void Individual::crossover(const Individual& other, Individual& child1, Individual& child2)
{
    int cut = rand() % (numOfCustomers - 1) + 1;

    for (int i = 0; i < cut; ++i) {
        child1.genotype[i] = genotype[i];
        child2.genotype[i] = other.genotype[i];
    }
    for (int i = cut; i < numOfCustomers; ++i) {
        child1.genotype[i] = other.genotype[i];
        child2.genotype[i] = genotype[i];
    }
}
double Individual::calcFitness(int currentGenId)
{


    if(currentGenId <=whenWasFitnessCorrect)
        return fitness;
    else
    {

        whenWasFitnessCorrect=currentGenId;
        //use Evaluator, calculate new Fitness, save it and return it
        fitness = evaluator.evaluate(genotype);
        return fitness;
    }
}

void Individual::copyFrom(const Individual& other){
    genotype = other.genotype;
    fitness = other.fitness;
    whenWasFitnessCorrect = other.whenWasFitnessCorrect;
}

string Individual::writeGenotype()
{
    string out;
    out.reserve(genotype.size() * 3); // rough estimate, avoids reallocations

    for (size_t i = 0; i < genotype.size(); ++i) {
        out += std::to_string(genotype[i]);
        if (i + 1 < genotype.size())
            out += ' ';
    }
    return out;
}
