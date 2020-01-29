/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 11:50:07 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/27 10:38:08 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	parse_args(t_data *d, int ac, char **av)
{
	if (ac != 2)
		exit(printf("usage: %s obj_file\n\n", av[0]));
	d->objfilename = av[1];
}

void handle_events(t_data *d)
{
	float	speed;
	double	now;

	now = glfwGetTime();
	speed = 6 * (now - d->last_frame);
	d->last_frame = now;
	d->mix_value = fmax(d->mix_value - 0.3 * speed, 0);
	// printf("mix val: %f, transition1: %d, transition2: %d\n", d->mix_value, d->transition[0], d->transition[1]);
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

void	transition_toggle(t_data *d, int targetstate)
{
	if (d->mix_value)
		return ;
	d->transition[0] = d->transition[1];
	d->transition[1] = (targetstate == d->transition[0]) ? 0 : targetstate;
	d->mix_value = 1.0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	t_data *d;

	d = glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        transition_toggle(d, 1);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        transition_toggle(d, 2);
}

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"uniform mat4 model;"
	"uniform mat4 view;"
	"uniform mat4 projection;"
	"out vec3 pos;"
	"out vec3 FragPos;"
	"void main()\n"
	"{\n"
	"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	"	pos = aPos;"
	"	FragPos = vec3(view * model * vec4(aPos, 1.0));"
	"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;"
	"in vec3 pos;"
	"in vec3 FragPos;"
	"uniform sampler2D ourTexture;"
	"uniform float mix_value;"
	"uniform int transition[2];"
	"vec4 effect_0() {"
	"	float factor = gl_PrimitiveID % 5 * 0.1 + 0.05;"
	"	return (vec4(factor, factor, factor, 1));"
	"}"
	"vec4 effect_1() {"
	"	return (texture(ourTexture, vec2(pos.z, -pos.y)));"
	"}"
	"vec4 effect_2() {"
	"	vec3 color = vec3(0.5f, 0, 0);"
    "	vec3 xTangent = dFdx( FragPos );"
    "	vec3 yTangent = dFdy( FragPos );"
    "	vec3 faceNormal = normalize( cross( xTangent, yTangent ) );"
	"	vec3 lightPos  = vec3(0, 1, 3);"
	"	vec3 lightColor = vec3(1, 1, 1);"
	"	vec3 lightDir = normalize(lightPos  - FragPos);"
	"	float diff = max(dot(faceNormal, lightDir), 0.0);"
	"	vec3 diffuse = diff * lightColor;"
    "	float ambientStrength = 0.1;"
    "	vec3 ambient = ambientStrength * lightColor;"
	"	float specularStrength = 0.5;"
	"	vec3 viewDir = normalize( - FragPos);"
	"	vec3 reflectDir = reflect(-lightDir, faceNormal); "
	"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
	"	vec3 specular = specularStrength * spec * lightColor; \n"
	"	return (vec4(((ambient + diffuse + specular) * color), 1));"
	"}"
	"void main() {"
	"   vec4 results[3];"
	"   results[0] = effect_0();"
	"   results[1] = effect_1();"
	"   results[2] = effect_2();"
	"   FragColor = mix(results[transition[1]], results[transition[0]], mix_value);"
	"}";

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

int main(int ac, char **av)
{
	t_data	mydata = {.pos = {0, 0, 3}};
	t_data	*d = &mydata;
	parse_args(d, ac, av);
	parse_obj(d);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	d->window = glfwCreateWindow(800, 600,
		"SCOP. Movement: W/A/S/D/Space/Shift, Texture: T, Lighting: L",
		NULL, NULL);
	if (d->window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwSetWindowUserPointer(d->window, d);
	glfwSetKeyCallback(d->window, key_callback);
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

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, d->faces.size, d->faces.array, GL_STATIC_DRAW);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, d->vertices.size, d->vertices.array, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Load texture
	int w, h;
	uint8_t *data = read_ppm("./texture.ppm", &w, &h);
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
		int mix_value_loc = glGetUniformLocation(shaderProgram, "mix_value");
		int transition_loc = glGetUniformLocation(shaderProgram, "transition");

		handle_events(d);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glUniform1f(mix_value_loc, d->mix_value);
		glUniform1iv(transition_loc, 2, d->transition);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view);
		glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindVertexArray(VAO);
		model = mat_identity((float[16]){});
		model = mat_rotate('y', (float)glfwGetTime() * 0.5, model);
		model = mat_translate(d->model_center_offset, model);
		glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model);
		// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDrawElements(GL_TRIANGLES, d->faces.size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(d->window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
