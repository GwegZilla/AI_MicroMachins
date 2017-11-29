#pragma once
#include "stdafx.h"
namespace aimm
{
	template<typename T>
	class Singleton
	{
	public:
		static bool isBuilt();
		static T* instance();

	protected:
		static T* _instance;
		static void buildInstance();

		Singleton() {}
		~Singleton() {}
	};

	template<typename T>
	T* Singleton<T>::_instance = 0;

	template<typename T>
	void Singleton<T>::buildInstance()
	{
		if (_instance)
			throw "Singleton already constructed";
		_instance = new T();
	}

	template<typename T>
	T* Singleton<T>::instance() {
		if (!isBuilt())
			buildInstance();
		return _instance;
	}

	template<typename T>
	bool Singleton<T>::isBuilt() {
		return (_instance != 0);
	}
}