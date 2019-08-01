#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//vertex shader
const char *vertex_shader_source = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"

"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";


const char *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


//In modern OpenGl, we are required to define at least a vertex and fragment shader
//of our own (There are no default vertex/fragments shaders on the GPU)


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//We'd like to send it as input (our vertices) to the first process of the graphics pipeline : the vertex shader
//		-It's done by creating memory on the GPU
//		-Configure how O.GL should interprete the memory and specify how to send the data to graphics card

//To manage this memory -> Vertex buffer objects (VBO)

	/*unsigned int VBO;
	glGenBuffers(1, &VBO);*/

//VBO type of buffer is -> GL_ARRAY_BUFFER

	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

//glBufferData -> copies the previous defined vertex data into the buffer's memory
//		-glBufferData(type,size in bytes,data,the way we want the CG manage the data)
//we stored the vertex data within the memory now we want to build the vertex and fragment shader

//We wrote the source code for the vertex shader ( const char *vertex_shader_source)
//->	in order to use the shader it has to dynamically compile it at run time 
	const int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//Next we attach the shader source code to the shader object and compile the shader
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
//-> glShaderSource(shader, how many string we're passing, source code of the shader,NULL)
	glCompileShader(vertex_shader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	const int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment_shader, 512,NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << "\n";
	}

//Both the shaders are now compiled and the only thing left to do is link both shader objects
//into a shader program that we can use for rendering
//		-When linking -> it links the outputs of each shader to the inputs of the next shader

	const int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::COMPILATION::FAILED\n" << infoLog << "\n";
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);


//We define vertex data

	//Triangles
	/*float vertices [] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};*/
	//If we want to draw a rectangle -> but it's an overhead of 50%
	/*float vertices[] = {
		// first triangle
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  0.5f, 0.0f,  // top left 
		// second triangle
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};*/

//So we can use EBO ( element buffer objects) -> buffer that stores indices that
//O.GL uses to decide what vertices to draw

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


//But O.GL don't know how it should interpret the vertex data in memory and how it should
//connect the vertex data to the vertex shader's attributes
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//		->glVertexAttribPointer(which vertex attribute we want to configure,size of the vertex attribute,type,if we want data to be normalized,stride ->space between consecutive vertex attributes,offset where the position data begin in the buffer)
	//glEnableVertexAttribArray(0);

//0. copy our vertices array in a buffer for O.GL to use
	//Vertex Array Object
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

//VAO will make it easy to switch between VBO
//Core OpenGL requires that we use a VAO so it knows what to do with our vertex inputs. If we fail to bind a VAO, OpenGL will most likely refuse to draw anything.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
//2. use our shader program when we want to render an object

	glBindBuffer(GL_ARRAY_BUFFER, 0);//Unbind
	glBindVertexArray(0);


	while(!glfwWindowShouldClose(window))
	{
		//Input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBindVertexArray(0);
		//-> whenever we call glClear and clear the buffer-> the entire color buffer
		//will be filled with the color as configured by glClearColor


		//Check and call events and swap the buffers
		glfwSwapBuffers(window);//-> Swap the color buffer
		glfwPollEvents(); //-> check if any event are trigerred
	}

	//Once we've linked them into the program object -> we no longer need them so we can delete them
	

	glfwTerminate();
	//Terminate here clear all previously allocated GLFW resources
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
	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
		if (polygonMode == GL_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}
