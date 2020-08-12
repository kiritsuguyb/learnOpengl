#include <iostream>
#include <glad.h>
#include <glfw3.h>
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
	//��һ�����ǳ�ʼ��context�����ݱ����������ʹ�õ�opengl�汾
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//һЩǰ��hint�趨���ˣ��ͽ���window������װ��current context�����ÿ��window����context
	//������glfwMakeCurrentContext�����������������ڵ��������ĸ�window
	GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 800);

	//�ڿ�ʼloop֮ǰ�趨��callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//׼���׶�
	//1.׼����������
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
	//׼��һ��VAO
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

	//2׼��vertexAttributePointer
	//����ÿ��vertexAttribute��Ϊ�˷��������opengl��Ҫ֪��
	//1.�������ڱ����ĸ�����(index)
	//2.������Դ��Ŀ�ʼ���ҵ�һ��ȥ����(pointer)
	//3.������Զ��һ���Զ��೤���ڴ�(size)
	//4.������Ա�����ʲô��������ڴ浱��ʲô����(type)
	//5.����������ڴ������������󣬷�����ֱ��������һ���ڴ��ȡ(stride)
	//6.��������Ƿ񱻹�һ������Ҫ��Ҫֱ����������ݻ����Լ�����һ��
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//2.׼��shader��shader program
	//2.1׼��vertexshader
	unsigned int vertexShader;
	vertexShader=glCreateShader(GL_VERTEX_SHADER);
	//������������������� const char* const*,��˼��һ��ָ���ַ�������ָ��a��ָ��b
	//��һ��const��ʾָ��aָ����ַ����������ɸ���
	//�ڶ���const��ʾָ��bָ���ָ��a���ɸ���
	//���Ը��ĵ���ָ��b������ʹ��ָ����һ���ַ�������ָ��
	//���Ҫ����ָ��b��ָ��Ӧ�����ӵ�����const���η�������
	//const char* const* const
	//ע�⣬const char* const*ʵ���Ͼ��� char **,�ַ�ָ���ָ��
	//���vertexshadersource�Ѿ����ַ�ָ�룬ֻ��Ҫ��ȡһ�����ü��ɡ�
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

	//2.2׼��fragmentShader
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

	//2.3׼��shaderProgram
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
