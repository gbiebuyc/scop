/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 11:50:07 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/21 16:10:26 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void handle_events(t_data *d)
{
	float	speed;
	double	now;

	now = glfwGetTime();
	speed = 6 * (now - d->last_frame);
	d->last_frame = now;
	if (glfwGetKey(d->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(d->window, true);
    if (glfwGetKey(d->window, GLFW_KEY_W) == GLFW_PRESS)
        d->pos[Z] -= speed;
    if (glfwGetKey(d->window, GLFW_KEY_S) == GLFW_PRESS)
        d->pos[Z] += speed;
    if (glfwGetKey(d->window, GLFW_KEY_A) == GLFW_PRESS)
        d->pos[X] -= speed;
    if (glfwGetKey(d->window, GLFW_KEY_D) == GLFW_PRESS)
        d->pos[X] += speed;
    if (glfwGetKey(d->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        d->pos[Y] += speed;
    if (glfwGetKey(d->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        d->pos[Y] -= speed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;"
	"uniform mat4 model;"
	"uniform mat4 view;"
	"uniform mat4 projection;"
	"out vec3 ourColor;"
	"out vec2 TexCoord;"
	"void main()\n"
	"{\n"
	"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"   ourColor = aColor;"
	"   TexCoord = aTexCoord;"
	"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;"
	"in vec2 TexCoord;"
	"uniform sampler2D ourTexture;"
	"void main()\n"
	"{\n"
	"   FragColor = texture(ourTexture, TexCoord);"
	"}\n\0";

uint8_t *read_ppm(char *filename, int *w, int *h)
{
	FILE    *f;
	char    buf[255];
	uint8_t *img_buf;

	if (!(f = fopen(filename, "rb")))
		exit(printf("fopen failed %s\n", filename));
	if (!fgets(buf, 255, f) || (strncmp(buf, "P6\n", 3) != 0))
		exit(printf("image read error 1"));
	if (!fgets(buf, 255, f))
		exit(printf("image read error 1 bis"));
	while (strncmp(buf, "#", 1) == 0)
		if (!fgets(buf, 255, f))
			exit(printf("image read error 2"));
	if (sscanf(buf, "%u %u", w, h) < 2)
		exit(printf("image read error 3"));
	if (!fgets(buf, 255, f))
		exit(printf("image read error 4"));
	if (!(img_buf = malloc(*w * *h * 3)))
		exit(printf("image read error 5"));
	if (!(fread(img_buf, 1, *w * *h * 3, f)))
		exit(printf("image read error 6"));
	return img_buf;
}

int main()
{
	t_data	mydata = {.pos = {0, 0, 3}};
	t_data	*d = &mydata;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	d->window = glfwCreateWindow(800, 600,
			"SCOP. Controls = W/A/S/D/Space/Shift.", NULL, NULL);
	if (d->window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(d->window);
	glfwSetFramebufferSizeCallback(d->window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}

	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float cubePositions[][3] = {
	  { 0.0f,  0.0f,  0.0f}, 
	  { 2.0f,  5.0f, -15.0f}, 
	  {-1.5f, -2.2f, -2.5f},  
	  {-3.8f, -2.0f, -12.3f},  
	  { 2.4f, -0.4f, -3.5f},  
	  {-1.7f,  3.0f, -7.5f},  
	  { 1.3f, -2.0f, -2.5f},  
	  { 1.5f,  2.0f, -2.5f}, 
	  { 1.5f,  0.2f, -1.5f}, 
	  {-1.3f,  1.0f, -1.5f}  
	};

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	// glEnableVertexAttribArray(1);

	// tex attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Load texture
	int w, h;
	uint8_t *data = read_ppm("./wall.ppm", &w, &h);
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(data);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(d->window))
	{
		int width, height;
		glfwGetFramebufferSize(d->window, &width, &height);

		float *model;
		float *view;
		float *projection;

		view = mat_look_at(d->pos);
		projection = mat_projection(width / (float)height);

		int modelLoc = glGetUniformLocation(shaderProgram, "model");
		int viewLoc = glGetUniformLocation(shaderProgram, "view");
		int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

		handle_events(d);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view);
		glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);
		// glUniform4f(vertexColorLocation, 0, greenValue, 0, 1);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindTexture(GL_TEXTURE_2D, tex);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; i++) {
			float angle = 20.0f * i;
			model = mat_identity((float[16]){});
			model = mat_translate(cubePositions[i], model);
			model = mat_rotate('y', (float)glfwGetTime() * 0.5, model);
			glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(d->window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
