#pragma once
#include "Object.h"
class Aim :
	public Object
{
public:

	Aim(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{
		
	}
	
	

	Aim() : Object()
	{

	}
	~Aim()
	{
	}
};

