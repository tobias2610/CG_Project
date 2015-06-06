#include <windows.h> 
#include <mmsystem.h> 
#include "support.h"
#include "cgmath.h"		// slee's simple math library
#include "trackball.h"
#include "mesh.h"
#include "light.h"
#include "AK.h"
#include "Box.h"
#include "Enemy.h"
#include "Wall.h"
#include "World.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#define stepSize 0.25f
#define num_boxes 3
#define num_enemyes 1
#define num_Walls 6
//*******************************************************************
// index variables for OpenGL objects
GLuint	program = 0;					// ID holder for GPU program
GLuint	vertexPositionBuffer = 0;		// ID holder for vertex position buffer
int     windowWidth, windowHeight;

//*******************************************************************
// global variables
int			frame = 0;	// index of rendering frames
bool		bMouseLButtonDown = false;
Camera		camera;
AK			ak;
Box			box;
Wall		wall;
Wall		worldWall;
Enemy		enemy;
Trackball	trackball(camera.viewMatrix, 1.0f);
GLuint		textureObject = 0;
Light		light;
Material	material;
std::vector<Object> objects;
std::vector<Wall> Map;
std::vector<Enemy> Enemys;
int xBefore=0;
int yBefore = 0;
World  *world;

//World data

btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;
//*******************************************************************
void update()
{
	// configure projection matrix
	camera.fovy = PI / 6.0f;
	//camera.aspectRatio; // udpate in reshape()
	camera.dNear = 1.0f;
	camera.dFar = 1000.0f;
	camera.projectionMatrix = mat4::perspective(camera.fovy, camera.aspectRatio, camera.dNear, camera.dFar);

	// setup uniform matrix
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, camera.viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, camera.projectionMatrix);
	glBindTexture(GL_TEXTURE_2D, textureObject);
	glUniform1i(glGetUniformLocation(program, "TEX"), 0);	 // GL_TEXTURE0

	// setup light properties
	glUniform4fv(glGetUniformLocation(program, "lightPosition"), 1, light.position);
	glUniform4fv(glGetUniformLocation(program, "Ia"), 1, light.ambient);
	glUniform4fv(glGetUniformLocation(program, "Id"), 1, light.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Is"), 1, light.specular);

	// setup material properties
	glUniform4fv(glGetUniformLocation(program, "Ka"), 1, material.ambient);
	glUniform4fv(glGetUniformLocation(program, "Kd"), 1, material.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Ks"), 1, material.specular);
	glUniform1f(glGetUniformLocation(program, "shininess"), material.shininess);

}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// notify to GL that we like to use our program now
	glUseProgram(program);

	//*****************************************************World-Walls*********************************************************
	glBindBuffer(GL_ARRAY_BUFFER, worldWall.getMesh()->vertexBuffer);
	// bind vertex position buffer
	GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	GLuint vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	GLuint vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, worldWall.getImageWidth(), worldWall.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, worldWall.getImage());

	for (int k = 1, w = worldWall.getImageWidth() >> 1, h = worldWall.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	//for (int i = 0; i < num_Walls; i += 2){
	//	//back and Front wall
	//	if (i == 0){
	//		//front
	//		mat4 modelMatrix = mat4::identity();
	//		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//		//	modelMatrix = mat4::rotate(vec3(0, 1, 0), (1 / 2.0 * PI));
	//		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y, wall.getPosition().z) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), wall.getXRotation())*modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(1, 0, 0), wall.getYRotation())*modelMatrix;
	//		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//		glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());

	//		//back
	//		modelMatrix = mat4::identity();
	//		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), (PI));
	//		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y, wall.getPosition().z + 10) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), wall.getXRotation())*modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(1, 0, 0), wall.getYRotation())*modelMatrix;
	//		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//		glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());


	//	}
	//	//left and right wall
	//	else if (i == 2){
	//		//right
	//		mat4 modelMatrix = mat4::identity();
	//		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), (0.5 * PI));
	//		modelMatrix = mat4::translate(wall.getPosition().x + 5, wall.getPosition().y, wall.getPosition().z + 5) * modelMatrix;

	//		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//		glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());

	//		//left
	//		modelMatrix = mat4::identity();
	//		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
	//		modelMatrix = mat4::translate(wall.getPosition().x - 5, wall.getPosition().y, wall.getPosition().z + 5) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), wall.getXRotation())*modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(1, 0, 0), wall.getYRotation())*modelMatrix;
	//		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//		glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());

	//	}
	//	//bottom and seiling
	//	else if (i == 4){
	//		//bottom
	//		mat4 modelMatrix = mat4::identity();
	//		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(1, 0, 0), (0.5 * PI));
	//		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y - 5, wall.getPosition().z + 5) * modelMatrix;

	//		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//		glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());

	//		//seiling
	//		modelMatrix = mat4::identity();
	//		modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), -(0.5 * PI));
	//		modelMatrix = mat4::translate(wall.getPosition().x, wall.getPosition().y - 5, wall.getPosition().z + 5) * modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(0, 1, 0), wall.getXRotation())*modelMatrix;
	//		modelMatrix = mat4::rotate(vec3(1, 0, 0), wall.getYRotation())*modelMatrix;


	//		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//		glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());
	//	}
	//	/*mat4 modelMatrix = mat4::identity();
	//	modelMatrix = mat4::scale(wall.getScale(), wall.getScale(), wall.getScale()) * modelMatrix;
	//	modelMatrix = mat4::rotate(vec3(0,1,0),(1/2.0 * PI));
	//	modelMatrix = mat4::translate(wall.getPosition().x + 5, wall.getPosition().y, wall.getPosition().z + 5) * modelMatrix;

	//	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	//	glDrawArrays(GL_TRIANGLES, 0, wall.getMesh()->vertexList.size());
	//	*/
	//}
	int numWalls = world->getNum_WorldWalls();
	std::vector<mat4> worldWalls = world->getWorldWalls();
	for (int i = 0; i < numWalls; i++){

			glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, worldWalls[i]);

			glDrawArrays(GL_TRIANGLES, 0, worldWall.getMesh()->vertexList.size());
	}

	//*****************************************************BOX*********************************************************
	glBindBuffer(GL_ARRAY_BUFFER, box.getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, box.getImageWidth(), box.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, box.getImage());

	for (int k = 1, w = box.getImageWidth() >> 1, h = box.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	for (int i = 0; i < num_boxes; i++){
		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(box.getScale(), box.getScale(), box.getScale()) * modelMatrix;
		modelMatrix = mat4::translate(box.getPosition().x+i, box.getPosition().y+i, box.getPosition().z+i) * modelMatrix;
		modelMatrix = mat4::rotate(vec3(0, 1, 0), box.getXRotation())*modelMatrix;
		modelMatrix = mat4::rotate(vec3(1, 0, 0), box.getYRotation())*modelMatrix;

		btCollisionShape* boxShape = new btBoxShape(btVector3(box.getPosition().x, box.getPosition().y, box.getPosition().z));

		btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
			btQuaternion(0, 0, 0, 1),
			btVector3(box.getPosition().x + i, box.getPosition().y + i, box.getPosition().z + i)
			));

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass, in kg. 0 -> Static object, will never move.
			motionstate,
			boxShape,  // collision shape of body
			btVector3(0, 0, 0)    // local inertia
			);
		btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);
		dynamicsWorld->addRigidBody(rigidBody);

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, box.getMesh()->vertexList.size());
	}

	//*****************************************************ENEMY**********************************************************************************

	glBindBuffer(GL_ARRAY_BUFFER, enemy.getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, enemy.getImageWidth(), enemy.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, enemy.getImage());

	for (int k = 1, w = enemy.getImageWidth() >> 1, h = enemy.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	for (int i = 0; i < num_enemyes; i++){
		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(1, 1, enemy.getScale()) * modelMatrix;
		modelMatrix = mat4::translate(enemy.getPosition().x + i, enemy.getPosition().y + i, enemy.getPosition().z + i) * modelMatrix;

		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, enemy.getMesh()->vertexList.size());
	}

	//*******************************************AK***********************************************************************

	// bind vertex position buffer
	glBindBuffer(GL_ARRAY_BUFFER, ak.getMesh()->vertexBuffer);

	// bind vertex position buffer
	vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	glTexImage2D(GL_TEXTURE_2D, 0, 3, ak.getImageWidth(), ak.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, ak.getImage());

	for (int k = 1, w = ak.getImageWidth() >> 1, h = ak.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	float gunRotateX = -ak.getDirection().y;
	float gunRotateY = -ak.getDirection().x;

	mat4 modelMatrix = mat4::identity();
	//
	modelMatrix = mat4::rotate(vec3(0, 0, 1), gunRotateY)*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), gunRotateX)*modelMatrix;
	modelMatrix = mat4::rotate(vec3(0, 0, 1), PI)*modelMatrix;
	modelMatrix = mat4::rotate(vec3(1, 0, 0), -PI / 2)*modelMatrix;
	modelMatrix = mat4::translate(-camera.at) * modelMatrix;
	modelMatrix = mat4::scale(ak.getScale(), ak.getScale(), ak.getScale()) * modelMatrix;
	//modelMatrix = mat4::translate(camera.at) * modelMatrix; 
	modelMatrix = mat4::translate(ak.getPosition().x, ak.getPosition().y, ak.getPosition().z) * modelMatrix;


	btCollisionShape* boxShape = new btBoxShape(btVector3(ak.getPosition().x, ak.getPosition().y, ak.getPosition().z));

	btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
		btQuaternion(0, 0, 0, 1),
		btVector3(ak.getPosition().x, ak.getPosition().y, ak.getPosition().z)
		));

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		0,                  // mass, in kg. 0 -> Static object, will never move.
		motionstate,
		boxShape,  // collision shape of body
		btVector3(0, 0, 0)    // local inertia
		);
	btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBody);


	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, ak.getMesh()->vertexList.size());

	// now swap backbuffer with front buffer, and display it
	glutSwapBuffers();
	// increment FRAME index
	frame++;


}
void worldInit(){
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void display()
{
	update();
	render();
}

void reshape(int width, int height)
{
	// set current viewport in pixels
	// viewport: the window area that are affected by rendering
	// (win_x, win_y, win_width, win_height)
	glViewport(0, 0, windowWidth = width, windowHeight = height);

	camera.aspectRatio = float(width) / float(height);

	// post signal to call display
	// this causes GL to call display() soon
	// but, we do not know exactly when dipslay() is called
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON){
		//PlaySound((LPCWSTR)"../bin/Sounds/Shoot.wav", NULL, SND_FILENAME);
	}
}

