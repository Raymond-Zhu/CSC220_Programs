#include "knapsack.hpp"

int main()
{
    /* Initialize and declare the parameters */
    std::map<int, bool> bestSet;
    std::map<int, bool> include;
    std::vector<std::pair<int,int> > profitAndWeight;
    int numBest = 0;
    int maxProfit = 0;
    int maxWeight = 16;

    /* Reads the file and stores the data */
    profitAndWeight = readFile("data.txt");

    /* Solves the knapsack problem */
    knapsack(0,profitAndWeight,maxWeight,0,0,maxProfit,bestSet,numBest,include);

    std::cout << maxProfit << std::endl;
    for (std::map<int,bool>::const_iterator i = bestSet.begin(); i != bestSet.end(); ++i) {
        if(i->second) {
            std::cout << "Item " << i->first << " has been included" << std::endl;
        }
    }

    return 0;
}
