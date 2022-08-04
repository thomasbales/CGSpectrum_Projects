#include "Node.h"

#include<string>
#include<unordered_map>
#include<assert.h>
#include<iostream>

using namespace std;

Node::Node(string name)
	:m_name(name),
	m_pNeighbors(new unordered_map<string, Edge>())
{

}

Node::~Node()
{
	delete m_pNeighbors;
	m_pNeighbors = nullptr;
}

//Renamed AddConnection to AddNeighbor for better readability
void Node::AddNeighbor(Node* neighborNode, int cost)
{
	AddDirectionalNeighbor(neighborNode, cost);
	neighborNode->AddDirectionalNeighbor(this, cost);
}

//Renamed AddChildNode to AddDirectionalNeigbor for better readability
void Node::AddDirectionalNeighbor(Node* toNode, int cost)
{
	assert(m_pNeighbors->count(toNode->m_name) != 1);

	Edge edge = Edge(toNode, cost);
	(*m_pNeighbors)[toNode->m_name] = edge;
}

void Node::PrintChildren()
{
	cout << "Node " << m_name << "'s neighbor nodes are:" << endl;
	for (auto const& entry : *m_pNeighbors)
	{
		cout << entry.second.m_node->m_name << ", " << entry.second.m_cost << endl;
	}
}