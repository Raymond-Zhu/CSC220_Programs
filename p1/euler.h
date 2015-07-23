#pragma once

#include <iostream>
#include <string>
#include <set>
#include <map>

class Graph {
    public:
        Graph();
        Graph(std::string file);
        bool isGraphConnected();
        bool isDegreeEven();
        void printEulerCircuit();
    private:
        std::map<int, std::set<int> > adj;
};


