#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
//顶点着色器（Vertex Shader） 向量（Vector）每个分量值表示空间的一个坐标。
/*const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n "// 位置变量的属性位置值为0
"layout (location = 1) in vec3 aColor;\n "// 位置变量的属性位置值为0
"layout (location = 2) in vec2 aTexCoord;\n "// 纹理属性位置值为0
//"out vec4 vertexColor;"//为片段着色器指定一个颜色输出
"out vec3 ourColor;"//向片段着色器输出一个颜色
"out vec2 TexCoord;"
"uniform float xOffset;"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x+xOffset, aPos.y, aPos.z, 1.0);\n"// add the xOffset to the x position of the vertex position
//"   gl_Position = vec4(aPos,1.0);\n"//注意我们如何把一个vec3作为vec4的构造器的参数
"	ourColor = aColor;\n"
"	TexCoord = vec2(aTexCoord.x,aTexCoord.y);\n"
//"   vertexColor = vec4(0.5,0.0,0.0,1.0);\n"// 把输出变量设置为暗红色
"}\0";
//片段着色器 4个元素是红色、绿色、蓝色和alpha（透明度）
/*const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";*/
/*const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"// 从顶点着色器传来的输入变量（名称相同、类型相同）
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";*/
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;"
"in vec2 TexCoord;"
"uniform sampler2D texture1;"
//"uniform vec4 ourColor;\n"// 在OpenGL程序代码中设定这个变量
"void main()\n"
"{\n"
"	FragColor = texture(texture1, TexCoord);"
//"   FragColor = ourColor;\n"
//"	FragColor = vec4(ourColor,1.0);\n"
"}\n\0";
int main() {
	glfwInit();//初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);//创建一个窗口对象
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//监听窗口改变时
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {//GLAD是用来管理OpenGL的函数指针的，初始化GLAD
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建一个顶点着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//把着色器源码附加到着色器对象上
	glCompileShader(vertexShader);//编译顶点着色器

	//判断着色器是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FEILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建一个片段着色器对象
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//把着色器源码附加到着色器对象上
	glCompileShader(fragmentShader);//编译片段着色器

	//判断着色器是否编译成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FEILED\n" << infoLog << std::endl;
	}

	//把两个着色器对象链接到一个用来渲染的着色器程序
	int shaderProgram = glCreateProgram();//创建一个程序，获取程序对象的ID引用
	glAttachShader(shaderProgram, vertexShader);//将之前编译的着色器附加到程序对象上
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//链接着色器程序

	//判断着色器程序是否链接成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FEILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);//删除顶点着色器对象
	glDeleteShader(fragmentShader);//删除片段着色器对象

	//glViewport(0, 0, 800, 600);//初始化OpenGL渲染窗口的尺寸，即视口（Viewport）

	float vertices[] = {
		//位置                   //颜色
		-0.5f, -0.5f, 0.0f,     1.0f,0.0f,0.0f,   0.0f,0.0f,// left
		 0.5f, -0.5f, 0.0f,     0.0f,1.0f,0.0f,  1.0f,0.0f,// right
		 0.0f,  0.5f, 0.0f,     0.0f,0.0f,1.0f,     0.5f,1.0f// top
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);//初始化顶点数组
	glGenBuffers(1,&VBO);//初始链接顶点
	//glGenBuffers(1, &EBO);

	//绑定顶点数组对象
	glBindVertexArray(VAO);

	//复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//设置顶点属性指针
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0, GL_RGB,GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glUseProgram(shaderProgram);//当我们渲染一个物体时要使用着色器程序
	int texture = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1f(texture, 1);

	while (!glfwWindowShouldClose(window)/*检查窗口是否被要求退出*/) {//渲染循环（Render Loop）
		//输入
		processInput(window);

		//渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//清空屏幕所有的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清除缓存，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glUseProgram(shaderProgram);//当我们渲染一个物体时要使用着色器程序

		// 更新uniform颜色
		float timeValue = (float)glfwGetTime();
		float greenValue = sin(timeValue) / 0.5f + (-0.5f);
		//float offset = 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "xOffset");
		glUniform1f(vertexColorLocation, greenValue);

		glBindVertexArray(VAO);
		//glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型。由于我们在一开始时说过，我们希望绘制的是一个三角形，这里传递GL_TRIANGLES给它。
		//第二个参数指定了顶点数组的起始索引，我们这里填0。
		//最后一个参数指定我们打算绘制多少个顶点，这里是3（我们只从我们的数据中渲染一个三角形，它只有3个顶点长）。
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//双缓冲
		glfwPollEvents();//检查有没有触发什么事件、更新窗口状态
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();//清理所有的资源
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height) {
	glViewport(0, 0, width, height);//初始化OpenGL渲染窗口的尺寸，即视口（Viewport）
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,true);
	}
}