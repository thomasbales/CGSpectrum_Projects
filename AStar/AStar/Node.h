#pragma once

#include<string>
#include<unordered_map>

using namespace std;

class Node
{
	class Edge
	{
	public:
		Node* m_node;
		int m_cost;

		Edge()
			:m_node(nullptr),
			m_cost(0)
		{

		}

		Edge(Node* node, int cost)
			:m_node(node),
			m_cost(cost)
		{

		}

		~Edge()
		{
			m_node = nullptr;
		}
	};

public:
	string m_name;
	//changed from list to unordered map to allow for easier access of edges
	//renamed from m_pChildren to m_pNeighbors since it better describes node relationships
		//in a graph.
	unordered_map<string, Edge>* m_pNeighbors;

	Node(string name);

	~Node();

	//Renamed AddConnection to AddNeighbor for better readability
	void AddNeighbor(Node* neighborNode, int cost);

	//Renamed AddChildNode to AddDirectionalNeigbor for better readability
	void AddDirectionalNeighbor(Node* toNode, int cost);

	void PrintChildren();
};

