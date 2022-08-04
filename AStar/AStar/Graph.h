#pragma once

#include<string>
#include<unordered_map>

#include "Node.h"

//Added a graph class to encapsulate a map of nodes

using namespace std;

struct Coordinate
{
	string m_fromNodeName;
	string m_toNodeName;
	int m_cost;
	//Added a directional boolean to add support for adding directional connections between nodes
	bool m_isDirectional;

	Coordinate(string fromNodeName, string toNodeName, int cost, bool isDirectional = false)
		:m_fromNodeName(fromNodeName),
		m_toNodeName(toNodeName),
		m_cost(cost),
		m_isDirectional(isDirectional)
	{

	}
};

class Graph
{
	unordered_map<string, Node*>* m_pNodeMap;

public:
	Graph()
		:m_pNodeMap(new unordered_map<string, Node*>())
	{
		
	}

	Graph(Coordinate coordinates[], int sizeOfArray)
		:m_pNodeMap(new unordered_map<string, Node*>())
	{
		AddNodes(coordinates, sizeOfArray);
	}

	~Graph()
	{
		for (auto node : *m_pNodeMap)
		{
			delete node.second;
			node.second = nullptr;
		}

		delete m_pNodeMap;
		m_pNodeMap = nullptr;
	}

	void AddNode(Coordinate coordinate)
	{
		//if fromNode does not exist in graph
		if (!m_pNodeMap->count(coordinate.m_fromNodeName))
		{
			Node* fromNode = new Node(coordinate.m_fromNodeName);
			(*m_pNodeMap)[coordinate.m_fromNodeName] = fromNode;
		}

		//if toNode does not exist
		if (!m_pNodeMap->count(coordinate.m_toNodeName))
		{
			Node* toNode = new Node(coordinate.m_toNodeName);
			(*m_pNodeMap)[coordinate.m_toNodeName] = toNode;
		}

		//Add neigbors to nodes based on coordinate data
		if (coordinate.m_isDirectional)
		{
			(*m_pNodeMap)[coordinate.m_fromNodeName]->AddDirectionalNeighbor((*m_pNodeMap)[coordinate.m_toNodeName], coordinate.m_cost);
		}
		else
		{
			(*m_pNodeMap)[coordinate.m_fromNodeName]->AddNeighbor((*m_pNodeMap)[coordinate.m_toNodeName], coordinate.m_cost);
		}
	}

	void AddNodes(Coordinate coordinates[], int sizeOfArray)
	{
		for (int i = 0; i < sizeOfArray; i++)
		{
			AddNode(coordinates[i]);
		}
	}

	Node* GetNode(string nodeName)
	{
		if (m_pNodeMap->count(nodeName) != 1)
		{
			return nullptr;
		}

		return  (*m_pNodeMap)[nodeName];
	}

	Node* operator[](string nodeName)
	{
		return GetNode(nodeName);
	}

	void PrintChildren()
	{
		for (auto const& entry : *m_pNodeMap)
		{
			entry.second->PrintChildren();
		}

		cout << endl;
	}
};

