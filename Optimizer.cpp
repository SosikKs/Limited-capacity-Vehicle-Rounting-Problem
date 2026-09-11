#include <math.h>
#include "Optimizer.hpp"
#include "Evaluator.hpp"
#include "GeneticAlgorithm.hpp"
#include "Individual.hpp"
using namespace std;
Optimizer::Optimizer(Evaluator& evaluator_)
    : evaluator(evaluator_),
      genOfBestIndividualEver(0),
      bestIndividualEver(evaluator_.getNumOfGroups(),evaluator_.getNumberOfCustomers(),(int)(MutProb * (RAND_MAX + 1.0)),evaluator_),
      geneticAlgorithm(PopSize,CrossProb,MutProb,LimitGen,evaluator_.getNumOfGroups(),evaluator_.getNumberOfCustomers(),evaluator_){}

void Optimizer::start()
{
    geneticAlgorithm.run(bestIndividualEver,genOfBestIndividualEver);
}
