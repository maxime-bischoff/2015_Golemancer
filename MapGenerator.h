#pragma once


#include "Map.h"
using std::vector;




class TMapGenerator
{
public:
	TMapGenerator();
	~TMapGenerator();

	//:: VARIABLES
    vector<TRoom *> m_pRoomPatterns;

	int m_iRoomPatternsCount;

	static EDoor GetOppositeDoor(EDoor eDoor);

	//:: FUNCTIONS
	TRoom 
		* GetRandomRoomOfType(ERoomType eType),
		* FindRoomToConnect(EDoor eDoor,  ERoomType eType);

	vector < TRoom * > GetAllRoomOfType(ERoomType eType);
	

	void
		ImportRoomPatterns(),
		GenerateHome(THome * pHome),
	
		
		ImportGroundCollider(  string sFileName );

	

	TMap
		* GenerateMap(const int iSeed, int iDifficulty),
		*GenerateBoss(),
		*GenerateTuto();
		
	

};

