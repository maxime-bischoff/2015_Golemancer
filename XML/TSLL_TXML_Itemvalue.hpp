
#include <cstring>
#include  <cstdlib>
//#include <cwctype>

template <class T>
TSLL_TXML_ItemValue< T >::TSLL_TXML_ItemValue(void)
{
}

template <class T>
TSLL_TXML_ItemValue< T >::~TSLL_TXML_ItemValue(void)
{

	


}



template <class T> bool TSLL_TXML_ItemValue< T >::IsValueNumeric (){

	bool bError = false;
	int iIndice  = 0;
	int iSize = strlen(m_cValue);

	while( !bError && (iIndice < iSize))
	{
		bool dot = false;
		if(m_cValue[0] == '-')
		{		
			iIndice++;
		}else
		if(m_cValue[iIndice] == '.') 
		{
			if (dot == true) {bError = true ;} else{ dot = true;	}	
			if (iIndice == -1) bError = true;		
		}else{		

/*
			if(	!iswdigit(m_cValue[iIndice]))
			{
				bError = true;										
			}
			*/
			if(	m_cValue[iIndice] <0 ||m_cValue[iIndice] > 9 )
			{
				bError = true;										
			}



		}									
		iIndice++;
	}




	if(bError) return false; else return true;







}



template <class T> float TSLL_TXML_ItemValue< T >::GetValueNumeric (){

	return ( static_cast<float>(   atof(m_cValue) ));

}



template <class T> const char * TSLL_TXML_ItemValue< T >::GetValueString(){

	return m_cValue;


}

								