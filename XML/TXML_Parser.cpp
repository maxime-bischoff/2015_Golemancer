#include <cstring>

#include <stdio.h>
#include "../Const.h"
//#include <conio.h>
//#include "windows.h"



EElementName TXML_TParser::m_eCurrentElementName = EELEMENT_LEVELSHAPE;



TXML_TParser::TXML_TParser(){
	//654564

}



TXML_TParser::~TXML_TParser(){


	if(m_tRoot != 0) delete m_tRoot;


}




TXML_Root * TXML_TParser::ReadXML( char * filename )
{
	char *buffer=0;
	m_tRoot = new TXML_Root();

	

	TGfxFile * pFile = GfxFileOpenRead( filename );

	if( pFile == 0 )
	{
		GfxDbgPrintf("ERROR : Cannot open the file: %s\n",filename);
		return 0;
	}

	int size = GfxFileSize( pFile );

	//GfxDbgPrintf("Reading XML file: %s\n",filename);
	buffer = FMALLOC(char, size+1 );
	GfxFileRead( pFile, (void*) buffer, size );

	buffer[ size ] =0; // force cstring terminator

#if 1
	//GfxDbgPrintf("------ORIGINAL FILE---------\n");
	//GfxDbgPrintf(buffer);
	//GfxDbgPrintf("----------------------------\n");
#endif

	// analyze the file content
	char *curPosition = buffer;


	

	while( *curPosition !=0 )
	{
		
		

		char * openChar = strchr(curPosition, '<');

		if(  openChar == 0)
		{
			*curPosition = 0;
		
		}
		else{
		curPosition = m_tRoot->XMLParseElement(openChar, 0);

		}

		//GfxDbgPrintf("Fini");
		
	}
		
	// http://www.cplusplus.com/reference/clibrary/cstring/strstr/

	//delete [] buffer;
	FFREE(buffer);

	GfxFileClose(pFile);


	return m_tRoot;
}

void TXML_TParser::GetElement(TXML_Element * pElement){

	//GfxDbgPrintf("\n%s ", pElement->m_cName);
	//GfxDbgPrintf("\n%s ", pElement->m_cValue);
	GetElementByName(pElement->m_cName);
	ElementToGame(pElement);
	pElement->m_pElementChildsList->IterateList( GetElement );

	
	//pElement->m_pAttributesList->IterateList(PrintAttribute);


}
/*
void TXML_TParser::GetAttribute(TXML_Attribute * pAttribute){
	
	GfxDbgPrintf("\n	 %s ", pAttribute->m_cName);

	if(pAttribute->IsValueNumeric())
		GfxDbgPrintf("Num =  : %f ", pAttribute->GetValueNumeric());
	else
		GfxDbgPrintf("Alpha = : %s ", pAttribute->GetValueString());


}
*/
void TXML_TParser:: ElementToGame(TXML_Element * /*pElement*/){
	//TXML_Attribute * pAttribute = 0;
	//switch(m_eCurrentElementName)
	//{
	//case EELEMENT_COLLISION :	{
	//					TGfxVec2 tPos ;
	//					TGfxVec2 tSize ;
	//					float fAngle;

	//					pAttribute = pElement->FindAttribute("rotation");
	//					if(pAttribute==0)
	//					return;	else fAngle = pAttribute->GetValueNumeric() * PI /180.0f  ;

	//					pAttribute = pElement->FindAttribute("posX");
	//					if(pAttribute==0)
	//					return;	else tPos.x = pAttribute->GetValueNumeric();

	//					pAttribute = pElement->FindAttribute("posZ");
	//					if(pAttribute==0)
	//					return;	else tPos.y =- pAttribute->GetValueNumeric();

	//					pAttribute = pElement->FindAttribute("sizeX");
	//					if(pAttribute==0)
	//					return;	else tSize.x = pAttribute->GetValueNumeric()/2.0f;

	//					pAttribute = pElement->FindAttribute("sizeZ");
	//					if(pAttribute==0)
	//					return;	else tSize.y = pAttribute->GetValueNumeric()/2.0f;

	//					pAttribute = pElement->FindAttribute("shape");
	//					if(pAttribute==0)
	//						return;	else    {

	//							//GfxDbgPrintf(  pAttribute->GetValueString() );

	//							if ( !strcmp( pAttribute->GetValueString() , "Box")) {

	//							
	//						
	//					
	//					

	//							}else{

	//					

	//							}

	//					};




	//				
	//	
	//					break;}

	//case EELEMENT_PLAYER:{

	//					TGfxVec2 tPos ;
	//					TGfxVec2 tSize ;
	//					pAttribute = pElement->FindAttribute("posX");
	//					if(pAttribute==0)
	//					return;	else tPos.x = pAttribute->GetValueNumeric();

	//					pAttribute = pElement->FindAttribute("posZ");
	//					if(pAttribute==0)
	//					return;	else tPos.y = -pAttribute->GetValueNumeric();

	//				



	//					break;
	//					}


	//

	//		






	//
	//
	//
	//				
	//}



}



