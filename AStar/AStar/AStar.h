#pragma once

#include<string>
#include<assert.h>
#include<unordered_set>
#include<chrono>
#include<thread>
#include<list>
#include<iostream>

#include"Graph.h"
#include"Node.h"
#include"PriorityMap.h"

using namespace std;

struct Waypoint
{
    Waypoint* m_pParentWaypoint;
    Node* m_pNode;
    int m_totalCost;
    string m_nodeName;

    Waypoint(Node* node, string nodeName)
        :m_pParentWaypoint(nullptr),
        m_pNode(node),
        m_totalCost(0),
        m_nodeName(nodeName)
    {

    }

    bool operator>(Waypoint otherWaypoint)
    {
        if (m_totalCost > otherWaypoint.m_totalCost)
        {
            return true;
        }

        return false;
    }

    bool operator<(Waypoint otherWaypoint)
    {
        if (m_totalCost < otherWaypoint.m_totalCost)
        {
            return true;
        }

        return false;
    }

    //TODO: overload other operators
};

class AStar
{
    static list<string>* ConstructPath(Waypoint* finalWaypoint)
    {
        Waypoint* pCurrentWaypoint = finalWaypoint;
        list<string>* pPath = new list<string>();

        while (pCurrentWaypoint->m_pParentWaypoint != nullptr)
        {
            pPath->push_front(pCurrentWaypoint->m_pNode->m_name);
            pCurrentWaypoint = pCurrentWaypoint->m_pParentWaypoint;
        }

        pPath->push_front(pCurrentWaypoint->m_pNode->m_name);

        return pPath;
    }

public:
	//TODO: this should return a graph object (as a path) to the caller
	static list<string>* GetShortestPath(Graph* pGraph, string startNodeName, string endNodeName)
	{
        bool didFindTargetNode = false;
		Node* startNode = (*pGraph)[startNodeName];
		Node* endNode = (*pGraph)[endNodeName];
        Waypoint* startWaypoint = new Waypoint(startNode, startNode->m_name);

		assert(startNode != nullptr);
		assert(endNode != nullptr);

		unordered_map<string, Waypoint*> closedWaypoints;
		PriorityMap<string, Waypoint*> openWaypoints;

		//TODO: implement A* algorythm
        openWaypoints.Push(startWaypoint->m_nodeName, startWaypoint);
        startWaypoint = nullptr;
        Waypoint* currentWaypoint = nullptr;

        while (openWaypoints.Size() > 0)
        {
            currentWaypoint = openWaypoints.Pop();

            if (currentWaypoint->m_nodeName == endNode->m_name)
            {
                //end search. Found node!
                cout << "Found node!" << endl;
                didFindTargetNode = true;
                break;
            }

            for (auto const& edge : *currentWaypoint->m_pNode->m_pNeighbors)
            {
                cout << "Scanning edge from " << currentWaypoint->m_nodeName
                    << " to " << edge.second.m_node->m_name << "..." << endl;

                Waypoint* neighborWaypoint = new Waypoint(edge.second.m_node, edge.second.m_node->m_name);
                int neighborTotalCost = edge.second.m_cost + currentWaypoint->m_totalCost;
                neighborWaypoint->m_totalCost = neighborTotalCost;
                neighborWaypoint->m_pParentWaypoint = currentWaypoint;

                if (openWaypoints.Contains(neighborWaypoint->m_nodeName))
                {
                    if (neighborTotalCost >= neighborWaypoint->m_totalCost)
                    {
                        delete neighborWaypoint;
                        neighborWaypoint = nullptr;
                        continue;
                    }
                        
                }
                else if (closedWaypoints.count(neighborWaypoint->m_nodeName) != 0)
                {
                    if (neighborTotalCost >= neighborWaypoint->m_totalCost)
                    {
                        delete neighborWaypoint;
                        neighborWaypoint = nullptr;
                        continue;
                    }

                    openWaypoints.Push(neighborWaypoint->m_nodeName, neighborWaypoint);
                    closedWaypoints.erase(neighborWaypoint->m_nodeName);
                }
                else
                {
                    openWaypoints.Push(neighborWaypoint->m_nodeName, neighborWaypoint);
                }
            }

            closedWaypoints[currentWaypoint->m_nodeName] = currentWaypoint;
        }

        if (!didFindTargetNode)
        {
            cout << endl << "Could not find target node!" << endl;
            return nullptr;
        }

        list<string>* pPath = ConstructPath(currentWaypoint);

        delete currentWaypoint;
        currentWaypoint = nullptr;

        for (auto waypoint : closedWaypoints)
        {
            delete waypoint.second;
            waypoint.second = nullptr;
        }

        closedWaypoints.clear();

        while (openWaypoints.Size() > 0)
        {
            Waypoint* waypoint = openWaypoints.Pop();
            delete waypoint;
            waypoint = nullptr;
        }

        return pPath;
	}

    static void PrintPath(list<string> path, Graph* pGraph)
    {
        int i = 0;
        int totalCost = 0;
        while (path.size() > 1)
        {
            string nodeName = path.front();
            cout << nodeName << " -> ";
            path.pop_front();

            totalCost += pGraph->GetNode(nodeName)->m_pNeighbors->at(path.front()).m_cost;
            
        }

        cout << path.front() << endl;
        cout << "Total cost: " << totalCost << endl << endl;
    }
};
