#include <iostream>
#include <glad.h>
#include <glfw3.h>
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

//Vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main(){\n"
"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor=vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow* window);
int main(void)
{
	//第一步就是初始化context，根据本机的情况和使用的opengl版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//一些前提hint设定好了，就建立window，用来装到current context里，所以每个window都是context
	//但是用glfwMakeCurrentContext函数来决定我们现在调整的是哪个window
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//!*this should happen before initialize glad
	//在这个context下面初始化glad，拉取opengl的函数。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//定义窗口内视图大小，左下角和右上角的屏幕坐标
	glViewport(0, 0, 800, 800);

	//在开始loop之前设定好callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//准备阶段
	//1.准备顶点数据
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,   // top left 
		1.0f,-0.5f,0.0f
	};
	unsigned int indices[] = {  // note that we start from 0!
	1,2, 3,   // first triangle
	0, 1, 4    // second triangle
	};
	//准备一个VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

	//2准备vertexAttributePointer
	//对于每个vertexAttribute，为了方便遍历，opengl需要知道
	//1.现在是在遍历哪个属性(index)
	//2.这个属性从哪开始，我第一个去哪里(pointer)
	//3.这个属性多大，一次性读多长的内存(size)
	//4.这个属性本质是什么，把这段内存当作什么看待(type)
	//5.这个属性在内存里两两间隔多大，方便我直接跳到下一个内存读取(stride)
	//6.这个属性是否被归一化，我要不要直接用这个数据还是自己处理一下
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//2.准备shader和shader program
	//2.1准备vertexshader
	unsigned int vertexShader;
	vertexShader=glCreateShader(GL_VERTEX_SHADER);
	//这里第三个参数类型是 const char* const*,意思是一个指向字符串常量指针a的指针b
	//第一个const表示指针a指向的字符串常量不可更改
	//第二个const表示指针b指向的指针a不可更改
	//可以更改的是指针b，可以使其指向另一个字符串常量指针
	//如果要限制指针b的指向，应该增加第三个const修饰符，如下
	//const char* const* const
	//注意，const char* const*实际上就是 char **,字符指针的指针
	//因此vertexshadersource已经是字符指针，只需要再取一次引用即可。
	glShaderSource(vertexShader, 1, &vertexShaderSource,NULL);
	glCompileShader(vertexShader);

	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	//2.2准备fragmentShader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	//2.3准备shaderProgram
	unsigned int shaderProgram=glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infolog << std::endl;
	}
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//rendering command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);

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

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
