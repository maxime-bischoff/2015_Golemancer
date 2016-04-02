#pragma once
#include "Transform.h"
class TLiving :
	public TTransform
{
public:
	TLiving();
	~TLiving();

	float m_fLife;

	void Speak();
};