void motion(int x, int y)
{
	static bool just_warped = false;

	if (just_warped) {
		just_warped = false;
		return;
	}

		int dx = x - windowWidth / 2;
		int dy = y - windowHeight / 2;

		if (dx) {


			box.setXRotation(((float)dx / 1000)+box.getXRotation());
			/*std::vector<mat4> wWalls = world->getWorldWalls();
*/
			//worldWall.setXRotation(((float)dx / 1000) + worldWall.getXRotation());
			Wall woWall = world->getWall();
			woWall.setXRotation(((float)dx / 1000) + woWall.getXRotation());
			world->setWorldWalls(worldWall);
		}

		if (dy) {
			box.setYRotation(((float)dy / 1000) + box.getYRotation());
			//worldWall.setYRotation(((float)dy / 1000) + worldWall.getYRotation());
			//world->setWorldWalls(worldWall);
			Wall woWall = world->getWall();
			woWall.setYRotation(((float)dy / 1000) + woWall.getYRotation());
			world->setWorldWalls(worldWall);
		}

		glutWarpPointer(windowWidth / 2, windowHeight / 2);

		just_warped = true;
	


}

void move(int key, int x, int y)
{

}

void idle()
{
	glutPostRedisplay();	// signal to call display soon
}

void keyboard(unsigned char key, int x, int y)
{

	vec4 a = mat4(cos((2 * PI - box.getXRotation())), 0, (sin((2 * PI - box.getXRotation()))), 0.f, 0, 1, 0.f, 0.f, sin((2 * PI - box.getXRotation())), 0.f, cos((2 * PI - box.getXRotation())), 0.f, 0.f, 0.f, 0.f, 1.f).operator*(vec4(0, 0, 1, 0));
	vec4 b = mat4(cos((box.getXRotation())), 0, (sin((box.getXRotation()))), 0.f, 0, 1, 0.f, 0.f, sin((box.getXRotation())), 0.f, cos((box.getXRotation())), 0.f, 0.f, 0.f, 0.f, 1.f).operator*(vec4(1, 0, 0, 0));

	
	
	if (key == 'w' || key == 'W' ){
		for (int i = 1; i < 2; i++){
			box.setPosition(box.getPosition() - b*stepSize);
			worldWall.setPosition(worldWall.getPosition() - b*stepSize);
			world->setWorldWalls(worldWall);
		}
	}
	else if (key == 's' || key == 'S'){
		for (int i = 1; i < 2; i++){
			box.setPosition(box.getPosition() - a*stepSize);
			worldWall.setPosition(worldWall.getPosition() - a*stepSize);
			world->setWorldWalls(worldWall);

		}
	}
	else if (key == 'a' || key == 'A'){
		for (int i = 1; i < 2; i++){
			box.setPosition(box.getPosition() + b*stepSize);
			worldWall.setPosition(worldWall.getPosition() + b*stepSize);
			world->setWorldWalls(worldWall);

		}
	}
	else if (key == 'd' || key == 'D'){
		for (int i = 1; i < 2; i++){
			box.setPosition(vec4(box.getPosition().x - stepSize, box.getPosition().y, box.getPosition().z, box.getPosition().w));
			worldWall.setPosition(vec4(worldWall.getPosition().x - stepSize, worldWall.getPosition().y, worldWall.getPosition().z, worldWall.getPosition().w));
			world->setWorldWalls(worldWall);
		}
	}
	else if (key == 27){
		exit(0);
	}
}

