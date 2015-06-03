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
	Enemy(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{
		HP = 100;
	}

	~Enemy()
	{
	}
};

