#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include "euler.h"

Graph::Graph() {};

Graph::Graph(std::string file) {
    std::ifstream f(file.c_str());
    int n,m;
    if(f.is_open()) {
        while(f >> m >> n) {
            adj[m].insert(n);
            adj[n].insert(m);
        }
    } else {
        std::cout << "File not found" << std::endl;
    }
}

bool Graph::isDegreeEven() {
    for (std::map<int, std::set<int> >::const_iterator i = adj.begin(); i != adj.end(); ++i) {
        if(i->second.size() % 2 != 0)
            return false;
    }
    return true;
}

bool Graph::isGraphConnected() {
    std::map<int, bool> discovered;
    for (std::map<int,std::set<int> >::const_iterator i = adj.begin(); i != adj.end(); ++i) {
        discovered[i->first] = false;
    }
    std::queue<int> q;
    q.push(0);
    while(!q.empty()) {
       int v = q.front();
       q.pop();
       for (std::set<int>::const_iterator i = adj[v].begin(); i != adj[v].end(); ++i) {
            if(!discovered[*i]) {
                discovered[*i] = true;
                q.push(*i);
            }
       }
    }

    for (std::map<int,bool>::const_iterator i = discovered.begin(); i != discovered.end(); ++i) {
        if(!i->second)
            return false;
    }
    return true;
}

void findEulerCircuit(std::map<int, std::set<int> >& copyOfGraph, std::stack<int>& circuit, int source) {
    for (std::set<int>::const_iterator i = copyOfGraph[source].begin(); i != copyOfGraph[source].end(); ++i) {
        copyOfGraph[source].erase(*i);
        copyOfGraph[*i].erase(source);
        findEulerCircuit(copyOfGraph,circuit,*i);
    }
    circuit.push(source);

}

void Graph::printEulerCircuit() {
    if(!isGraphConnected() || !isDegreeEven()) {
        std::cout << "Euler circuit does not exist." << std::endl;
    } else {
        std::stack<int> circuit;
        std::map<int, std::set<int> > copyOfGraph = adj;
        findEulerCircuit(copyOfGraph,circuit,0);
        while(!circuit.empty()) {
            std::cout << circuit.top() << std::endl;
            circuit.pop();
        }

    }
}

