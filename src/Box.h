#include "Object.h"
#include "cgmath.h"
#pragma once
class Box : public Object
{

public:
	Box(){

	}
	Box(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{

	}

	bool clisionDetect(vec3 direct){
		
	}

	~Box()
	{
	}
};

