#pragma once

template<class T>
class singleton
{
	singleton() = default;
	~singleton() = default;

public:
	static T &instance()
	{
		static T inst;

		return inst;
	}
};

