
//#include <glad/glad.h>
// 引入GLEW库 定义静态链接
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include<iostream>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb-master/stb_image.h>
#include <vector>
// 引入SOIL库
#include <SOIL/SOIL.h>
// 包含着色器加载库
#include "Shader.h"

void initGLFWWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// 键盘回调函数原型声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLfloat mixValue = 0.4f; // 纹理混合参数


//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {
	initGLFWWindow();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);//创建窗口对象
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);//监听窗口变化
	/*if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//GLAD是用来管理OpenGL的函数指针的，初始化GLAD
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}*/

	// 注册窗口键盘事件回调函数
	glfwSetKeyCallback(window, key_callback);

	// 初始化GLEW 获取OpenGL函数
	glewExperimental = GL_TRUE; // 让glew获取所有拓展函数
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error::GLEW glew version:" << glewGetString(GLEW_VERSION)
			<< " error string:" << glewGetErrorString(status) << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置视口参数
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//Shader* shader;
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	/*float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left

		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};*/
	/*GLfloat vertices[] = {
		// 第一个三角形
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		// 第二个三角形
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};*/
	/*GLfloat vertices[] = {
		-0.5f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, // 0
		0.5f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,  // 1
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,  // 2
		0.0f, -0.5f, 0.0f,	1.0f, 1.0f, 0.0f  // 3
	};*/

	// 指定顶点属性数据 顶点位置 颜色 纹理
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,  // 0
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	2.0f, 0.0f,  // 1
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	2.0f, 2.0f,  // 2
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 2.0f   // 3
	};

	// 索引数据
	GLshort indices[] = {
		0, 1, 2,  // 第一个三角形
		0, 2, 3   // 第二个三角形
	};



	//创建缓存对象
	GLuint VBO, VAO, EBOId;
	//创建绑定VAO对象
	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);


	//创建绑定VBO对象
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Step3: 创建并绑定EBO 对象 传送数据
	glGenBuffers(1, &EBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Step4: 指定解析方式  并启用顶点属性
	// 顶点位置属性
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 顶点颜色属性
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	// 顶点纹理坐标
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 注意不要解除EBO绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader ourShader("3.5.shader.vs", "3.5.shader.fs");

	// Section3 准备纹理对象
	// Step1 创建并绑定纹理对象
	GLuint textureId1, textureId2;
	glGenTextures(1, &textureId1);
	glBindTexture(GL_TEXTURE_2D, textureId1);
	// Step2 设定wrap参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Step3 设定filter参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR); // 为MipMap设定filter方法

	// Step4 加载纹理
	GLubyte *imageData = NULL;
	int picWidth, picHeight;
	imageData = SOIL_load_image("awesomeface.png",&picWidth, &picHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picWidth, picHeight,0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Step5 释放纹理图片资源
	SOIL_free_image_data(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &textureId2);
	glBindTexture(GL_TEXTURE_2D, textureId2);
	// Step2 设定wrap参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Step3 设定filter参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 为MipMap设定filter方法
	imageData = SOIL_load_image("container.jpg", &picWidth, &picHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picWidth, picHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Step5 释放纹理图片资源
	SOIL_free_image_data(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		glfwPollEvents(); // 处理例如鼠标 键盘等事件

		// render
		// ------
		// 清除颜色缓冲区 重置为指定颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// render the triangle
		// 这里填写场景绘制代码
		glBindVertexArray(VAO);
		ourShader.use();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// 启用纹理单元 绑定纹理对象
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId1);
		glUniform1i(glGetUniformLocation(ourShader.ID, "tex"), 0); // 设置纹理单元为0号
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureId2);
		glUniform1i(glGetUniformLocation(ourShader.ID, "tex2"), 1); // 设置纹理单元为1号

		glUniform1f(glGetUniformLocation(ourShader.ID, "mixValue"),
			mixValue); // 设置纹理混合参数

		// 使用索引绘制
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // 交换缓存
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;

}

void initGLFWWindow() {
	if (!glfwInit())	// 初始化glfw库
	{
		std::cout << "Error::GLFW could not initialize GLFW!" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // 关闭窗口
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		mixValue += 0.05f;
		if (mixValue > 1.0f)
			mixValue = 1.0f;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		mixValue -= 0.05f;
		if (mixValue < 0.0f)
			mixValue = 0.0f;
	}

}