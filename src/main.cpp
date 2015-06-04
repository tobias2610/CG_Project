#include "support.h"
#include "cgmath.h"		// slee's simple math library
#include "trackball.h"
#include "mesh.h"
#include "light.h"
#include "AK.h"
#include "Box.h"
#include "Enemy.h"
#define stepSize 0.05f
#define num_boxes 3
#define num_enemyes 1
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
Enemy		enemy;
Trackball	trackball(camera.viewMatrix, 1.0f);
GLuint		textureObject = 0;
Light		light;
Material	material;
std::vector<Object> objects;
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

	//*****************************************************BOX*********************************************************
	glBindBuffer(GL_ARRAY_BUFFER, box.getMesh()->vertexBuffer);

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

	glTexImage2D(GL_TEXTURE_2D, 0, 3, box.getImageWidth(), box.getImageHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, box.getImage());

	for (int k = 1, w = box.getImageWidth() >> 1, h = box.getImageHeight() >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	for (int i = 0; i < num_boxes; i++){
		mat4 modelMatrix = mat4::identity();
		modelMatrix = mat4::scale(box.getScale(), box.getScale(), box.getScale()) * modelMatrix;
		modelMatrix = mat4::translate(box.getPosition().x+i, box.getPosition().y+i, box.getPosition().z+i) * modelMatrix;

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

	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, ak.getMesh()->vertexList.size());

	// now swap backbuffer with front buffer, and display it
	glutSwapBuffers();
	// increment FRAME index
	frame++;


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
		
	}
}

void motion(int x, int y)
{
	camera.at = vec3((x - windowWidth / 2) / float(windowWidth - 1), -(y - windowHeight / 2) / float(windowHeight - 1), camera.at.z);
	ak.setDirection(vec3((x - windowWidth / 2) / float(windowWidth - 1), -(y - windowHeight / 2) / float(windowHeight - 1), ak.getDirection().z));
	ak.setPosition(vec3((x - windowWidth / 2) / float(windowWidth - 1), -(y - windowHeight / 2) / float(windowHeight - 1), ak.getPosition().z));
	
	
	camera.viewMatrix = mat4::lookAt(camera.eye, camera.at, camera.up);
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
	if (key == 'w' || key == 'W' ){
		for (int i = 1; i < 2; i++){
			box.setPosition(vec3(box.getPosition().x, box.getPosition().y, box.getPosition().z + stepSize));
		}
	}
	else if (key == 's' || key == 'S'){
		for (int i = 1; i < 2; i++){
			box.setPosition(vec3(box.getPosition().x, box.getPosition().y, box.getPosition().z - stepSize));
		}
	}
	else if (key == 'a' || key == 'A'){
		for (int i = 1; i < 2; i++){
			box.setPosition(vec3(box.getPosition().x + stepSize, box.getPosition().y, box.getPosition().z));
		}
	}
	else if (key == 'd' || key == 'D'){
		for (int i = 1; i < 2; i++){
			box.setPosition(vec3(box.getPosition().x - stepSize, box.getPosition().y, box.getPosition().z));
		}
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

	box = Box(1.f, vec3(0.f, 0.f, -5.f), "../bin/Images/Box.jpg", NULL);
	enemy = Enemy(0.1f, vec3(-2.f, -2.f, -8.f), "../bin/Images/Enemy.jpg", NULL);
	ak = AK(0.006f, vec3(0, 0, 0), "../bin/Images/tex_AK.jpg", "../bin/Mods/AK.obj");


	// create a vertex buffer
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

	objects.resize(2 * sizeof(Object));
	objects[0] = ak;
	objects[1] = box;

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
