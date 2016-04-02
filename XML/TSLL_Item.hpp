


template< typename T >
TSLL_Item< T >::TSLL_Item(): m_pNextItem (0)   
{
}

template< typename T >
TSLL_Item< T >::~TSLL_Item(){


	if(m_pNextItem !=0)
	{
		delete m_pNextItem;	
	}

}



template <class T>
T * TSLL_Item< T >::GetNextItem(){

	return this->m_pNextItem;

}

template <class T>
void TSLL_Item< T >::SetNextItem(T * pNextItem)
{
	this->m_pNextItem = pNextItem;
}

template <class T>
void TSLL_Item< T >::SetLast()
{
	this->m_pNextItem = 0;
}

template <class T>
void TSLL_Item< T >::InsertAfter(T * pNewItem)
{
	pNewItem->m_pNextItem = this->m_pNextItem;
	this->m_pNextItem = pNewItem;	
}




