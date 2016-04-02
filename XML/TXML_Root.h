#ifndef TXML_ROOT_H_INCLUDED
#define TXML_ROOT_H_INCLUDED


class TXML_Element;
//#include "TSLL_Root.h";
//#include "TXML_Element.h";
#include "TSLL_Root.h"

class TXML_Root
{
public:
	TXML_Root();
	~TXML_Root();

	

	char * XMLParseElement(char * pChar, TXML_Element * pParent);

	TSLL_Root<TXML_Element> * m_pRootElementsList;
	
	//TXML_Element * FindFirstElement(const char * pName );

private:

	
//	class TXML_Element * m_pRootElement;
	



};


#endif
