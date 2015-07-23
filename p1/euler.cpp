#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include "euler.h"

Graph::Graph() {};

/* Opens the file and fills the adjacency list */
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

/* Checks if degree is even */
bool Graph::isDegreeEven() {
    for (std::map<int, std::set<int> >::const_iterator i = adj.begin(); i != adj.end(); ++i) {
        if(i->second.size() % 2 != 0)
            return false;
    }
    return true;
}

/* Do a bfs to see if graph is connected. This is for just in case */
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

/* Does a DFS-like search on the graph. */
void findEulerCircuit(std::map<int, std::set<int> >& copyOfGraph, std::stack<int>& circuit, int source) {

    /* Goes through the neighbors of the current vertex and erase the edge between them */
    for (std::set<int>::const_iterator i = copyOfGraph[source].begin(); i != copyOfGraph[source].end(); ++i) {
        copyOfGraph[source].erase(*i);
        copyOfGraph[*i].erase(source);

        /* Recursively perform the search */
        findEulerCircuit(copyOfGraph,circuit,*i);
    }
    /* When the vertex has no more neighbors, we can push it onto the stack since we're done with that vertex */
    circuit.push(source);

}

/* Recursively finds Euler Circuit */
void Graph::printEulerCircuit() {
    if(!isGraphConnected() || !isDegreeEven()) {
        std::cout << "Euler circuit does not exist." << std::endl;
    } else {

        /* Stack that keeps track of the circuit */
        std::stack<int> circuit;

        /* Create a copy of the adjacency list in case we need it later */
        std::map<int, std::set<int> > copyOfGraph = adj;

        /* Calls the recursive function */
        findEulerCircuit(copyOfGraph,circuit,0);

        /* Prints the path */
        while(!circuit.empty()) {
            std::cout << circuit.top() << std::endl;
            circuit.pop();
        }

    }
}

