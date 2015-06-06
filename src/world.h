#include "Object.h"
#include "cgmath.h"
#include "Wall.h"
#include "Object.h"
#include "Enemy.h"
#pragma once
class World
{
private:
	float angleX;
	float angleY;
	const int NUM_WORLDWALLS = 6;
	//list of modelMatrix for each Object Cathegory
	std::vector<mat4> surroundings;
	std::vector<mat4> worldWalls;
	std::vector<mat4> enemies;
	Wall wall;
	Enemy enemy;
	Wall worldWall;
	vec4 addPos;
	
	

public:

	World(Wall sWall, Enemy sEnemy, Wall sworldWall)
	{

		wall = sWall;
		enemy = sEnemy;
		worldWall = sworldWall;
		angleX = 0;
		angleY = 0;
		addPos = vec4(0, 0, 0, 0);

	}

	World(){

	}
	bool clisionDetect(vec3 direct){

	}
	vec4 getPosition(){
		return addPos;
	}
	void setPosition(vec4 pPos){
		addPos = pPos;
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
	int getNum_WorldWalls(){
		return NUM_WORLDWALLS;
	}
	Wall getWall(){
		return worldWall;
	}
	void setWorldWalls(Wall simplewall){
		worldWall = simplewall;
		worldWalls.resize(6 * sizeof(mat4));

		//-------------------------------------------------------front-------------------------------------
		vec3 position = vec4(0, 0, 0, 0) + addPos;
		vec3 rotationAxis = vec3(0, 1, 0);
		float angle = 0.5 * PI;
		mat4 modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);
		modelMatrix = mat4::rotate(vec3(0, 1, 0), angleX)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), angleY)*modelMatrix;
		worldWalls[0] = modelMatrix;

		//-------------------------------------------------------back---------------------------------------
		position = vec4(0, 0, 10,0) + addPos;
		angle = PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);
		modelMatrix = mat4::rotate(vec3(0, 1, 0), angleX)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), angleY)*modelMatrix;
		worldWalls[1] = modelMatrix;

		//*********************************************left and right wall***********************************************
		//-----------------------------------------------right--------------------------------------------------
		position = vec4(5, 0, 5, 0) + addPos;
		//rotationAxis = vec3(0, 1, 0);
		angle = 0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);
		modelMatrix = mat4::rotate(vec3(0, 1, 0), angleX)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), angleY)*modelMatrix;
		worldWalls[2] = modelMatrix;
		//----------------------------------------------left----------------------------------------------------

		position = vec4(-5, 0, 5, 0) + addPos;
		angle = -0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);
		modelMatrix = mat4::rotate(vec3(0, 1, 0), angleX)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), angleY)*modelMatrix;
		worldWalls[3] = modelMatrix;

		//**************************************************bottom and seiling*************************************

		//-------------------------------------------------------bottom------------------------------
		position = vec4(0, 5, 5, 0) + addPos;
		rotationAxis = vec3(1, 0, 0);
		angle = 0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);
		modelMatrix = mat4::rotate(vec3(0, 1, 0), angleX)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), angleY)*modelMatrix;
		worldWalls[4] = modelMatrix;


		//-----------------------------------------seiling--------------------------------------------------
		position = vec4(0, -5, 5, 0) + addPos;
		rotationAxis = vec3(1, 0, 0);
		angle = -0.5 * PI;
		modelMatrix = worldWall.calcModelMatrix(position, rotationAxis, angle);
		modelMatrix = mat4::rotate(vec3(0, 1, 0), angleX)*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), angleY)*modelMatrix;
		worldWalls[5] = modelMatrix;
	}
	void initEnemies(){

	}
	void initSurroundingWalls(){

	}
	std::vector<mat4> getWorldWalls(){
		
		return worldWalls;
	}

	~World()
	{
	}
};

