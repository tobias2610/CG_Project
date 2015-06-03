#pragma once
#include "Object.h"
class Enemy :
	public Object
{
private:
	int HP;
public:

	Enemy()
	{
	}
	Enemy(float pScale, vec3 pPosition, unsigned char* pImage) :Object(pScale, pPosition, pImage)
	{
		HP = 100;
	}

	~Enemy()
	{
	}
};

