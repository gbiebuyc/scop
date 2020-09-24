/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_gl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 18:16:37 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/29 18:17:09 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	init_gl_2(t_data *d)
{
	GLuint vbo;

	glGenVertexArrays(1, &d->model_vao);
	glBindVertexArray(d->model_vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
		d->gl_arr_buf.size, d->gl_arr_buf.array, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glEnable(GL_MULTISAMPLE);
}

void	error_callback(int err_code, const char *description)
{
	(void)err_code;
	printf("%s\n", description);
}

void	init_gl(t_data *d)
{
	setenv("LIBGL_ALWAYS_SOFTWARE", "1", 1);
	glfwSetErrorCallback(error_callback);
	glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	d->window = glfwCreateWindow(800, 600,
		"SCOP. Movement: W/A/S/D/Space/Shift, Texture: T, Shaders: Left/Right",
		NULL, NULL);
	if (d->window == NULL)
		exit_scop(d, printf("Failed to create GLFW window\n"));
	glfwSetWindowUserPointer(d->window, d);
	glfwSetKeyCallback(d->window, (GLFWkeyfun)(intptr_t) & key_callback);
	glfwMakeContextCurrent(d->window);
	glfwSetFramebufferSizeCallback(d->window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		exit_scop(d, printf("Failed to initialize GLAD\n"));
	init_gl_2(d);
}
