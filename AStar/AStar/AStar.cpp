#include <iostream>
#include <list>

#include "Node.h"


using namespace std;

void AStar(Node* startNode, Node* endNode);
bool LinearSearch(list<Node*>* container, Node* target);
Node* GetLowestCostNode(list<Node*>* container);

int main()
{
    Coordinate coordinates[4] = {
        Coordinate("a", "b", 5),
        Coordinate("b", "c", 1),
        Coordinate("a", "d", 7),
        Coordinate("c", "d", 1)
    };

    Node nodeA = Node("a");
    Node nodeB = Node("b");
    Node nodeC = Node("c");
    Node nodeD = Node("d");

    nodeA.ConnectNode(&nodeB, 5);
    nodeA.ConnectNode(&nodeD, 7);
    nodeB.ConnectNode(&nodeC, 1);
    nodeC.ConnectNode(&nodeD, 1);

    nodeA.PrintChildren();
    nodeB.PrintChildren();
    nodeC.PrintChildren();
    nodeD.PrintChildren();

    AStar(&nodeB, &nodeD);
}

void AStar(Node* startNode, Node* endNode)
{
    list<Node*> openNodes;
    list<Node*> closedNodes;

    openNodes.push_back(startNode);
    Node* currentNode = nullptr;

    while (openNodes.size() > 0)
    {
        currentNode = GetLowestCostNode(&openNodes);

        if (currentNode->m_name == endNode->m_name)
        {
            //end search. Found node!
            cout << endl << "Found target! Node " << startNode->m_name 
                << " to " << currentNode->m_name << " has a cost of " 
                << currentNode->TotalCost << endl;

            Node* pathNode = currentNode;
            list<string> pathNodes;
            while (pathNode != nullptr)
            {
                pathNodes.push_back(pathNode->m_name);
                pathNode = pathNode->Parent;
            }
            while (pathNodes.size() > 1)
            {
                cout << pathNodes.back() << " ->  ";
                pathNodes.pop_back();
            }

            cout << pathNodes.back() << endl << endl;

            break;
        }

        for (auto const& edge : *currentNode->m_pChildren)
        {
            cout << "Scanning edge from " << currentNode->m_name 
                << " to " << edge.m_node->m_name << "..." << endl;

            int childTotalCost = edge.m_cost + currentNode->TotalCost;

            if (LinearSearch(&openNodes, edge.m_node))
            {
                if (childTotalCost >=  edge.m_node->TotalCost)
                    continue;
            }
            else if (LinearSearch(&closedNodes, edge.m_node))
            {
                if (childTotalCost >= edge.m_node->TotalCost)
                    continue;

                openNodes.push_back(edge.m_node);
                closedNodes.remove(edge.m_node);
            }
            else
            {
                openNodes.push_back(edge.m_node);
            }

            edge.m_node->TotalCost = childTotalCost;
            edge.m_node->Parent = currentNode;
        }

        closedNodes.push_back(currentNode);
    }

    if (currentNode != endNode)
    {
        cout << endl << "Could not find target node!" << endl;
    }
}

bool LinearSearch(list<Node*>* container, Node* target)
{
    for (auto const& item : *container)
    {
        if (item == target)
        {
            return true;
        }
    }

    return false;
}

Node* GetLowestCostNode(list<Node*>* container)
{
    Node* lowestCostNode = container->front();

    for (auto const& item : *container)
    {
        if (item->TotalCost < lowestCostNode->TotalCost)
        {
            lowestCostNode = item;
        }
    }

    container->remove(lowestCostNode);

    return lowestCostNode;
}

