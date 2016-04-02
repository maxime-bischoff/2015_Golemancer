#ifndef TSLL_TXML_ITEMVALUE_H_INCLUDED
#define TSLL_TXML_ITEMVALUE_H_INCLUDED

#include "TSLL_Item.h"

template <class T> class TSLL_TXML_ItemValue : public TSLL_Item<T>
{
public:
	TSLL_TXML_ItemValue(void);
	~TSLL_TXML_ItemValue(void);


	static const int MAX_SIZE_NAME = 512;
	static const int MAX_SIZE_VALUE = 512;

	char m_cName[MAX_SIZE_NAME +1];
	char m_cValue[MAX_SIZE_VALUE +1];


	bool IsValueNumeric ();
	float GetValueNumeric ();
	const char * GetValueString();
	

private:
};



#include "TSLL_TXML_Itemvalue.hpp"

#endif