#pragma once

#include "menu.h"

class TMainMenu : public TMenu{
public:

	TMainMenu();
	~TMainMenu();
	void ActionButton();
	void BackButton();
	virtual void Initialize();
	void Update();
};
