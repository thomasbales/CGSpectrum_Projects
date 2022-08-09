#pragma once

#include <assert.h>

#include "IDelegate.h"

template <class C, class ReturnType, class ...Args>
class MemberDelegate : public IDelegate<ReturnType, Args...>
{
	typedef ReturnType(C::* InternalFunction)(Args...);
	typedef void* InstancePtr;

	InstancePtr m_pInstance;
	InternalFunction m_pFunction;


public:
	MemberDelegate()
		:m_pInstance(nullptr),
		m_pFunction(nullptr),
		IDelegate<ReturnType, Args...>(DelegateType::Member)
	{

	}

	MemberDelegate(C* pInstance, InternalFunction pFunction)
		:m_pInstance(nullptr),
		m_pFunction(nullptr),
		IDelegate<ReturnType, Args...>(DelegateType::Member)
	{
		Bind(pInstance, pFunction);
	}

	void Bind(C* pInstance, InternalFunction pFunction)
	{
		m_pInstance = pInstance;
		m_pFunction = pFunction;
	}

	void Unbind()
	{
		m_pFunction = nullptr;
	}

	bool Compare(C* pInstance, InternalFunction pFunction)
	{
		if (pInstance == m_pInstance && pFunction == m_pFunction)
		{
			return true;
		}

		return false;
	}

	ReturnType Invoke(Args... args) override
	{
		assert(m_pInstance != nullptr);
		assert(m_pFunction != nullptr);

		(static_cast<C*>(m_pInstance)->*m_pFunction)(args...);
	}

	//overload () to call Invoke
	ReturnType operator()(Args... args)
	{
		Invoke(args...);
	}
};

