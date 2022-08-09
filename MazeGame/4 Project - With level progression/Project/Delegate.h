#pragma once

#include <assert.h>

#include "IDelegate.h"
#include "StaticDelegate.h"
#include "MemberDelegate.h"

using namespace std;

//PURPOSE:
//This class wraps StaticDelegate and MemberDelegate into a class
//that can hold either one static function or one member function.

template<class ReturnType, class ...Args>
class Delegate : public IDelegate<ReturnType, Args...>
{
	IDelegate<ReturnType, Args...>* m_pFunction;

	IDelegate<ReturnType, Args...>* CreateStaticDelegate(ReturnType(*staticFunction)(Args...))
	{
		return new StaticDelegate<ReturnType, Args...>(staticFunction);
	}

	template<class C>
	IDelegate<ReturnType, Args...>* CreateMemberDelegate(C* pInstance, ReturnType(C::* memberFunction)(Args...))
	{
		return new MemberDelegate<C, ReturnType, Args...>(pInstance, memberFunction);
	}

public:
	Delegate()
		:m_pFunction(nullptr),
		IDelegate<ReturnType, Args...>(DelegateType::Delegate)
	{

	}

	~Delegate()
	{
		Unbind();
	}

	void Bind(ReturnType(*pStaticFunction)(Args...))
	{
		if (m_pFunction != nullptr)
		{
			Unbind();
		}

		m_pFunction = CreateStaticDelegate(pStaticFunction);
	}
	void Unbind()
	{
		delete m_pFunction;
		m_pFunction = nullptr;
	}

	template<class C>
	void Bind(C* pInstance, ReturnType(C::*pMemberFunction)(Args...))
	{
		if (m_pFunction != nullptr)
		{
			Unbind();
		}

		m_pFunction = CreateMemberDelegate<C>(pInstance, pMemberFunction);
	}

	bool Compare(ReturnType(*pStaticFunction)(Args...))
	{
		if (m_pFunction->GetType() == DelegateType::Static)
		{
			return dynamic_cast<StaticDelegate<ReturnType, Args...>*>(m_pFunction)->Compare(pStaticFunction);
		}
		
		return false;
	}

	template<class C>
	bool Compare(C* pInstance, ReturnType(C::* pMemberFunction)(Args...))
	{
		if (m_pFunction->GetType() == DelegateType::Member)
		{
			return dynamic_cast<MemberDelegate<C, ReturnType, Args...>*>(m_pFunction)->Compare(pInstance, pMemberFunction);
		}

		return false;
	}

	ReturnType Invoke(Args... args) override
	{
		assert(m_pFunction != nullptr);
		m_pFunction->Invoke(args...);
	}

	//overload () to call Invoke
	ReturnType operator()(Args... args)
	{
		Invoke(args...);
	}
};


