#include <stdio.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

const char *vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(pos.xyz, 1.0);\n"
		"}";
const char *mandelbrotShaderSource =
		"#version 330 core\n"
		"uniform vec2 move;\n"
		"uniform float zoom;\n"
		"uniform vec2 screen;\n"
		"in vec4 gl_FragCoord;\n"
		"out vec4 frag_color;\n"
		"#define MAX_ITERATIONS 1000\n"
		"int get_iterations()\n"
		"{\n"
		" float n=1./zoom;\n"
		" float real=((gl_FragCoord.x+move.x-screen.x/2.)/1000.)*n;\n"
		" float imag=((gl_FragCoord.y-move.y-screen.y/2.)/1000.)*n;\n"
		" \n"
		" int iterations=0;\n"
		" float const_real=real;\n"
		" float const_imag=imag;\n"
		" \n"
		" while(iterations<MAX_ITERATIONS)\n"
		" {\n"
		"   float tmp_real=real;\n"
		"   real=(real*real-imag*imag)+const_real;\n"
		"   imag=(2.*tmp_real*imag)+const_imag;\n"
		"   \n"
		"   float dist=real*real+imag*imag;\n"
		"   \n"
		"   if(dist>4.)\n"
		"   break;\n"
		"   \n"
		"   ++iterations;\n"
		" }\n"
		" return iterations;\n"
		"}\n"
		"vec3 gradient(float n){\n"
		" float red=10.f*n/MAX_ITERATIONS;\n"
		" float green=5.f*n/MAX_ITERATIONS-.5f;\n"
		" float blue=(6.f*n-9.f)/(2.f*(4.f*MAX_ITERATIONS-6.f));\n"
		" return vec3(red,green,blue);\n"
		"}\n"
		"vec4 return_color()\n"
		"{\n"
		" int iter=get_iterations();\n"
		" \n"
		" if (iter == MAX_ITERATIONS) return vec4(1.f, 1.f, .75f, 1.f);"
		" \n"
		" return vec4(gradient(iter),1.f);\n"
		"}\n"
		"void main()\n"
		"{\n"
		" frag_color=return_color();\n"
		"}";
const char *viewShaderSource =
		"#version 330 core\n"
		"uniform vec2 move;\n"
		"uniform vec2 mouse;\n"
		"uniform float zoom;\n"
		"uniform vec2 screen;\n"
		"in vec4 gl_FragCoord;\n"
		"out vec4 frag_color;\n"
		"#define MAX_ITERATIONS 1000\n"
		"int get_iterations_1()\n"
		"{\n"
		" float n=1./zoom;\n"
		" float real=((gl_FragCoord.x+move.x-screen.x/2.)/1000.)*n;\n"
		" float imag=((gl_FragCoord.y-move.y-screen.y/2.)/1000.)*n;\n"
		" \n"
		" int iterations=0;\n"
		" float const_real=real;\n"
		" float const_imag=imag;\n"
		" \n"
		" while(iterations<MAX_ITERATIONS)\n"
		" {\n"
		"   float tmp_real=real;\n"
		"   real=(real*real-imag*imag)+const_real;\n"
		"   imag=(2.*tmp_real*imag)+const_imag;\n"
		"   \n"
		"   float dist=real*real+imag*imag;\n"
		"   \n"
		"   if(dist>4.)\n"
		"   break;\n"
		"   \n"
		"   ++iterations;\n"
		" }\n"
		" return iterations;\n"
		"}\n"
		"int get_iterations_2()\n"
		"{\n"
		" float real=((gl_FragCoord.x+move.x-screen.x/2.)/1000.)*4;\n"
		" float imag=((gl_FragCoord.y-move.y-screen.y/2.)/1000.)*4;\n"
		" \n"
		" int iterations=0;\n"
		" \n"
		" while(iterations<MAX_ITERATIONS)\n"
		" {\n"
		"   float tmp_real=real;\n"
		"   real=(real*real-imag*imag)+mouse.x;\n"
		"   imag=(2.*tmp_real*imag)+mouse.y;\n"
		"   \n"
		"   float dist=real*real+imag*imag;\n"
		"   \n"
		"   if(dist>4.)\n"
		"   break;\n"
		"   \n"
		"   ++iterations;\n"
		" }\n"
		" return iterations;\n"
		"}\n"
		"vec3 gradient(float n){\n"
		" float red=10.f*n/MAX_ITERATIONS;\n"
		" float green=5.f*n/MAX_ITERATIONS-.5f;\n"
		" float blue=(6.f*n-9.f)/(2.f*(4.f*MAX_ITERATIONS-6.f));\n"
		" return vec3(red,green,blue);\n"
		"}\n"
		"vec4 return_color()\n"
		"{\n"
		" int iter1=get_iterations_1();\n"
		" int iter2=get_iterations_2();\n"
		" int iter=iter1+iter2;\n"
		" \n"
		" if (iter1 == MAX_ITERATIONS) iter1 = 0;"
		" if (iter2 == MAX_ITERATIONS) iter2 = 0;"
		" \n"
		" return vec4(gradient(max(iter1, iter2)),1.f);\n"
		"}\n"
		"void main()\n"
		"{\n"
		" frag_color=return_color();\n"
		"}";
