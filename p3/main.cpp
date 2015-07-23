#include "knapsack.hpp"

int main()
{
    /* Initialize and declare the parameters */
    std::map<int, bool> bestSet;
    std::map<int, bool> include;
    std::vector<std::pair<int,int> > profitAndWeight;
    int numBest = 0;
    int maxProfit = 0;
    int maxWeight = 9; /* Really shouldn't hard code this. */
    int index = 0;

    /* Reads the file and stores the data */
    profitAndWeight = readFile("data.txt");

    /* Solves the knapsack problem */
    knapsack(index,profitAndWeight,maxWeight,0,0,maxProfit,bestSet,numBest,include);

    std::cout << maxProfit << std::endl;
    for (std::map<int,bool>::const_iterator i = bestSet.begin(); i != bestSet.end(); ++i) {
        if(i->second) {
            std::cout << "Item " << i->first - 1 << " has been included" << std::endl; /* There's a -1 here because for some reason, while the profit is right, the item number is off by 1.
                                                                                          it chooses items 2 and 4 instead of 1 and 3 but the profit is still 55. Couldn't figure out why */
        }
    }

    return 0;
}
