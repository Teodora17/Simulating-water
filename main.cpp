#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void processKeyboardInput ();

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float random = 1;
float timer=1;
float timer2 = 1;
float amplitude = 1.75;
glm::vec2 direction = glm::vec2(0.7f,0.5f);
float frequency;
float length=80.1;
float phase;
float speed=84;
glm::mat4 trans;
glm::vec3 lightColor(0.0f, 0.0f, 0.9f);



bool ok = true; //test mouse movement

float lastX = 0.0f;
float lastY = 0.0f;

Window window("Game Engine", 800, 800);
Camera camera;


glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, -200.0f);

int main()
{
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader carShader("Shaders/car_vertex_shader.glsl", "Shaders/car_frag_shader.glsl");
	
	//Textures
	GLuint tex = loadBMP("Resources/Textures/wood.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/water.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/orange.bmp");
	GLuint tex4 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex5 = loadBMP("Resources/Textures/wood.bmp");

	glEnable(GL_DEPTH_TEST);

	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(1.0f, 1.0f);

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(1.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 1.0f);

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3,   
		1, 2, 3 };

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures2;
	textures2.push_back(Texture());
	textures2[0].id = tex2;
	textures2[0].type = "texture_diffuse";

	std::vector<Texture> textures3;
	textures3.push_back(Texture());
	textures3[0].id = tex3;
	textures3[0].type = "texture_diffuse";

	std::vector<Texture> textures4;
	textures4.push_back(Texture());
	textures4[0].id = tex4;
	textures4[0].type = "texture_diffuse";

	std::vector<Texture> textures5;
	textures5.push_back(Texture());
	textures5[0].id = tex5;
	textures5[0].type = "texture_diffuse";

	
	Mesh mesh(vert, ind, textures3);

	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh plane = loader.loadObj("Resources/Models/plane1.obj", textures2);
	Mesh car = loader.loadObj("Resources/Models/car.obj");
	frequency = 2 * 3.1415 / length;
	phase = speed * 2 * 3.1415 / length;
	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		lastX = window.getWidth() / 2.0f;
		lastY = window.getHeight() / 2.0f;
		timer2 = timer2 + 0.002;
		timer = timer + 0.00034;
		trans = glm::translate(trans, glm::vec3((float)glfwGetTime() * 0.002, 0.0, 0.0));
		
		processKeyboardInput();

		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressing mouse button" << std::endl;
			lightPos= glm::vec3(-480.0f, 100.0f, -200.0f);
			lightColor = glm::vec3(sin(11.6-timer2)*sin(11.6-timer2)/2+0.2f,sin(timer2)*sin(timer2)/2+0.3f, cos(timer2)*cos(timer2) / 2+0.45f );
		}
		 //// Code for the light ////
		
		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");

		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix*trans;
		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);

		//// Ending code for the light ////

		shader.use();

		///// Test Obj files with color of the light ////

		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");
		GLuint Ttimer = glGetUniformLocation(shader.getId(), "timer");
		GLuint Amp = glGetUniformLocation(shader.getId(), "amplitude");
		GLuint Direction = glGetUniformLocation(shader.getId(), "direction");
		GLuint Freq = glGetUniformLocation(shader.getId(), "frequency");
		GLuint Phase = glGetUniformLocation(shader.getId(), "phase");
		//GLuint Lightcucc = glGetUniformLocation(shader.getId(), "lightColor");

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		glUniform1f(glGetUniformLocation(shader.getId(), "timer"),timer);
		glUniform1f(glGetUniformLocation(shader.getId(), "amplitude"), amplitude);
		glUniform2f(glGetUniformLocation(shader.getId(), "direction"), direction.x,direction.y);
		glUniform1f(glGetUniformLocation(shader.getId(), "frequency"), frequency);
		glUniform1f(glGetUniformLocation(shader.getId(), "phase"), phase);
		

		//box.draw(shader);

		///// Test water Obj file //////

	/*	ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);*/

		plane.draw(shader);
		
		carShader.use();
		GLuint MatrixID3 = glGetUniformLocation(carShader.getId(), "MVP1");
		GLuint ModelMatrixID2 = glGetUniformLocation(carShader.getId(), "model");
		glm::mat4 ModelMatrix1 = glm::mat4(1.0);
		ModelMatrix1 = glm::translate(ModelMatrix1, glm::vec3(15.0f, 10.0f, 0.0f));
		ModelMatrix1 = glm::scale(ModelMatrix1, glm::vec3(15.0f));
		glm::mat4 MVP1 = ProjectionMatrix * ViewMatrix * ModelMatrix1;
		
		MVP1 = ProjectionMatrix * ViewMatrix * ModelMatrix1;
		glUniformMatrix4fv(MatrixID3, 1, GL_FALSE, &MVP1[0][0]);
		glUniformMatrix4fv(ModelMatrixID2, 1, GL_FALSE, &ModelMatrix1[0][0]);

		car.draw(carShader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(15.0f, -10.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(15.0));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		//rock2.draw(shader);

		window.update();
	}
}

void processKeyboardInput()
{
	float cameraSpeed = 150 * deltaTime;

	//translation
	if (window.isPressed(GLFW_KEY_W))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_S))
		camera.keyboardMoveBack(cameraSpeed);
	if (window.isPressed(GLFW_KEY_A))
		camera.keyboardMoveLeft(cameraSpeed);
	if (window.isPressed(GLFW_KEY_D))
		camera.keyboardMoveRight(cameraSpeed);
	if (window.isPressed(GLFW_KEY_R))
		camera.keyboardMoveUp(cameraSpeed);
	if (window.isPressed(GLFW_KEY_F))
		camera.keyboardMoveDown(cameraSpeed);

	//rotation
	if (window.isPressed(GLFW_KEY_LEFT))
		camera.rotateOy(cameraSpeed);
	if (window.isPressed(GLFW_KEY_RIGHT))
		camera.rotateOy(-cameraSpeed);
	if (window.isPressed(GLFW_KEY_UP))
		camera.rotateOx(cameraSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.rotateOx(-cameraSpeed);
}
