#ifndef _PTR_VECTOR_HH
#define _PTR_VECTOR_HH

#include "auto_ptr.hh"
#include <vector>

namespace oneself{

template<typename T>
class ptr_vector : public std::vector<T*>{
public:
	explicit ptr_vector(){
	}
	
	~ptr_vector(){
		clear();
	}

	void clear(){
		typename std::vector<T*>::iterator it;
		for(it = std::vector<T*>::begin(); it != std::vector<T*>::end(); ++it){
			delete *it;//释放指针指向的内存.
		}
		
		/*
		for(size_t i = 0; i < std::vector<T*>::size(); ++i){
			delete std::vector<T*>::back();
		}*/
		
		std::vector<T*>::clear(); //释放指针本身.
	}

	typename std::vector<T*>::iterator erase(typename std::vector<T*>::iterator it){
		if(it >= std::vector<T*>::begin() && it < std::vector<T*>::end()){
			delete *it;
			std::vector<T*>::erase(it);
		}
	}

	void pop_back(){
		if(std::vector<T*>::size() > 0){
			delete std::vector<T*>::back();
			std::vector<T*>::pop_back();
		}
	}
	
	void push_back(T* const &v){
		oneself::auto_ptr<T> ap(v);
		std::vector<T*>::push_back(v);
		ap.release();
	}

	void push_back(oneself::auto_ptr<T> &v){
		std::vector<T*>::push_back(v.get());
		v.release();
	}

private:
	ptr_vector<T>& operator=(const ptr_vector<T>&);
	ptr_vector<T>(ptr_vector<T>&);

};

}

#endif
