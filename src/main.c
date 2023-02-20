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
const char *fragmentShaderSource =
		"#version 330 core\n"
		"uniform vec2 move;\n"
		"uniform float zoom;\n"
		"in vec4 gl_FragCoord;\n"
		"out vec4 frag_color;\n"
		"#define MAX_ITERATIONS 1000\n"
		"int get_iterations()\n"
		"{\n"
		" float n = 1./zoom;\n"
		" float real=(gl_FragCoord.x/1000.-.5)*n + move.x/1000.*n;\n"
		" float imag=(gl_FragCoord.y/1000.-.375)*n - move.y/1000.*n;\n"
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
		" float red = 10.f * n / MAX_ITERATIONS;\n"
		" float green = 5.f * n / MAX_ITERATIONS - .5f;\n"
		" float blue = (6.f * n -9.f) / (2.f * (4.f * MAX_ITERATIONS - 6.f));\n"
		" return vec3(red, green, blue);\n"
		"}\n"
		"vec4 return_color()\n"
		"{\n"
		" int iter=get_iterations();\n"
		" if(iter==MAX_ITERATIONS)\n"
		" {\n"
		"   gl_FragDepth=0.f;\n"
		"   return vec4(1.f,1.f,.75f,1.f);\n"
		" }\n"
		" \n"
		" float iterations=float(iter)/MAX_ITERATIONS;\n"
		" return vec4(gradient(iter),1.f);\n"
		"}\n"
		"void main()\n"
		"{\n"
		" frag_color=return_color();\n"
		"}";

int screen_width = 1000;
int screen_height = 700;

unsigned int VBO, VAO, EBO;
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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
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
		glm_vec2_scale(cameraTranslation, toZoom/zoom, cameraTranslation);
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
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		canDrag = false;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		zoom = 0.25;
		glm_vec2_zero(cameraTranslation);
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
		// Vector2 draged = sDivide(camera.zoom, Vector2Subtract(target, mousePosition));

		vec2 draged;
		glm_vec2_sub(target, mousePosition, draged);
		glm_vec2_add(wherePressed, draged, cameraTranslation);
	}
}

void init()
{
	// fps counter
	lastTime = glfwGetTime();

	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER); // orange color
	shaderProgram = glCreateProgram();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderOrange);
	// link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShaderOrange);
	glLinkProgram(shaderProgram);
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
	int vertexLocation = glGetUniformLocation(shaderProgram, "move");
	glUseProgram(shaderProgram);
	glUniform2f(vertexLocation, 0, 0);
	vertexLocation = glGetUniformLocation(shaderProgram, "zoom");
	glUniform1f(vertexLocation, zoom);

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