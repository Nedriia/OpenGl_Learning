#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


//vertex shader
/*const char *vertex_shader_source = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"

"out vec4 vertex_color;\n"

"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n" //-> give a vec3 to a vec4 constructor
"vertex_color = vec4(1.0f,1.0f,1.0f,1.0f);\n"
"}\n\0";*/

const char *vertex_shader_source = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aCol;\n"

"out vec3 vertex_color;\n"

"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n" //-> give a vec3 to a vec4 constructor
"vertex_color = aCol;\n"
"}\n\0";
//Here the vertex shader decide the color of the fragment shader

/*const char *fragment_shader_source = "#version 330 core\n"
"in vec4 vertex_color;\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"FragColor = vertex_color;\n"
"}\n\0";*/

//Uniform -> are another way to pass data from the CPU to the shaders on the GPU
//They are global -> a uniform variable is unique per shader program object 
//and
//can be accessed from any shader at any stage in the shader program

/*const char *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"

"uniform vec4 ourColor;\n" //-> we set this variable in the O.GL code

"void main()\n"
"{\n"
"FragColor = ourColor;\n" 
"}\n\0";*/

const char *fragment_shader_source = "#version 330 core\n"
"in vec3 vertex_color;\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"FragColor = vec4(vertex_color,1.0f);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "Shaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	const int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::VERTEX::SHADER::COMPILATION::FAILED\n" << infoLog << "\n";
	}

	const int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::SHADER::COMPILATION::FAILED\n" << infoLog << "\n";
	}

	const int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);


	//If we want to set a color for each vertex we would have to declare as many 
	//unforms as we have vertices -> a better solution would be to include
	//more data in the vertex attributes
	/*float vertices[] = {
		// first triangle
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};*/

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	//Because we added another vertex attribute and updated VBO's memory
	//-> we have to reconfigure the vertex attribute pointer
	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//(void*)(3 * sizeof(float)
	//-> we know how the memory is organize
	//-> we also know we already have read 3 float
	//-> so the pointer will be = (void*)(previous reading data size)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(shader_program);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window))
	{
		//Input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);

		/*float timeValue = glfwGetTime();
		float green_value = (sin(timeValue)/2.0f) +0.5f;
		int vertex_ColorLocation = glGetUniformLocation(shader_program, "ourColor");	
		glUniform4f(vertex_ColorLocation, 0.0f, green_value,0.0f, 1.0f);*/

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	;	return 0;
	}


	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			GLint polygonMode;
			glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
			if (polygonMode == GL_LINE)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
}
