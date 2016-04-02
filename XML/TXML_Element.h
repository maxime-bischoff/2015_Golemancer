#ifndef TXML_ELEMENT_H_INCLUDED
#define TXML_ELEMENT_H_INCLUDED

#include "TSLL_TXML_Itemvalue.h"

class TXML_Element;
class TXML_Attribute;
#include "TSLL_Root.h"

class TXML_Element : public  TSLL_TXML_ItemValue<TXML_Element>
{

	//friend class TSLL_TXML_ItemValue;
public:
	TXML_Element();
	~TXML_Element();

	TXML_Element *  GetNextElement();
	TXML_Element *  GetElementList();
	TXML_Element *  GetThisElement();

	TSLL_Root<TXML_Element> *  m_pElementChildsList;
	TSLL_Root<TXML_Attribute> *  m_pAttributesList;


	TXML_Element * FindNextSiblingElement(const char * pName );
	TXML_Element * FindFirstChildElement(const char * pName );
	TXML_Attribute * FindAttribute(const  char * pName );




	


};


#endif

