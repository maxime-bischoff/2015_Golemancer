
#include <ctype.h>
#include <cstring>

#include "../Const.h"


TXML_Root::TXML_Root()
{
	m_pRootElementsList = new TSLL_Root<TXML_Element>();
}

TXML_Root::~TXML_Root()
{

	 delete m_pRootElementsList;
}








char * TXML_Root::XMLParseElement(char * pChar, TXML_Element * pParent)
{
	


	bool bAutoClose = false;
	char * pOpenChar = pChar;
	char * pCloseChar = strchr(pChar, '>');

	char *pAutoCloseChar = strchr(pChar, '/');

	if(pCloseChar == pAutoCloseChar+1  ){
		bAutoClose = true;
	}

	if(*pOpenChar+1 == '?') 
	{
		pChar+= pChar - strstr(pChar, "?>") + 2;	
		return pChar;
	}
	
	if(pOpenChar+1 == strstr(pChar, "!--") )
	{
		pChar=  strstr(pChar, "-->") + 3;	
		return pChar;
	}

	char * pElementNameStart = pChar+1;
	char * pElementNameEnd = pChar+1;	

	int  iElementLength = pCloseChar - pOpenChar -1;

	// Get the element name size

	for( int i = 0; i < iElementLength+1; i++ )
	{
		if( !isgraph( *(pElementNameEnd) ) ) 
		{
			pElementNameEnd++;
			break;
		}
					 
		pElementNameEnd++;
	}
	TXML_Element * tElement = new TXML_Element();


	int iLength = pElementNameEnd -1- pElementNameStart;
	if(iLength> TSLL_TXML_ItemValue<TXML_Element>::MAX_SIZE_NAME )
	{
		iLength = TSLL_TXML_ItemValue<TXML_Element>::MAX_SIZE_NAME;
		//GfxDbgPrintf("Element name cropped");	
	}

	strncpy(tElement->m_cName, pElementNameStart,iLength );
	tElement->m_cName[iLength] = 0;

	//if(pParent!=0)  GfxDbgPrintf("Parent:.%s.\n", pParent->m_cName);
//		GfxDbgPrintf(".%s.\n", tElement->m_cName);
	//	GfxDbgPrintf(".%s.\n", tElement->m_cValue);


	// =============== Attributes extraction ================================== //
	char * pCurrentChar = pElementNameEnd;
	char * pEqualAttribute = strchr (pCurrentChar, '=');	

		
	while ( pEqualAttribute !=0 && pEqualAttribute < pCloseChar)
	{
		// ==== Name attribute extraction === //
		char * pAttributeNameStart = pEqualAttribute -1;
		char * pAttributeNameEnd = pEqualAttribute -1;
		bool bSeparatorFound = false;
		TXML_Attribute * tAttribute = new TXML_Attribute();

		while(!bSeparatorFound)
		{
			if( isgraph(* (pAttributeNameStart-1) ))
			{
				*pAttributeNameStart--;
			}else{						
				bSeparatorFound = true;						
			}				
		}
		
		iLength = pAttributeNameEnd - pAttributeNameStart+1;
		if(iLength> TSLL_TXML_ItemValue<TXML_Attribute>::MAX_SIZE_NAME ) // Test the attribute name
		{
			iLength = TSLL_TXML_ItemValue<TXML_Attribute>::MAX_SIZE_NAME;
		//	GfxDbgPrintf("Attribute name cropped");	
		}
		strncpy(tAttribute->m_cName,  pAttributeNameStart, iLength); 
		tAttribute->m_cName[iLength] = 0;


		// ==== Value attribute extraction === //

		char * pAttributeValueStart = strchr(pEqualAttribute, '"');
		char * pAttributeValueEnd = strchr(pAttributeValueStart+1, '"');
	

		
		iLength = pAttributeValueEnd - pAttributeValueStart-1;
		if(iLength> TSLL_TXML_ItemValue<TXML_Attribute>::MAX_SIZE_VALUE ) // Test the attribute Value
		{
			iLength = TSLL_TXML_ItemValue<TXML_Attribute>::MAX_SIZE_VALUE;
		//	GfxDbgPrintf("Attribute value cropped");	
		}
		strncpy(tAttribute->m_cValue,  pAttributeValueStart+1, iLength); 
		tAttribute->m_cValue[iLength] = 0;

		
		// ==== Attribute integration ==== //
		
		if(tElement->m_pAttributesList == 0)
		{
			tElement->m_pAttributesList =  new TSLL_Root<TXML_Attribute>();					
		}
		tElement->m_pAttributesList->Append(tAttribute);	

	//	GfxDbgPrintf("AttributeName.%s.\n", tAttribute->m_cName);
		//GfxDbgPrintf("AttributeValue.%s.\n", tAttribute->m_cValue);


		
		pCurrentChar =  pAttributeValueEnd+1;
		pEqualAttribute = strchr (pCurrentChar, '=');	


	}	




	if(bAutoClose)
	{


		if( pParent == 0 )
		{
			this->m_pRootElementsList->m_pFirstItem = tElement;
			pParent = tElement;		
		}
		else
		{
			pParent->m_pElementChildsList->Append( tElement );	
		}
		return pCloseChar+1;	
	}
	pChar = pCloseChar+1;

	for( int i = 0; i < TSLL_TXML_ItemValue<TXML_Element>::MAX_SIZE_VALUE; i++ )
		{
			if( *pChar == '<' )
			{
				tElement->m_cValue[i] = 0;
				break;
			}
			

			if(isgraph(*pChar) || *pChar ==' ')
			{
				tElement->m_cValue[i]  = *pChar;
				tElement->m_cValue[i+1] = 0;
			}

			pChar++;
		}

	

	bool bEndFound = false;
		
	while(!bEndFound)
	{
			if( *(pChar+1) != '/' )
			{
				pChar = XMLParseElement( pChar, tElement );

				if( pChar == 0 )
					return 0;

				pChar = strchr( pChar, '<' );
			}else{
				pChar = strchr( pChar, '>' );
				bEndFound = true;			
			}
	}


	// Copie du nom et clamp si plus grand que le tableau
	
	



		if( pParent == 0 )
		{
			this->m_pRootElementsList->m_pFirstItem = tElement;
			pParent = tElement;		
		}
		else
		{
			pParent->m_pElementChildsList->Append( tElement );	
		}

	//	GfxDbgPrintf(".%s.\n", tElement->m_cName);
	//	GfxDbgPrintf(".%s.\n", tElement->m_cValue);


	return pChar;
	
}







/*TXML_Element * TXML_Root::FindFirstElement(const char * pName )
{
	bool bFound = false;
	TSLL_Root<TXML_Element> * pCurrentElement = m_pRootElementsList;
	



}*/