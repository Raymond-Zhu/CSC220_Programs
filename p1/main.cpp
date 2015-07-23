#include <iostream>
#include "euler.h"

int main() {
    Graph g = Graph("graph1.txt");
    g.printEulerCircuit();
}