void PrintAttribute(TXML_Attribute * pAttribute){

	GfxDbgPrintf("\n	 %s ", pAttribute->m_cName);

	if(pAttribute->IsValueNumeric())
		GfxDbgPrintf("Num =  : %f ", pAttribute->GetValueNumeric());
	else
		GfxDbgPrintf("Alpha = : %s ", pAttribute->GetValueString());
}

void PrintName(TXML_Element * pElement){

	GfxDbgPrintf("\n%s ", pElement->m_cName);
	//GfxDbgPrintf("\n%s ", pElement->m_cValue);
	pElement->m_pElementChildsList->IterateList(PrintName);
	pElement->m_pAttributesList->IterateList(PrintAttribute);

}




void TXML_TParser::DoXML(char * filename){

	
	m_tRoot = ReadXML(filename);


	PrintName(m_tRoot->m_pRootElementsList->m_pFirstItem);
	


}

void TXML_TParser::ExtractXML(){

	
	m_tRoot->m_pRootElementsList->IterateList(GetElement);


}



bool TXML_TParser::CompareStringValue( const char * pStr1, const char * pStr2 )
{
	if( strcmp(pStr1,pStr2) == 0 )return false; else return true;
}


void TXML_TParser::GetElementByName(const char * pStrElementName)
{
	if(  !strcmp(pStrElementName, "collision"))
	{
		m_eCurrentElementName = EELEMENT_COLLISION;
		return;	
	}

	if( !strcmp(pStrElementName, "playerspawn"))
	{
		m_eCurrentElementName = EELEMENT_PLAYER;
		return;	
	}
	
	if( !strcmp(pStrElementName, "node"))
	{
		m_eCurrentElementName = EELEMENT_NODE;
		return;	
	}

	if( !strcmp(pStrElementName, "point"))
	{
		m_eCurrentElementName = EELEMENT_LEVELSHAPE;
		return;	
	}
}



/*
void TXML_TParser::GetAttributeByName(const char * pStrAttributeName)
{
	if(  strcmp(pStrAttributeName, "rotation"))
	{
		m_eCurrentAttributeName = EAttributeName_angle;
		return;	
	}

	if(  strcmp(pStrAttributeName, "posX"))
	{
		m_eCurrentAttributeName = EAttributeName_posX;
		return;	
	}

		if(  strcmp(pStrAttributeName, "posY"))
	{
		m_eCurrentAttributeName = EAttributeName_posX;
		return;	
	}

	if(  strcmp(pStrAttributeName, "sizeX"))
	{
		m_eCurrentAttributeName = EAttributeName_sizeX;
		return;	
	}

	if(  strcmp(pStrAttributeName, "sizeY"))
	{
		m_eCurrentAttributeName = EAttributeName_sizeY;
		return;	
	}
}

*/

