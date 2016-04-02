#ifndef TSLL_ITEM_H_INCLUDED
#define TSLL_ITEM_H_INCLUDED
//#include "TSLL_Item.h"

template <class T> class TSLL_Item{

	//friend class TSLL_Root;

private:
	T * m_pNextItem;

public:
	TSLL_Item();
	~TSLL_Item();

	T * GetNextItem();

	void SetLast();
	void SetNextItem(T * pNextItem);

	void InsertAfter(T * pNewItem);



};

#include "TSLL_Item.hpp"



#endif