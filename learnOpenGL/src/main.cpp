
/*д���ʼ��һ������
opengl����һ���೦�ӹ�����
��������Ҫ��������ָ����Ĺ೦����ʦ���ˡ�
����Ϊ��׼����Щ�೦����Ҫ�ܶಽ�衣
��������Ҫ׼���೦����������浽����װ�����⡢ţ�⡢���⻹��Ѽ�⡣
�������Ҫ׼��װ��Щ�����ĳ���
Ȼ�����ܲ�����ͽ�ְ���Щϡϡ������������װ��һ���������
����Ҫһ��©�����ڹ�������Щ©�����ǻ�����װ�õ�©����
��ô����ͺ���ȷ
1.׼�������������vertices attribute��
2.׼��һ�����£�glGenBuffer��
3.�ѳ��¹��ڻ�����©���ϣ�glBindBuffer��
4.�������ﶼ���������Ķ�Ӧ©����(glBufferData)
5.�ѹ೦�͵���һ���ӹ���

opengl�д��ڼ�������㣬���ϵ��·ֱ���
context/window��
state machine with locations, 
objects
������Ҫ����context��context����һ��window�������window��������һ����statemachine
���״̬��������һ���locations����Щlocation������������ݣ���ʲô����أ���objects��
ÿ��objects�ֻ���һЩ���ԡ�
object���Ե����ݴ�������
һ��ʼ��cpu�˶���
���ǵ���Ҫ����ʱ������Ӧ�ð���Щobject���ݹ���GPU buffer��
�����Ҫ��GPU����һ��buffer�����
��ô��ι�����
����buffer������˵��opengl�Ѿ�֪�����кܶ����ݽ��������Ԥ�����˺ܶ�������ڣ���target
Ϊ����buffer�������Ϣ��������Ҫ��buffer�󶨵�openGL�ڶ����target�е�һ��
�����target�й������ݣ��Ϳ����ˡ�

//����һ��buffer��������buffer��id
//�����id��bufferע��󶨵�ĳ��OpenGL��ֱ��handle��location/target�ϡ�
//ֱ���趨location/target�����ԣ��Ϳ��԰�object��Ϣͨ�����target����buffer�С�

����shader��˵��Ҳ��һ���ģ�����Ҫ����opengl������һ��shader������object���ݣ���
Ϊ������Ҫ����һ��shader��������Ȼ�������������װ��Դ���롣
������̲�����Ҫʹ��opengl��target
ֱ�ӱ������Դ���룬���ܵȴ�ʹ�á�
shader�൱��һ���Ѿ������õĹ೦�����þ���©����ֱ���͸���һ���Ϳ��ԡ�
*/
#include <iostream>
#include <glad.h>
#include <glfw3.h>
#include "Shader.h"
#include "stb_image.h"
#include "Texture.h"
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "VAO.h"
#include "Light.h"

#define PI 3.1415927f
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);
void mousebutton_callback(GLFWwindow*, int, int, int);
void processInput(GLFWwindow* window);
float deltaTime = 0.0f;
float lastTime = 0.0f;
bool firstMouse=true;
bool rightButtonPressed;
double lastX = 0, lastY = 0, xoffset = 0, yoffset = 0.0,yaw=-90,pitch=0.0;
Camera* currentCam;
float lightIntensity = 1.0f;
float maxDistance = 32.0f;

