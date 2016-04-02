#include "../const.h"



TXML_Element::TXML_Element()
{
	m_pElementChildsList = new TSLL_Root<TXML_Element>();
	m_pAttributesList = new TSLL_Root<TXML_Attribute>();
}

TXML_Element::~TXML_Element()
{

	if(m_pElementChildsList!=0) {
		delete m_pElementChildsList;}
	if( m_pAttributesList != 0)  {
		delete m_pAttributesList;}





}

TXML_Element * TXML_Element::GetThisElement(){

	return this;

}

TXML_Attribute * TXML_Element::FindAttribute(const  char * pName ){

	TXML_Attribute* pCurrentAttribute = 0;
	bool bFound = false;
	bool bEndOfList = false;
	if(this->m_pAttributesList->m_pFirstItem == 0) return 0; else pCurrentAttribute = m_pAttributesList->m_pFirstItem;

	while(!bFound && !bEndOfList)
	{
		if(!strcmp(pName, pCurrentAttribute->m_cName))
		{
			bFound = true;
			return pCurrentAttribute;
		
		}else{

			if(pCurrentAttribute->GetNextItem()!=0)
			{
			pCurrentAttribute = pCurrentAttribute->GetNextItem();
			
			
			}else{
			return 0;
			
			}		
		}
	}

	return 0;

}

/*
TXML_Element * TXML_Element::GetNextElement(){

	
	//return GetNextItem();
	return;

}

*/
/*

	TXML_Element * TXML_Element::GetElementList(){
	
	
	
	
	}*/