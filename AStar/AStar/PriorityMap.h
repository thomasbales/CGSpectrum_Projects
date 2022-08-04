#pragma once

#include<queue>
#include<unordered_set>
#include<string>
#include<vector>
#include <type_traits>

//Added PrioritySet class to have a container that combines the best about a set and a priority 
	//queue. This will make reading the list of open nodes in A* more performant.

using namespace std;

template<class Key, class Value>
class InternalValue
{
public:
	pair<Key, Value> m_pair;

	InternalValue(Key key, Value value)
		:m_pair(pair<Key, Value>(key, value))
	{

	}
};

template<class Key, class Value>
bool operator>(InternalValue<Key, Value> thisValue, InternalValue<Key, Value> otherValue)
{
	if (is_pointer<Value>::value)
	{
		if (*(thisValue.m_pair.second) > *(otherValue.m_pair.second))
		{
			return true;
		}

		return false;
	}

	if (thisValue.m_pair.second > otherValue.m_pair.second)
	{
		return true;
	}

	return false;
}

template<class Key, class Value>
bool operator<(InternalValue<Key, Value> thisValue, InternalValue<Key, Value> otherValue)
{
	if (is_pointer<Value>::value)
	{
		if (*(thisValue.m_pair.second) < *(otherValue.m_pair.second))
		{
			return true;
		}

		return false;
	}

	if (thisValue.m_pair.second < otherValue.m_pair.second)
	{
		return true;
	}

	return false;
}
//TODO: overload other operators

template<class Key, class Value, class Comparison = greater<InternalValue<Key, Value>>>
class PriorityMap
{
	unordered_set<Key> m_set;
	priority_queue<InternalValue<Key, Value>, vector<InternalValue<Key, Value>>, Comparison>  m_queue;

public:
	PriorityMap()
		:m_set(unordered_set<Key>()),
		m_queue(priority_queue<InternalValue<Key, Value>, vector<InternalValue<Key, Value>>, Comparison>())
	{

	}

	~PriorityMap()
	{

	}

	bool Contains(Key key)
	{
		if (m_set.count(key) != 0)
		{
			return true;
		}

		return false;
	}

	//returns next value, but doesn't remove it
	Value Peek()
	{
		InternalValue<Key, Value> internalValue = m_queue.top();
		return internalValue.m_pair.second;
	}

	//returns next value, and removes it
	Value Pop()
	{
		InternalValue<Key, Value> internalValue = m_queue.top();
		
		m_set.erase(internalValue.m_pair.first);
		m_queue.pop();

		return internalValue.m_pair.second;
	}

	//adds element to container
	void Push(Key key, Value newValue)
	{
		if (Contains(key))
		{
			return;
		}

		InternalValue<Key, Value> newInternalValue = InternalValue<Key, Value>(key, newValue);
		m_queue.push(newInternalValue);
		m_set.insert(key);
	}

	int Size()
	{
		return m_queue.size();
	}
};
