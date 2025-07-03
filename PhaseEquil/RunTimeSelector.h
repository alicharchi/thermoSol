#pragma once
#include <memory>

template<class T, class P>
class RunTimeSelector
{
public:

	RunTimeSelector() = default;
	virtual ~RunTimeSelector() = default;

	T* const Get() const
	{
		return _instance.get();
	}

	const std::unique_ptr<T>& GetRef()
	{
		return _instance;
	}

	void Select(P type)
	{
		_type = type;
		SelectInternal(_type);
	}

	void Select(int type)
	{
		Select(static_cast<P>(type));
	}

	P GetType() const
	{
		return _type;
	}

protected:
	std::unique_ptr<T> _instance;
	P _type;
	virtual void SelectInternal(P type) = 0;
};

