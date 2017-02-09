/**
 *	File: AXengine/Tool/Singleton.h
 *	Purpose: Make one static instance of derived class
 */

#ifndef __AX__TOOL__SINGLETON_H
#define __AX__TOOL__SINGLETON_H

namespace AX { namespace Tool {

template<typename T> class Singleton {
public:
	static T& Instance()
	{
		if(instance == nullptr)
			init();
		
		return *instance;
	}

protected:
	Singleton() { }
	
	static void init()
	{
		static T tInstance;
		instance = &tInstance;
	}
	
	static T* instance;
	
private:
	Singleton(Singleton<T>& other) { }

};

template<typename T> T* Singleton<T>::instance = nullptr;

} } //namespace AX::Tool

#endif // __AX__TOOL__SINGLETON_H
