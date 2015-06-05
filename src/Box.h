#include "Object.h"
#include "cgmath.h"
#pragma once
class Box : public Object
{
private:
	float angleX;
	float angleY;
public:
	Box(){

	}
	Box(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale, pPosition, pImagePath, path)
	{
		angleX = 0;
		angleY = 0;
	}

	bool clisionDetect(vec3 direct){
		
	}

	float getXRotation(){
		return angleX;
	}

	float getYRotation(){
		return angleY;
	}

	void setXRotation(float dx){
		angleX = dx;
	}
	void setYRotation(float dy){
		angleY = dy;
	}
	~Box()
	{
	}
};

