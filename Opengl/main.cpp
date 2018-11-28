#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
//������ɫ����Vertex Shader�� ������Vector��ÿ������ֵ��ʾ�ռ��һ�����ꡣ
/*const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n "// λ�ñ���������λ��ֵΪ0
"layout (location = 1) in vec3 aColor;\n "// λ�ñ���������λ��ֵΪ0
"layout (location = 2) in vec2 aTexCoord;\n "// ��������λ��ֵΪ0
//"out vec4 vertexColor;"//ΪƬ����ɫ��ָ��һ����ɫ���
"out vec3 ourColor;"//��Ƭ����ɫ�����һ����ɫ
"out vec2 TexCoord;"
"uniform float xOffset;"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x+xOffset, aPos.y, aPos.z, 1.0);\n"// add the xOffset to the x position of the vertex position
//"   gl_Position = vec4(aPos,1.0);\n"//ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
"	ourColor = aColor;\n"
"	TexCoord = vec2(aTexCoord.x,aTexCoord.y);\n"
//"   vertexColor = vec4(0.5,0.0,0.0,1.0);\n"// �������������Ϊ����ɫ
"}\0";
//Ƭ����ɫ�� 4��Ԫ���Ǻ�ɫ����ɫ����ɫ��alpha��͸���ȣ�
/*const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";*/
/*const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"// �Ӷ�����ɫ�����������������������ͬ��������ͬ��
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";*/
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;"
"in vec2 TexCoord;"
"uniform sampler2D texture1;"
//"uniform vec4 ourColor;\n"// ��OpenGL����������趨�������
"void main()\n"
"{\n"
"	FragColor = texture(texture1, TexCoord);"
//"   FragColor = ourColor;\n"
//"	FragColor = vec4(ourColor,1.0);\n"
"}\n\0";
int main() {
	glfwInit();//��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL",NULL,NULL);//����һ�����ڶ���
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//�������ڸı�ʱ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {//GLAD����������OpenGL�ĺ���ָ��ģ���ʼ��GLAD
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);//����һ��������ɫ������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����ɫ��Դ�븽�ӵ���ɫ��������
	glCompileShader(vertexShader);//���붥����ɫ��

	//�ж���ɫ���Ƿ����ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FEILED\n" << infoLog << std::endl;
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//����һ��Ƭ����ɫ������
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//����ɫ��Դ�븽�ӵ���ɫ��������
	glCompileShader(fragmentShader);//����Ƭ����ɫ��

	//�ж���ɫ���Ƿ����ɹ�
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FEILED\n" << infoLog << std::endl;
	}

	//��������ɫ���������ӵ�һ��������Ⱦ����ɫ������
	int shaderProgram = glCreateProgram();//����һ�����򣬻�ȡ��������ID����
	glAttachShader(shaderProgram, vertexShader);//��֮ǰ�������ɫ�����ӵ����������
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);//������ɫ������

	//�ж���ɫ�������Ƿ����ӳɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FEILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);//ɾ��������ɫ������
	glDeleteShader(fragmentShader);//ɾ��Ƭ����ɫ������

	//glViewport(0, 0, 800, 600);//��ʼ��OpenGL��Ⱦ���ڵĳߴ磬���ӿڣ�Viewport��

	float vertices[] = {
		//λ��                   //��ɫ
		-0.5f, -0.5f, 0.0f,     1.0f,0.0f,0.0f,   0.0f,0.0f,// left
		 0.5f, -0.5f, 0.0f,     0.0f,1.0f,0.0f,  1.0f,0.0f,// right
		 0.0f,  0.5f, 0.0f,     0.0f,0.0f,1.0f,     0.5f,1.0f// top
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);//��ʼ����������
	glGenBuffers(1,&VBO);//��ʼ���Ӷ���
	//glGenBuffers(1, &EBO);

	//�󶨶����������
	glBindVertexArray(VAO);

	//���ƶ������鵽�����й�OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//���ö�������ָ��
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);
	//��������
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

	glUseProgram(shaderProgram);//��������Ⱦһ������ʱҪʹ����ɫ������
	int texture = glGetUniformLocation(shaderProgram, "texture1");
	glUniform1f(texture, 1);

	while (!glfwWindowShouldClose(window)/*��鴰���Ƿ�Ҫ���˳�*/) {//��Ⱦѭ����Render Loop��
		//����
		processInput(window);

		//��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//�����Ļ���е���ɫ
		glClear(GL_COLOR_BUFFER_BIT);//������棬�����ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glUseProgram(shaderProgram);//��������Ⱦһ������ʱҪʹ����ɫ������

		// ����uniform��ɫ
		float timeValue = (float)glfwGetTime();
		float greenValue = sin(timeValue) / 0.5f + (-0.5f);
		//float offset = 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "xOffset");
		glUniform1f(vertexColorLocation, greenValue);

		glBindVertexArray(VAO);
		//glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�����������һ��ʼʱ˵��������ϣ�����Ƶ���һ�������Σ����ﴫ��GL_TRIANGLES������
		//�ڶ�������ָ���˶����������ʼ����������������0��
		//���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3������ֻ�����ǵ���������Ⱦһ�������Σ���ֻ��3�����㳤����
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//˫����
		glfwPollEvents();//�����û�д���ʲô�¼������´���״̬
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();//�������е���Դ
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height) {
	glViewport(0, 0, width, height);//��ʼ��OpenGL��Ⱦ���ڵĳߴ磬���ӿڣ�Viewport��
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,true);
	}
}