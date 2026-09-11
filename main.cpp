#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Optimizer.hpp"
#include "Evaluator.hpp"
using namespace std;

int main()
{
    srand (time(NULL));

    int numOfGroups = 16;

    Evaluator evaluator(numOfGroups);
    int correctness = evaluator.loadProblem("Vrp-Set-X", "X-n209-k16");
    if(correctness!=EvaluatorErrors::NoErrorCode)
    {
        if(correctness==EvaluatorErrors::FileCannotOpen)
        {
            cout<< "Error code 1: File Cannot Open";
        }
        else if (correctness == EvaluatorErrors::WrongNumberOfCoordinates)
        {
            cout<< "Error code 2: Wrong Number Of Coordinates";
        }
        else
        {
            cout<< "Error code 3: Distance Matrix Wrongly Constructed";
        }
        return 0;
    }

    Optimizer optimizer = Optimizer(evaluator);
    optimizer.start();



    cout<<"Simulations Succesfull:\n Best Individuals Generation = "<<optimizer.getGenOfBestIndividualEver()
    <<"\nBest Individuals fitness = "<<optimizer.getBestFitness()
    <<"\nBest Individuals genotype: \n"<<optimizer.getBestIndividual().writeGenotype();


}
