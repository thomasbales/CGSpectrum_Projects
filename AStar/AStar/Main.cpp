#include<iostream>
#include<list>

#include"Graph.h"
#include"AStar.h"


using namespace std;

int main()
{
    Coordinate coordinates[5] = {
        Coordinate("a", "b", 5),
        Coordinate("b", "c", 1),
        Coordinate("a", "d", 7),
        Coordinate("c", "d", 1),
        Coordinate("a", "e", 9)
    };

    Graph* pGraph = new Graph(coordinates, 5);
    pGraph->PrintChildren();
    cout << endl;
    
    list<string>* pPath = AStar::GetShortestPath(pGraph, "a", "d");
    cout << endl;

    AStar::PrintPath(*pPath, pGraph);
    cout << endl << endl;

    delete pGraph;
    pGraph = nullptr;
    delete pPath;
    pPath = nullptr;

    return 0;
}

