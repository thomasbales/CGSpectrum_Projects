#pragma once

#include<vector>

#include<string>
#include<list>

#include"Delegate.h"

using namespace std;

//PURPOSE:
//This class wraps a list of Delegates and calls all of their associated functions
//when Event.Invoke is called.

template<typename... Args>
class Event
{
	typedef Delegate<void, Args...> EventDelegate;

	//list of funtion pointers
	list<EventDelegate*>* m_pListeners;

public:
	//initializes listeners list on heap
	Event()
		: m_pListeners(new list<EventDelegate*>())
	{

	}

	//destroys listeners list
	~Event()
	{
		ClearListeners();

		delete m_pListeners;
		m_pListeners = nullptr;
	}

	//calls all funtion pointers in listeners
	void Invoke(Args... args)
	{
		for (auto listener : *m_pListeners)
		{
			listener->Invoke(args...);
		}
	}

	//add function to listeners list
	void AddListener(void (*pStaticFunction)(Args...))
	{
		EventDelegate* pNewDelegate = new EventDelegate();
		pNewDelegate->Bind(pStaticFunction);
		m_pListeners->push_back(pNewDelegate);
	}

	template<class C>
	void AddListener(C* pInstance, void (C::* pMemberFunction)(Args...))
	{
		EventDelegate* pNewDelegate = new EventDelegate();
		pNewDelegate->Bind<C>(pInstance, pMemberFunction);
		m_pListeners->push_back(pNewDelegate);
	}

	//remove function from callbacks
	void RemoveListener(void (*pStaticFunction)(Args...))
	{
		for (auto listener = begin(*m_pListeners); listener != end(*m_pListeners); listener++) {
			if ((*listener)->Compare(pStaticFunction))
			{
				delete* listener;
				*listener = nullptr;
				m_pListeners->erase(listener);
				break;
			}
		}
	}

	template<class C>
	void RemoveListener(C* pInstance, void (C::* pMemberFunction)(Args...))
	{
		for (auto listener = begin(*m_pListeners); listener != end(*m_pListeners); listener++) {
			if ((*listener)->Compare<C>(pInstance, pMemberFunction))
			{
				delete* listener;
				*listener = nullptr;
				m_pListeners->erase(listener);
				break;
			}
		}
	}

	//remove all callbacks
	void ClearListeners()
	{
		while (!m_pListeners->empty())
		{
			delete m_pListeners->front();
			m_pListeners->pop_front();
		}
	}

	//overload () to call Invoke
	void operator()(Args... args)
	{
		Invoke(args...);
	}
};


