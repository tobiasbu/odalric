/*
Singleton.hpp
Created: 07/11/2015
Singleton template class for C++.
By: Tobias B. Ulrich
Using sources: http://www.yolinux.com/TUTORIALS/C++Singleton.html and https://gist.github.com/daniebker/2299755.

*/



#ifndef _OAI_SINGLETON_HPP_
#define _OAI_SINGLETON_HPP_

//#include <assert.h> 

template<typename T>
class Singleton
{
private:



	Singleton(Singleton const&) {};
	Singleton& operator=(Singleton const&) {};

protected:

	Singleton() {};
	~Singleton(void) {};

public:



	void destroy() {

		//delete Singleton<T>::m_instance;
		//Singleton<T>::m_instance = 0;

	}

	static T &getInstance() {

		//if (!m_instance)
		//m_instance = new T;

		//assert(m_instance != NULL);
		static T m_instance;

		return m_instance;

	}


};

//template <class T> T* Singleton<T>::m_instance = NULL;




#endif