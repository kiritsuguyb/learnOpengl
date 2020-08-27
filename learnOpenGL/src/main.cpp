
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
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
	currentCam = &cam;

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	//׼���׶�
	//layout
	VertexLayout layout;
	layout.push<float>(3u);
	layout.push<float>(2u);
	//1.׼����������
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

	//׼��texture0
	Texture texture0("container.jpg", GL_TEXTURE0);
	//׼��texture1
	Texture texture1("awesomeface.png", GL_TEXTURE1);

	Shader shaderProgram("shader.shader");
	//׼��һ��VAO
	VAO vao(shaderProgram,layout);
	vao.useShader().setInt("TEXTURE0", 0);
	vao.useShader().setInt("TEXTURE1", 1);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vao.bind();
	/*
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);*/
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
		const float radius = 10.0f;
		/*auto x = sin(1.3*glfwGetTime())*radius;
		auto y = sin(1.1*glfwGetTime() + PI / 4)*radius;
		auto z = cos(glfwGetTime())*radius;

		cam.PlaceAt(glm::vec3(x, y, z));*/
		//��ÿ��VAO�����л�
		view = currentCam->getViewMatrix();
		for (int i = 0; i < 10; i++) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime()* glm::radians((float)i+1)*20, glm::vec3(1.0f, 0.3f, 0.5f));
			shaderProgram.setMatrix("model", glm::value_ptr(model));
			shaderProgram.setMatrix("view", glm::value_ptr(view));
			shaderProgram.setMatrix("projection", glm::value_ptr(projection));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//


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
	if (glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS|| glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		currentCam->Translate(currentCam->forward()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		currentCam->Translate(-currentCam->forward()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		currentCam->Translate(-currentCam->right()*cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		currentCam->Translate(currentCam->right()*cameraSpeed);
	}
}
