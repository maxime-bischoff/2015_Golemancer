#pragma once

#include "menu.h"

class TPauseMenu : public TMenu
{
private:


public:
		
	TPauseMenu();
	~TPauseMenu();
	void ActionButton();
	void BackButton();
	void StartButton();
	void Update();
	void DelayAction();
	void Initialize();

	
};

class TWinMenu : public TMenu
{
private:

	

public:

		
	TWinMenu();
	~TWinMenu();
	void ActionButton();
	void BackButton();
	void Update();
	void DelayAction();
	void Initialize();
	




};



class TGameOverMenu : public TMenu
{

private:

public:

		
	TGameOverMenu();
	~TGameOverMenu();
	void ActionButton();
	void StartButton();
	void BackButton();
	void DelayAction();
	void Update();	
	void Initialize();





};


