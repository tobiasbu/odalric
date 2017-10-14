
#ifndef _OAI_COUNTCONTAINER_H_
#define _OAI_COUNTCONTAINER_H_


#include <unordered_map>

class CountContainer
{
private:

	int value = 0;

public:

	CountContainer()
	{

	}

	CountContainer(const int & value)
	{
		this->value = value;
	}

	void set(const int & value)
	{
		this->value = value;
	}

	int get() const
	{
		return value;
	}

	int & operator()()
	{
		return value;
	}

	const int & operator()() const
	{
		return value;
	}

	const int & operator+=(int rhs)
	{
		return value += rhs;

	}

	const int & operator+(int rhs)
	{
		value += rhs;
		return value;

	}

	const int & operator++()
	{
		value++;
		return value;

	}

	const int & operator=(int rhs)
	{
		value = rhs;
		return value;

	}

	const int & operator-=(int rhs)
	{
		return value -= rhs;

	}

	const int & operator-(int rhs)
	{
		value -= rhs;
		return value;

	}
};

/*
template<class T>
class CountContainer : public CountContainerBase
{
private:

	
	T type;

public:


	
	const T & getType() const
	{
		return T;
	}


	

};*/

template<class T>
class CountMap
{
private:

	T type;
	std::unordered_map<T, CountContainer> _map;

public:

	void set(const T & type, const int & value)
	{
		_map[type] = value;
	}

	CountContainer & get(const T & type)
	{
		return _map[type];
	}

	const T & getType()
	{
		return type;
	}

	void resetAll()
	{
		std::unordered_map<T, CountContainer>::iterator it = _map.begin();

		while (it != _map.end())
		{
			it->second = 0;
			it++;
		}
		/*for each (auto var in _map)
		{
			(*var).second = 0;

		}*/
	}

	CountContainer & operator()(const T & type)
	{
		if (_map.find(type) == _map.end())
			set(type, 0);

		return _map[type];
	}

	const CountContainer & operator()(const T & type) const
	{
		if (_map.find(type) == _map.end())
			set(type, 0);

		return _map[type];
	}

	CountContainer & operator[](const T & index) {

		return _map[index];
		//(&min)[index];

	}

	const CountContainer & operator[](const T & index) const {

		return _map[index];
		//return (&min)[index];

	}
	
};

namespace std {


		template<class U>
		struct hash<CountMap<U>>
		{
			U operator()(CountMap<U> const & x) const
			{
				return x.getType();

			}


		};

		template <class U>
		struct equal_to<CountMap<U>> : public unary_function<CountMap<U>, bool>
		{
			bool operator()(const CountMap<U>& x, const CountMap<U>& y) const
			{
				return  (x.getType() == y.getType());
			}
		};
	}





#endif