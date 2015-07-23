#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<string>

std::vector<std::pair<int,int> > readFile(std::string fileName) {
    std::ifstream file(fileName.c_str());
    int m,n;

    /* Vector of pairs. First is profit, second is weight */
    std::vector<std::pair<int, int> > profitAndWeight;
    if(file.is_open()) {
        while(file >> m >> n ) {
            std::pair<int,int> item(m,n);
            profitAndWeight.push_back(itej);
        }
    }
    return profitAndWeight;
}

void knapsack(int index, )