int main(void)
{
	//��һ�����ǳ�ʼ��context�����ݱ����������ʹ�õ�opengl�汾
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	stbi_set_flip_vertically_on_load(true);
	//һЩǰ��hint�趨���ˣ��ͽ���window������װ��current context�����ÿ��window����context
	//������glfwMakeCurrentContext�����������������ڵ��������ĸ�window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//!*this should happen before initialize glad
	//�����context�����ʼ��glad����ȡopengl�ĺ�����
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//���崰������ͼ��С�����½Ǻ����Ͻǵ���Ļ����
	glViewport(0, 0, 800, 600);

	//�ڿ�ʼloop֮ǰ�趨��callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);

	//׼���任����

	glm::mat4 view(1.0f);

	Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
	currentCam = &cam;

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	//׼���׶�
	//layout
	VertexLayout layout;
	layout.push<float>(3u);
	layout.push<float>(3u);
	layout.push<float>(2u);
	//1.׼����������
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//unsigned int indices[] = {  // note that we start from 0!
	//	0,1, 2,  // first triangle
	//	2,3,0
	//};


	Shader lightShader("light.shader");
	//׼��һ��VAO
	VAO light(lightShader, layout);
	light.useShader().setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	light.bind();

	//׼��texture0
	Texture texture0("container2.png", GL_TEXTURE0);
	//׼��texture1
	Texture texture1("container2_specular.png", GL_TEXTURE1);

	Shader objectShader("object.shader");
	VAO liObject(objectShader, layout);
	liObject.useShader().setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	liObject.useShader().setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	liObject.useShader().setInt("material.diffuse", 0);
	liObject.useShader().setInt("material.specular",1);
	liObject.bind();
	/*
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);*/
/*
	glm::vec3 pos(-1.2f, -1.0f, -2.0f);
	glm::mat4 mod(1.0f);
	mod = glm::translate(mod, pos);

	auto TImod = glm::transpose(glm::inverse(mod));
	auto itmod = glm::inverse(glm::transpose(mod));
	std::cout << "mod matrix!!:\n";
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			std::cout << mod[i][j] << "  ";
		}
		std::cout << "\n";

	}
	std::cout << "ITmod matrix!!:\n";
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << itmod[i][j] << "  ";
		}
		std::cout << "\n";

	}
	std::cout << "TImod matrix!!:\n";
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << TImod[i][j] << "  ";
		}
		std::cout << "\n";

	}*/
	while (!glfwWindowShouldClose(window)) {
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		//input
		processInput(window);

		//rendering command
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const float radius = 2.0f;
		auto x = sin(5*glfwGetTime())*radius;
		//auto y = sin(1.1*glfwGetTime() + PI / 4)*radius;
		auto z = cos(5*glfwGetTime())*radius;

		//cam.PlaceAt(glm::vec3(x, y, z));*/
		//�������λ��
		view = currentCam->getViewMatrix();
		//������Դ
		//��Դλ��
		glm::vec3 lightColor(1.0f,1.0f,1.0f);
		/*lightColor.x = sin(glfwGetTime() * 1.0f)*0.5f + 0.5f;
		lightColor.y = sin(glfwGetTime() * 0.4f)*0.5f + 0.5f;
		lightColor.z = sin(glfwGetTime() * 0.7f)*0.5f + 0.5f;*/
		auto diffuse = lightColor * 0.5f;
		auto ambient = lightColor * 0.2f;
		glm::vec3 lightPos(x, 1.5f, z);
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		//ʹ�ù�Դ����
		light.bind();
		//���øö���shader����
		light.useShader().setMatrix("model", glm::value_ptr(lightModel));
		light.useShader().setMatrix("view", glm::value_ptr(view));
		light.useShader().setMatrix("projection", glm::value_ptr(projection));
		light.useShader().setVec3("lightColor",glm::value_ptr(lightColor*lightIntensity) );
		//drawcall����
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//��������
		//λ��
		//ʹ���������
		liObject.bind();
		//���øö���shader����
		liObject.useShader().setMatrix("view", glm::value_ptr(view));
		liObject.useShader().setMatrix("projection", glm::value_ptr(projection));
		liObject.useShader().setVec3("viewPos", glm::value_ptr(currentCam->getPos()));
		liObject.useShader().setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		liObject.useShader().setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		liObject.useShader().setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		liObject.useShader().setFloat("material.shininess", 128.0f);
		liObject.useShader().setVec3("light.position", glm::value_ptr(lightPos));
		//Light_Spot light(currentCam->getPos(),currentCam->getForward(),ambient,diffuse,12.0,20.0,maxDistance,0.01f, lightIntensity);
		Light_Point light(lightPos,ambient, diffuse,maxDistance, 0.01f, lightIntensity);
		light.applyToShader(liObject.useShader());
		for (int i=0;i<10;i++)
		{
			glm::mat4 objectModel(1.0f);
			float angle = glfwGetTime() * 10.0f*(i + 1);
			//objectModel = glm::rotate(objectModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			objectModel = glm::translate(objectModel, cubePositions[i]);
			liObject.useShader().setMatrix("model", glm::value_ptr(objectModel));
			auto itobjectmodel = glm::inverse(glm::transpose(objectModel));
			liObject.useShader().setMatrix("itmodel", glm::value_ptr(itobjectmodel));
			//drawcall����
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		


		//check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "windows size changing" << std::endl;
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (rightButtonPressed)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		xoffset = xpos - lastX;
		yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.35f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		currentCam->LookAtDir(direction);
		currentCam->Up(0.0f, 1.0f, 0.0f);
	}
}

void mousebutton_callback(GLFWwindow * window, int button, int action, int mods)
{
	rightButtonPressed = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
	firstMouse = rightButtonPressed ? firstMouse : true;
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	bool shiftPress = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
	float cameraSpeed = (shiftPress?5.0:2.5f)*deltaTime;
	if (glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
	{
		currentCam->Translate(currentCam->forward()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
	{
		currentCam->Translate(-currentCam->forward()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		currentCam->Translate(-currentCam->right()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		currentCam->Translate(currentCam->right()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		lightIntensity += 0.1f;
		std::cout << "intensity:  " << lightIntensity << "\n";
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		lightIntensity -= 0.1f;
		lightIntensity = lightIntensity > 0 ? lightIntensity : 0;
		std::cout << "intensity:  " << lightIntensity << "\n";
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		maxDistance += 2.0f;
		std::cout << "maxDistance:  " << maxDistance << "\n";
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		maxDistance -= 2.0f;
		maxDistance = maxDistance > 0 ? maxDistance : 0;
		std::cout << "maxDistance:  " << maxDistance << "\n";
	}
}
