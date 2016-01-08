/*
*	µ¥ÀýÄ£°å
*	Author:daihongtian
*/

#pragma once

#include <mutex>
#include "uncopyable.h"

template <typename T>
class Singleton : public Uncopyable
{
public:
	static T *GetInstance()
	{
		if (NULL == s_ins)
		{
			std::lock_guard lock(GetMutex());
			if (NULL == s_ins)
			{
				s_ins = new (T)();
				atexit(Destory);
			}
		}

		return s_ins;
	}

private:
	static void Destory()
	{
		delete s_ins;
		s_ins = NULL;
	}

	static std::mutex& GetMutex()
	{
		static std::mutex mtx;
		return mtx;
	}

	Singleton() {}

	~Singleton() {}

private:
	static T* volatile s_ins;
};

template<class T>
T* volatile Singleton<T>::s_ins = NULL;