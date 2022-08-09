#pragma once

#include <assert.h>

#include "IDelegate.h"

template <class ReturnType, class ...Args>
class StaticDelegate : public IDelegate<ReturnType, Args...>
{
	typedef ReturnType (*InternalFunction)(Args...);

	InternalFunction m_pFunction;

public:
	StaticDelegate()
		:m_pFunction(nullptr),
		IDelegate<ReturnType, Args...>(DelegateType::Static)
	{

	}

	StaticDelegate(InternalFunction pFunction)
		:m_pFunction(nullptr),
		IDelegate<ReturnType, Args...>(DelegateType::Static)
	{
		Bind(pFunction);
	}

	void Bind(InternalFunction pFunction)
	{
		m_pFunction = pFunction;
	}

	void Unbind()
	{
		m_pFunction = nullptr;
	}

	bool Compare(InternalFunction pFunction)
	{
		if (pFunction == m_pFunction)
		{
			return true;
		}

		return false;
	}

	ReturnType Invoke(Args... args) override
	{
		assert(m_pFunction != nullptr);
		m_pFunction(args...);
	}

	//overload () to call Invoke
	ReturnType operator()(Args... args)
	{
		Invoke(args...);
	}
};
