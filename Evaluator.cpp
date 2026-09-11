#include "Evaluator.hpp"
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;

Evaluator::Evaluator(int numOfGroups_)
    : numOfGroups(numOfGroups_),
      problemName(""),
      problemDimension(0),
      problemCapacity(0),
      problemDistance(0.0),
      problemEdgeWeightType(""),
      problemDepot(0)
      {
          currentNode.resize(numOfGroups_);
          currentLoad.resize(numOfGroups_);
      }

double Evaluator::evaluate(const vector<int>& solution)
{

    double fitness=0.0;

    fill(currentNode.begin(),currentNode.end(),problemDepot);
    fill(currentLoad.begin(),currentLoad.end(),problemCapacity);

    int customerNode;
    int customerId;
    int customerGroup;

    for(int i=0;i<problemDimension-1;i++)
    {

        customerNode = problemPermutation[i]-1;  //change indexing from 1-based to 0-based
        customerId=customerNode-1;  //change from global indexing (depot at 0) to customer (first customer at 0)
        customerGroup = solution[customerId];

        if(problemDemands[customerNode]<=currentLoad[customerGroup])
        {
            fitness+=dist(currentNode[customerGroup],customerNode);
        }
        else
        {
            fitness+=dist(currentNode[customerGroup],problemDepot)+dist(problemDepot,customerNode);
            currentLoad[customerGroup]=problemCapacity;
        }

        currentNode[customerGroup]=customerNode;
        currentLoad[customerGroup]-=problemDemands[customerNode];
    }
    for(int i=0;i<numOfGroups;i++)
    {
        fitness+=dist(currentNode[i],problemDepot);
    }
    return fitness;
}

int Evaluator::loadProblem(const string& folderName,const string& instanceName)
{
    string filePath =   "data/lcvrp/" + folderName + "/"+ instanceName + ".lcvrp";
    ifstream file(filePath);
    if (!file.is_open()){
        return EvaluatorErrors::FileCannotOpen;
    }

    string line;
    while (getline(file, line)) {
        // skip empty lines
        if (line.empty()) continue;

        // parse header fields
        if (line.find("NAME") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string name = line.substr(colon_pos + 1);
                // remove all kind of whitespaces
                size_t start = name.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    name = name.substr(start);
                    size_t end = name.find_last_not_of(" \t\r\n");
                    name = name.substr(0, end + 1);
                }
                problemName=name;
            }
        }
        else if (line.find("DIMENSION") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                int dimension = stoi(line.substr(colon_pos + 1));
                problemDimension = dimension;
            }
        }
        else if (line.find("CAPACITY") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                int capacity = stoi(line.substr(colon_pos + 1));
                problemCapacity=capacity;
            }
        }
        else if (line.find("DISTANCE") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                double distance = stod(line.substr(colon_pos + 1));
                problemDistance = distance;
            }
        }
        else if (line.find("EDGE_WEIGHT_TYPE") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                string type = line.substr(colon_pos + 1);
                size_t start = type.find_first_not_of(" \t\r\n");
                if (start != string::npos) {
                    type = type.substr(start);
                    size_t end = type.find_last_not_of(" \t\r\n");
                    type = type.substr(0, end + 1);
                }
                problemEdgeWeightType = type;
            }
        }
        else if (line.find("EDGE_WEIGHT_SECTION") != string::npos) {
            ParseEdgeWeightSection(file);
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            ParseNodeCoordSection(file);
        }
        else if (line.find("DEMAND_SECTION") != string::npos) {
            ParseDemandSection(file);
        }
        else if (line.find("DEPOT_SECTION") != string::npos) {
            ParseDepotSection(file);
        }
        else if (line.find("PERMUTATION") != string::npos) {
            size_t colon_pos = line.find(':');
            if (colon_pos != string::npos) {
                problemPermutation.clear();
                problemPermutation.reserve(problemDimension > 0 ? problemDimension - 1 : 0);
                istringstream iss(line.substr(colon_pos + 1));
                int customer_id;
                while (iss >> customer_id) {
                    problemPermutation.push_back(customer_id);
                }
                while ((int)problemPermutation.size() < problemDimension - 1 && (file >> customer_id)) {
                    problemPermutation.push_back(customer_id);
                }
            }
        }
        else if (line.find("EOF") != string::npos) {
            break;
        }
    }
    file.close();

    if (problemEdgeWeightsFlat.empty() && problemEdgeWeightType == "EUC_2D"){
        if ((int)problemCoordinates.size() != problemDimension) return EvaluatorErrors::WrongNumberOfCoordinates;

        BuildEdgeWeightsFromCoords();
    }

    if ((int)problemEdgeWeightsFlat.size() != problemDimension * problemDimension)
        return EvaluatorErrors::DistanceMatrixWronglyConstructed;

    return EvaluatorErrors::NoErrorCode;
}

void Evaluator::ParseEdgeWeightSection(std::ifstream& file){
    int dimension = problemDimension;

    // allocate full NxN matrix in one block
    problemEdgeWeightsFlat.assign(dimension * dimension, 0.0);

    // read lower triangle and mirror (symmetric)
    for (int i = 1; i < dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            double weight;
            file >> weight;
            problemEdgeWeightsFlat[i * dimension + j] = weight;
            problemEdgeWeightsFlat[j * dimension + i] = weight;
        }
    }
}


void Evaluator::ParseNodeCoordSection(ifstream& file){
    int dimension = problemDimension;
    problemCoordinates.assign(dimension, Coordinate());

    for (int i = 0; i < dimension; ++i){
        int node_id;
        double x, y;
        file >> node_id >> x >> y;
        problemCoordinates[node_id-1] = Coordinate(x, y);
    }
}

void Evaluator::ParseDemandSection(ifstream& file){
    int dimension = problemDimension;
    problemDemands.assign(dimension, 0);
    for (int i = 0; i < dimension; ++i) {
        int node_id, demand;
        file >> node_id >> demand;
        problemDemands[node_id - 1] = demand;
    }
}

void Evaluator::ParseDepotSection(ifstream& file){
    int depot;
    file >> depot;
    problemDepot=depot-1;//change to 0-based

    int terminator;
    file >> terminator;
}

void Evaluator::BuildEdgeWeightsFromCoords()
{
    int n = problemDimension;
    problemEdgeWeightsFlat.assign(n * n, 0.0);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            const double dx = problemCoordinates[i].x - problemCoordinates[j].x;
            const double dy = problemCoordinates[i].y - problemCoordinates[j].y;
            const double d = sqrt(dx * dx + dy * dy);
            problemEdgeWeightsFlat[i * n + j] = d;
            problemEdgeWeightsFlat[j * n + i] = d;
        }
    }
}