bool initShaders(const char* vertShaderPath, const char* fragShaderPath)
{
	// create a program before linking shaders
	program = glCreateProgram();
	glUseProgram(program);

	// compile shader sources
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = readShader(vertShaderPath); if (vertexShaderSource == NULL) return false;
	GLint vertexShaderLength = strlen(vertexShaderSource);
	glShaderSource(vertexShader, 1, &vertexShaderSource, &vertexShaderLength);
	glCompileShader(vertexShader);
	if (!checkShader(vertexShader, "vertexShader")){ printf("Unable to compile vertex shader\n"); return false; }
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = readShader(fragShaderPath); if (fragmentShaderSource == NULL) return false;
	GLint fragmentShaderLength = strlen(fragmentShaderSource);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, &fragmentShaderLength);
	glCompileShader(fragmentShader);
	if (!checkShader(fragmentShader, "fragmentShader")){ printf("Unable to compile fragment shader\n"); return false; }
	// attach vertex/fragments shaders and link program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	if (!checkProgram(program, "program")){ printf("Unable to link program\n"); return false; }

	// deallocate string
	free((void*)vertexShaderSource);
	free((void*)fragmentShaderSource);

	return true;
}

bool userInit()
{
	wall = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wallBrown.jpg", NULL);
	worldWall = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);

	box = Box(1.f, vec3(0.f, -2.f, -5.f), "../bin/Images/Box.jpg", NULL);
	enemy = Enemy(0.1f, vec3(-2.f, -2.f, -8.f), "../bin/Images/Enemy.jpg", NULL);
	ak = AK(0.006f, vec3(0, 0, 0), "../bin/Images/tex_AK.jpg", "../bin/Mods/AK.obj");

	world = new World(wall, enemy, worldWall);
	worldInit();
	world->setWorldWalls(worldWall);

	// create a vertex buffer
	glGenBuffers(1, &worldWall.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, worldWall.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, worldWall.getMesh()->vertexList.size()*sizeof(vertex), &worldWall.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &wall.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, wall.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, wall.getMesh()->vertexList.size()*sizeof(vertex), &wall.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ak.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ak.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, ak.getMesh()->vertexList.size()*sizeof(vertex), &ak.getMesh()->vertexList[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &box.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, box.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, box.getMesh()->vertexList.size()*sizeof(vertex), &box.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &enemy.getMesh()->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, enemy.getMesh()->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, enemy.getMesh()->vertexList.size()*sizeof(vertex), &enemy.getMesh()->vertexList[0], GL_STATIC_DRAW);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	/*Map.resize(num_Walls*sizeof(Wall));
	Map[0] = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);
	Map[1] = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);
	Map[2] = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);
	Map[3] = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);
	Map[4] = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);
	Map[5] = Wall(10.f, vec3(0.f, 2.f, -5.f), "../bin/Images/wall_texture.jpg", NULL);*/



	objects.resize(2 * sizeof(Object));
	objects[0] = ak;
	objects[1] = box;
	objects[2] = wall;
	objects[3] = worldWall;

	// init camera
	camera.eye = vec3(0, 0.2f, 1);
	camera.at = vec3(0, 0, 0);
	camera.up = vec3(0, 1, 0);
	camera.viewMatrix = mat4::lookAt(camera.eye, camera.at, camera.up);

	// init light properties
	light.position = vec4(0.0f, 0.2f, 1.0f, 0.0f);   // directional light
	light.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	light.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// init material properties
	material.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	material.diffuse = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	material.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	material.shininess = 100.0f;

	return true;
}

int main(int argc, char* argv[])
{

	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	windowWidth = 1280;
	windowHeight = 720;
	
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
	glutCreateWindow("FPS");
	glutSetCursor(GLUT_CURSOR_NONE);
	glutFullScreen();
	glutWarpPointer(windowHeight / 2, windowWidth / 2);

	// Register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);		// callback when the window is resized
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);		// callback when the mouse is moving
	glutIdleFunc(idle);			// idle-time callback

	// init and check GLEW, version, extensions
	if (!initExtensions()){ printf("Failed to init extensions.\n"); return 0; }
	// create and compile shaders/program
	if (!initShaders("shaders/trackball.vert", "shaders/trackball.frag")){ printf("Failed to init program and shaders\n"); return 0; }
	// user initialization
	if (!userInit()){ printf("Failed to userInit()\n"); return 0; }
	// Start rendering loop
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutMainLoop();
	return 0;
}
