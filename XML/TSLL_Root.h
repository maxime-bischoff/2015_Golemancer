#ifndef TSLL_ROOT_H_INCLUDED
#define TSLL_ROOT_H_INCLUDED


template <class T> class TSLL_Root{


	

public :

	T * m_pFirstItem;


	typedef void( *IterateListCallback)(  T *);

	TSLL_Root();
	~TSLL_Root();



	void IterateList(IterateListCallback pCallback);
	T * FintLastItem();
	void Append(T * pNewItem);

	int GetCount();
	bool Remove(T * pToRemoveItem);
	
	bool Iterate();




};

#include "TSLL_Root.hpp"



#endif