const char *juliaShaderSource =
		"#version 330 core\n"
		"uniform vec2 move;\n"
		"uniform vec2 position;\n"
		"uniform float zoom;\n"
		"uniform vec2 screen;\n"
		"in vec4 gl_FragCoord;\n"
		"out vec4 frag_color;\n"
		"#define MAX_ITERATIONS 1000\n"
		"float n=1./zoom;\n"
		"int get_iterations()\n"
		"{\n"
		" float real=((gl_FragCoord.x+move.x-screen.x/2.)/1000.)*n;\n"
		" float imag=((gl_FragCoord.y-move.y-screen.y/2.)/1000.)*n;\n"
		" \n"
		" int iterations=0;\n"
		" \n"
		" while(iterations<MAX_ITERATIONS)\n"
		" {\n"
		"   float tmp_real=real;\n"
		"   real=(real*real-imag*imag)+position.x;\n"
		"   imag=(2.*tmp_real*imag)+position.y;\n"
		"   \n"
		"   float dist=real*real+imag*imag;\n"
		"   \n"
		"   if(dist>4.)\n"
		"   break;\n"
		"   \n"
		"   ++iterations;\n"
		" }\n"
		" return iterations;\n"
		"}\n"
		"vec3 gradient(float n){\n"
		" float red=10.f*n/MAX_ITERATIONS;\n"
		" float green=5.f*n/MAX_ITERATIONS-.5f;\n"
		" float blue=(6.f*n-9.f)/(2.f*(4.f*MAX_ITERATIONS-6.f));\n"
		" return vec3(red,green,blue);\n"
		"}\n"
		"vec4 return_color()\n"
		"{\n"
		" int iter=get_iterations();\n"
		" \n"
		" if (iter == MAX_ITERATIONS) return vec4(1.f, 1.f, .75f, 1.f);"
		" \n"
		" return vec4(gradient(iter),1.f);\n"
		"}\n"
		"void main()\n"
		"{\n"
		" frag_color=return_color();\n"
		"}";

int screen_width = 1000;
int screen_height = 700;

unsigned int VBO, VAO, EBO;
unsigned int shaders[3];
unsigned int shaderProgram;

double lastTime;
int nbFrames = 0;
int fps = 0;
char fpsStr[100];

double whell = 0;
GLboolean rightClick = GL_FALSE;
GLboolean leftClick = GL_FALSE;

vec2 wherePressed = {0, 0};
bool canDrag = true;
vec2 mousePosition = {-100, -100};
vec2 cameraTranslation = {0, 0};
vec2 target = {0, 0};
double zoom = 0.25;

typedef enum
{
	mandelbrot = 0,
	view = 1,
	julia = 2
} modes;
modes mode = mandelbrot;

CGLM_INLINE
void convert(vec2 vector, vec2 dest)
{
	double x = (vector[0] + cameraTranslation[0] - screen_width / 2) / (1000 * zoom);
	double y = (vector[1] + cameraTranslation[1] - screen_height / 2) / (1000 * zoom);

	dest[0] = x;
	dest[1] = y;
}

void reset(GLFWwindow *window)
{
	zoom = 0.25;
	glm_vec2_zero(cameraTranslation);
	glfwSetCursorPos(window, screen_width / 2, screen_height / 2);
}

