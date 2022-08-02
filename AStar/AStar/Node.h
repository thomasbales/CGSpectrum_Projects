#pragma once

#include <string>
#include <list>

using namespace std;

class Node
{
public:
	class Edge
	{
	public:
		Node* m_node;
		int m_cost;

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

	string m_name;
	list<Edge>* m_pChildren;

	int TotalCost = 0;
	Node* Parent = nullptr;

	Node(string name)
		:m_name(name),
		m_pChildren(new list<Edge>())
	{

	}

	~Node()
	{
		delete m_pChildren;
		m_pChildren = nullptr;
	}



	//Both this node and toNode will have each other as children
	void ConnectNode(Node* toNode, int cost)
	{
		Edge edge = Edge(toNode, cost);
		m_pChildren->push_back(edge);

		toNode->AddChildNode(this, cost);
	}

	//Only this node will have toNode as a child
	void AddChildNode(Node* toNode, int cost)
	{
		Edge edge = Edge(toNode, cost);
		m_pChildren->push_back(edge);
	}

	void PrintChildren()
	{
		cout << "Node " << m_name << "'s child nodes are:" << endl;
		for (auto const& edge : *m_pChildren) 
		{
			cout << edge.m_node->m_name << "; " << edge.m_cost << endl;
		}

		cout << endl;
	}
};

struct Coordinate
{
	string m_fromNode;
	string m_toNode;
	int m_cost;

	Coordinate(string fromNode, string toNode, int cost)
		:m_fromNode(fromNode),
		m_toNode(toNode),
		m_cost(cost)
	{

	}
};



