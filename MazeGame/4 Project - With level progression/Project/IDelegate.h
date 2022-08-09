#pragma once

enum class DelegateType
{
	Static,
	Member,
	Delegate
};

template<class ReturnType, class ...Args>
class IDelegate
{
protected:
	DelegateType m_type;
public:
	IDelegate(DelegateType type)
		:m_type(type)
	{

	}

	DelegateType GetType()
	{
		return m_type;
	}

	virtual ReturnType Invoke(Args... args) = 0;
};


