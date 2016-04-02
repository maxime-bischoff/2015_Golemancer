#pragma once

#include "Map.h"

class THome : public TMap
{
public:
	THome();
	~THome();

	void Load();
	void Update();
	void Render();
};

