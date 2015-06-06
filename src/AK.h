#pragma once
#include "cgmath.h"
#include "Object.h"
class AK : public Object
{
private:
	vec3 direction;
	int bullets;
	int maxBullets=30;
	int bulletStock;
public:
	AK(){

	}
	AK(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale,pPosition,pImagePath,path)
	{
		bulletStock = 60;
		bullets = 30;
		direction = vec3(0,0,1);
	}

	int getBulletStock(){
		return bulletStock;
	}
	void setBulletStock(int pBulletStock){
		bulletStock = pBulletStock;
	}

	int getMaxBullet(){
		return maxBullets;
	}
	int getBullets(){
		return bullets;
	}
	void setBullets(int pBullets){
		bullets = pBullets;
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

