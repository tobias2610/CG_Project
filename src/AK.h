#pragma once
#include "cgmath.h"
#include "Object.h"
#include "Aim.h"
class AK : public Object
{
private:
	vec3 direction;
	int bullets;
	int maxBullets=30;
	int bulletStock;
	Aim aim;
	int HP;
	int score;
public:
	AK(){

	}
	AK(float pScale, vec3 pPosition, char* pImagePath, char* path) :Object(pScale,pPosition,pImagePath,path)
	{
		HP = 100;
		bulletStock = 60;
		bullets = 30;
		direction = vec3(0,0,1);
		aim = Aim(0.05f, vec3(0, 0, -10), "../bin/Images/tex_AK.jpg", "Circle");
		score = 0;
	}
	void damage(int damage){
		HP -= damage;
		if (HP <= 0){
			exit(0);
		}
	}
	int getScore(){
		return score;
	}
	void setScore(int add ){
		score += add;
	}
	int getHP(){
		return HP;
	}
	Aim* getAim(){
		return &aim;
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

