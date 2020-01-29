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
	glfwSetKeyCallback(d->window, (GLFWkeyfun)key_callback);
	glfwMakeContextCurrent(d->window);
	glfwSetFramebufferSizeCallback(d->window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	load_shader_prog(d);

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
	load_texture(d);
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

		int modelLoc = glGetUniformLocation(d->shader_prog, "model");
		int viewLoc = glGetUniformLocation(d->shader_prog, "view");
		int projectionLoc = glGetUniformLocation(d->shader_prog, "projection");
		int mix_value_loc = glGetUniformLocation(d->shader_prog, "mix_value");
		int transition_loc = glGetUniformLocation(d->shader_prog, "transition");

		handle_events(d);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(d->shader_prog);
		glUniform1f(mix_value_loc, d->mix_value);
		glUniform1iv(transition_loc, 2, d->transition);
		glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view);
		glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
