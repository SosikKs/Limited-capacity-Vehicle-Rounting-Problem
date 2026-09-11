#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace EvaluatorErrors {
    constexpr int NoErrorCode = 0;
    constexpr int FileCannotOpen = 1;
    constexpr int WrongNumberOfCoordinates = 2;
    constexpr int DistanceMatrixWronglyConstructed = 3;
}

using namespace std;

struct Coordinate {
    double x;
    double y;

    Coordinate() : x(0.0), y(0.0){}
    Coordinate(double x, double y) : x(x), y(y){}
};

class Evaluator
{
private:

    int numOfGroups;

    string problemName;
    int problemDimension; // total number of nodes (customers + depot)
    int problemCapacity; // singular vehicle capacity
    double problemDistance; // maximum distance constraint (optional)
    string problemEdgeWeightType; // EUC_2D, EXPLICIT
    int problemDepot; // depot node id already changed to 0-based (usually 0)

    vector<Coordinate> problemCoordinates;
    vector<int> problemDemands; // customer demands (indexed from 0, but node 1 is at index 0)
    vector<int> problemPermutation; // fixed permutation for LcVRP (customer indexes 1..n-1)
    vector<double> problemEdgeWeightsFlat; // more optimal single vector

    vector<int> currentNode;
    vector<int> currentLoad;

    double dist(int i, int j) const {
        return problemEdgeWeightsFlat[i * problemDimension + j];
    }

    void ParseEdgeWeightSection(ifstream& file);
    void ParseNodeCoordSection(ifstream& file);
    void ParseDemandSection(ifstream& file);
    void ParseDepotSection(ifstream& file);


    void BuildEdgeWeightsFromCoords();

public:
    Evaluator(int numOfGroups_);
    double evaluate(const vector<int>& solution);
    int loadProblem(const string& folder_name,const string& instance_name);
    int getNumberOfCustomers()const {return problemDimension-1;}

    int getNumOfGroups()const {return numOfGroups;}
};
