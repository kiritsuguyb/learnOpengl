
/*写在最开始的一个比喻
opengl就是一个灌肠加工厂。
最终你需要输出许多各种各样的灌肠给厨师做菜。
但是为了准备这些灌肠，需要很多步骤。
首先你需要准备灌肠的内容物，里面到底是装填猪肉、牛肉、鸡肉还是鸭肉。
其次你需要准备装这些东西的肠衣
然后你总不可能徒手把这些稀稀拉拉的内容物装进一个肠衣里吧
你需要一个漏斗，在工厂里这些漏斗都是机器上装好的漏斗。
那么步骤就很明确
1.准备好所有内容物（vertices attribute）
2.准备一个肠衣（glGenBuffer）
3.把肠衣挂在机器的漏斗上（glBindBuffer）
4.把内容物都倒进机器的对应漏斗里(glBufferData)
5.把灌肠送到下一步加工。

opengl中存在几个抽象层，从上到下分别是
context/window，
state machine with locations, 
objects
首先你要建立context，context就是一个window，在这个window内下属了一整个statemachine
这个状态机又下属一大堆locations，这些location能用来填充数据，用什么填充呢，用objects。
每个objects又会有一些属性。
object属性的数据存放在哪里？
一开始由cpu端定义
但是当需要绘制时，首先应该把这些object数据灌入GPU buffer中
因此需要在GPU申请一个buffer来存放
那么如何灌入呢
对于buffer数据来说，opengl已经知道会有很多数据进来，因此预定义了很多数据入口，即target
为了向buffer里灌入信息，我们需要把buffer绑定到openGL众多操作target中的一个
向这个target中灌入数据，就可以了。

//申请一个buffer，获得这个buffer的id
//用这个id把buffer注册绑定到某个OpenGL能直接handle的location/target上。
//直接设定location/target的属性，就可以把object信息通过这个target填入buffer中。

对于shader来说，也是一样的，你先要告诉opengl这里有一个shader（就像object数据）。
为此你需要创建一个shader的容器，然后往这个容器里装填源代码。
这个过程并不需要使用opengl的target
直接编译这个源代码，就能等待使用。
shader相当于一个已经生产好的灌肠，不用经过漏斗，直接送给下一步就可以。
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
	//第一步就是初始化context，根据本机的情况和使用的opengl版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	stbi_set_flip_vertically_on_load(true);
	//一些前提hint设定好了，就建立window，用来装到current context里，所以每个window都是context
	//但是用glfwMakeCurrentContext函数来决定我们现在调整的是哪个window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//!*this should happen before initialize glad
	//在这个context下面初始化glad，拉取opengl的函数。
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//定义窗口内视图大小，左下角和右上角的屏幕坐标
	glViewport(0, 0, 800, 600);

	//在开始loop之前设定好callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);

	//准备变换矩阵

	glm::mat4 view(1.0f);

	Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
	currentCam = &cam;

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	//准备阶段
	//layout
	VertexLayout layout;
	layout.push<float>(3u);
	layout.push<float>(3u);
	layout.push<float>(2u);
	//1.准备顶点数据
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
	//准备一个VAO
	VAO light(lightShader, layout);
	light.useShader().setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	light.bind();

	//准备texture0
	Texture texture0("container2.png", GL_TEXTURE0);
	//准备texture1
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
		//更新相机位置
		view = currentCam->getViewMatrix();
		//画出光源
		//光源位置
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
		//使用光源对象
		light.bind();
		//设置该对象shader数据
		light.useShader().setMatrix("model", glm::value_ptr(lightModel));
		light.useShader().setMatrix("view", glm::value_ptr(view));
		light.useShader().setMatrix("projection", glm::value_ptr(projection));
		light.useShader().setVec3("lightColor",glm::value_ptr(lightColor*lightIntensity) );
		//drawcall绘制
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//画出物体
		//位置
		//使用物体对象
		liObject.bind();
		//设置该对象shader数据
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
			//drawcall绘制
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
