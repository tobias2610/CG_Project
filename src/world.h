#include "Object.h"
#include "cgmath.h"
#include "Wall.h"
#include "Object.h"
#include "Enemy.h"
#pragma once
class World : public Object
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
	
	

public:

	World(Wall sWall, Enemy sEnemy, Wall sworldWall)
	{

		wall = sWall;
		enemy = sEnemy;
		worldWall = sworldWall;

	}

	World(){

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
	int getNum_WorldWalls(){
		return NUM_WORLDWALLS;
	}
	void setWorldWalls(Wall simplewall){
		worldWall = simplewall;
		worldWalls.resize(6 * sizeof(mat4));
		//front
		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		//	modelMatrix = mat4::rotate(vec3(0, 1, 0), (1 / 2.0 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y, worldWall.getPosition().z) * modelMatrix;

		worldWalls[0] = modelMatrix;

		//back
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y, worldWall.getPosition().z + 10) * modelMatrix;

		worldWalls[1] = modelMatrix;

		//left and right wall
		//right
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), (0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x + 5, worldWall.getPosition().y, worldWall.getPosition().z + 5) * modelMatrix;

		worldWalls[2] = modelMatrix;


		//left
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x - 5, worldWall.getPosition().y, worldWall.getPosition().z + 5) * modelMatrix;

		worldWalls[3] = modelMatrix;



		//bottom and seiling

		//bottom
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), (0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y + 5, worldWall.getPosition().z + 5) * modelMatrix;

		worldWalls[4] = modelMatrix;


		//seiling
		modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(worldWall.getScale(), worldWall.getScale(), worldWall.getScale()) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
		modelMatrix = mat4::translate(worldWall.getPosition().x, worldWall.getPosition().y - 5, worldWall.getPosition().z + 5) * modelMatrix;

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

