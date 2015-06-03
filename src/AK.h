#pragma once
#include "cgmath.h"
#include "Object.h"
class AK : public Object
{
private:
	vec3 direction;
public:
	AK(){

	}
	AK(float pScale, vec3 pPosition, unsigned char* pImage) :Object(pScale,pPosition,pImage)
	{
		direction = vec3(0,0,0);
	}

	vec3 getDirection(){
		return direction;
	}
	void setDirection(vec3 pDirection){
		direction = pDirection;
	}

	void shoot(){
		
	}

	~AK()
	{
		//free(image);
	}
};

