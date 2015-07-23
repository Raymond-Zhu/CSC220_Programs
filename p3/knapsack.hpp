#pragma once

#include<iostream>
#include<fstream>
#include<vector>
#include<utility>
#include<string>
#include<algorithm>
#include<map>


/* Comparison object used for sorting the items by profit/weight */
struct Compare {
    bool operator() (const std::pair<int,int>& a, const std::pair<int,int>& b) {
        return (a.first/a.second) < (b.first/b.second);
    }
};

/* Reads the file and stores the data. */
std::vector<std::pair<int,int> > readFile(std::string fileName) {
    std::ifstream file(fileName.c_str());
    int m,n;

    /* Vector of pairs. First is profit, second is weight */
    std::vector<std::pair<int, int> > profitAndWeight;
    if(file.is_open()) {
        while(file >> m >> n ) {
            std::pair<int,int> item(m,n);
            profitAndWeight.push_back(item);
        }
    }
    Compare comp;
    std::sort(profitAndWeight.begin(), profitAndWeight.end(), comp);
    return profitAndWeight;
}

/* Promising function. Most of the algorithm was taking from the book. I just chose which data structures and types to use */
bool promising(int i,const std::vector<std::pair<int,int> >& data, const int& weight,const int& maxWeight,const int& profit, const int& maxProfit) {
    int j,k;
    int totalWeight;
    double bound;

    if(weight >= maxWeight) {
        return false;
    } else {
        j = i + 1;
        bound = profit;
        totalWeight = weight;
        while(j <= data.size() && totalWeight + data[j].second <= maxWeight) {
            totalWeight += data[j].second;
            bound += data[j].first;
            j++;
        }
        k = j;
        if(k <= data.size()) {
            bound = bound + (maxWeight - totalWeight) * (data[k].first/data[k].second);
        }
        return bound > maxProfit;
    }


}

/* Taken from book. Definitely could've made it cleaner if I put this into a class so I don't have to pass so many parameters. */
void knapsack(const int& index, const std::vector<std::pair<int,int> >& data, const int& maxWeight, const int& profit, const int& weight, int& maxProfit, std::map<int,bool>& bestSet, int& numBest, std::map<int,bool>& include) {
    if (weight <= maxWeight && profit > maxProfit ) {
        maxProfit = profit;
        numBest = index;
        bestSet = include;
    }

    if(promising(index,data,weight,maxWeight,profit,maxProfit)) {
        include[index+1] = true;
        knapsack(index+1,data,maxWeight,profit+data[index+1].first,weight+data[index+1].second, maxProfit, bestSet, numBest, include);
        include[index+1] = false;
        knapsack(index+1,data,maxWeight,profit,weight,maxProfit,bestSet,numBest,include);
    }
}