void changeMode(modes toMod)
{
	shaderProgram = shaders[toMod];
	glUseProgram(shaderProgram);
	mode = toMod;
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
	screen_width = width;
	screen_height = height;
	for (size_t i = 0; i < 3; i++)
	{
		glUseProgram(shaders[i]);
		int vertexLocation = glGetUniformLocation(shaders[i], "screen");
		glUniform2f(vertexLocation, screen_width, screen_height);
	}
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	double toZoom = zoom * (0.5f * yoffset + 1);
	if (toZoom > 0)
	{
		vec2 vector1;
		vec2 vector2;
		glm_vec2_add(mousePosition, cameraTranslation, vector1);
		glm_vec2_sub(vector1, (vec2){screen_width / 2, screen_height / 2}, vector2);
		glm_vec2_copy(vector2, cameraTranslation);
		glfwSetCursorPos(window, screen_width / 2, screen_height / 2);
		glm_vec2_scale(cameraTranslation, toZoom / zoom, cameraTranslation);
		zoom = toZoom;
	}
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glm_vec2_copy(mousePosition, target);
		glm_vec2_copy(cameraTranslation, wherePressed);
		canDrag = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (mode == mandelbrot)
		{
			changeMode(view);
			int vertexLocation = glGetUniformLocation(shaders[1], "move");
			glUniform2f(vertexLocation, cameraTranslation[0], cameraTranslation[1]);
			vertexLocation = glGetUniformLocation(shaders[1], "zoom");
			glUniform1f(vertexLocation, zoom);
		}
		else
		{
			changeMode(mandelbrot);
			reset(window);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		if (mode == view)
		{
			changeMode(julia);
			int vertexLocation = glGetUniformLocation(shaders[2], "position");
			vec2 position;
			convert(mousePosition, position);
			glUniform2f(vertexLocation, position[0], position[1]);
			reset(window);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		canDrag = false;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		reset(window);
	}
}

void mouseCursorCallback(GLFWwindow *window, double xpos, double ypos)
{
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	glm_vec2_copy((vec2){x, y}, mousePosition);

	if (canDrag && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_RELEASE)
	{
		vec2 draged;
		glm_vec2_sub(target, mousePosition, draged);
		glm_vec2_add(wherePressed, draged, cameraTranslation);
	}
}

void init()
{
	// fps counter
	lastTime = glfwGetTime();

	for (size_t i = 0; i < 3; i++)
	{
		// vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		shaders[i] = glCreateProgram();
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// fragment shader
		switch (i)
		{
		case 0:
			glShaderSource(fragmentShader, 1, &mandelbrotShaderSource, NULL);
			break;
		case 1:
			glShaderSource(fragmentShader, 1, &viewShaderSource, NULL);
			break;
		case 2:
			glShaderSource(fragmentShader, 1, &juliaShaderSource, NULL);
			break;
		}
		glCompileShader(fragmentShader);
		int success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			printf("%s", infoLog);
		};
		// link shaders
		glAttachShader(shaders[i], vertexShader);
		glAttachShader(shaders[i], fragmentShader);
		glLinkProgram(shaders[i]);
	}
	shaderProgram = shaders[0];

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
			-1.0f, -1.0f, -0.0f,
			1.0f, 1.0f, -0.0f,
			-1.0f, 1.0f, -0.0f,
			1.0f, -1.0f, -0.0f};
	unsigned int indices[] = {
			0, 1, 2, // first Triangle
			0, 3, 1	 // second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// mouse move
	for (size_t i = 0; i < 3; i++)
	{
		glUseProgram(shaders[i]);
		int vertexLocation = glGetUniformLocation(shaders[i], "move");
		glUniform2f(vertexLocation, 0, 0);
		vertexLocation = glGetUniformLocation(shaders[i], "zoom");
		glUniform1f(vertexLocation, zoom);
		vertexLocation = glGetUniformLocation(shaders[i], "mouse");
		glUniform2f(vertexLocation, 0, 0);
		vertexLocation = glGetUniformLocation(shaders[i], "screen");
		glUniform2f(vertexLocation, screen_width, screen_height);
	}

	// screen color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void render()
{
	double currentTime = glfwGetTime();

	glUseProgram(shaderProgram);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// mouse move
	int vertexLocation = glGetUniformLocation(shaderProgram, "move");
	glUniform2f(vertexLocation, cameraTranslation[0], cameraTranslation[1]);
	vertexLocation = glGetUniformLocation(shaderProgram, "zoom");
	glUniform1f(vertexLocation, zoom);
	if (mode == view)
	{
		vertexLocation = glGetUniformLocation(shaders[1], "mouse");
		vec2 position;
		convert(mousePosition, position);
		glUniform2f(vertexLocation, position[0], position[1]);
	}

	// fps counter
	nbFrames++;
	if (currentTime - lastTime >= 1.0)
	{ // If last prinf() was more than 1 sec ago
		// printf and reset timer
		fps = nbFrames;
		nbFrames = 0;
		lastTime += 1.0;
	}

	// zoom += 0.01;
	// glm_vec2_scale(cameraTranslation, zoom/(zoom - 0.01), cameraTranslation);
}

int main(int argc, char **args)
{
	GLFWwindow *window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(screen_width, screen_height, "window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetCursorPosCallback(window, mouseCursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);

	init();
	while (!glfwWindowShouldClose(window))
	{
		render();
		sprintf(fpsStr, "Window Fps: %d, local: %f, %f, zoom: %f", fps, cameraTranslation[0], cameraTranslation[1], zoom);
		glfwSetWindowTitle(window, fpsStr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	puts("fim do programa");

	return 0;
}