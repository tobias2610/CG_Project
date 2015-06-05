#include "Object.h"
#include "cgmath.h"
#pragma once
class Wall : public Object
{

public:
	Wall(){

	}
	Wall(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{

	}

	bool clisionDetect(vec3 direct){

	}

	~Wall()
	{
	}
};

