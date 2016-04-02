
#include "TXML_Parser.h"
template <class T>
	TSLL_Root< T >::TSLL_Root():
		m_pFirstItem(0)
	{
	
	
	}

	
template <class T>

void TSLL_Root< T >::IterateList(IterateListCallback pCallback){	

	T * pItem;
	pItem =m_pFirstItem;
	while(pItem)
	{
		if(pCallback)
		{
			pCallback( pItem );
		}

		pItem = pItem->GetNextItem();		
	}
	
	
	}



template <class T>
TSLL_Root< T >::~TSLL_Root(){

	if(m_pFirstItem !=0)
	delete m_pFirstItem;




}



template <class T>
T * TSLL_Root< T >::FintLastItem()
{
	T * pCurrentItem = this->m_pFirstItem;	
	
		while(pCurrentItem->GetNextItem()!=0)
		{	
			pCurrentItem = pCurrentItem->GetNextItem();		
		}
	
		
	return pCurrentItem;
}


template <class T>
void TSLL_Root< T >::Append(T * pNewItem)
{
	if(this->m_pFirstItem ==0)
	{
		m_pFirstItem = pNewItem;
	}else{
		T * pLastItem = FintLastItem();
		pLastItem->InsertAfter(pNewItem);
		pNewItem ->SetLast();
		}
//	pNewItem ->SetNextItem(0);
}

template <class T>
int TSLL_Root< T >::GetCount()
{
	int iCount = 0;

	T *pCurrentItem = this->m_pFirstItem;

	if(m_pFirstItem ==0)
	{
		return 0;
	}
	else
	{
		iCount++;

		while(pCurrentItem->GetNextItem()!=0)
		{	
			pCurrentItem = pCurrentItem->GetNextItem();		
			iCount++;
		}

	return iCount;	
	}
}


template <class T>
bool TSLL_Root< T >::Remove(T * pToRemoveItem)
{	
	T *pCurrentItem = this->m_pFirstItem;

	if(pCurrentItem ==0)
	{
		return false;


	}else{
		while (pCurrentItem->GetNextItem() != pToRemoveItem)
		{
			if(pCurrentItem->GetNextItem() == 0) 
				return false;

			pCurrentItem = pCurrentItem->GetNextItem();	
		}
	}

	pCurrentItem->SetNextItem(pCurrentItem->GetNextItem()->GetNextItem())  ;	
	pToRemoveItem->SetLast();
	return true;

	
}

