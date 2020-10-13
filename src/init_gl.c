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
	glVertexAttribPointer(0, 3, GL_FLOAT, false,
			8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false,
			8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false,
			8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	glGenFramebuffers(1, &d->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, d->fbo);

	glGenTextures(1, &d->screentexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, d->screentexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, d->w, d->h, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, d->screentexture, 0);

	glGenTextures(1, &d->normalstexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, d->normalstexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, d->w, d->h, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, d->normalstexture, 0);

	GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, DrawBuffers);

	glGenRenderbuffers(1, &d->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, d->rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, d->w, d->h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, d->rbo);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		puts("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	fflush(stdout);
	d->framebuffer_shader_prog = create_shader_prog(d, "./shaders/framebuffer.vert",
			"./shaders/framebuffer.frag");
	glUseProgram(d->framebuffer_shader_prog);
	glUniform1i(glGetUniformLocation(d->framebuffer_shader_prog, "screenTexture"), 2);
	glUniform1i(glGetUniformLocation(d->framebuffer_shader_prog, "normalsTexture"), 3);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	//glfwWindowHint(GLFW_SAMPLES, 4);
	d->w = 800;
	d->h = 600;
	d->window = glfwCreateWindow(d->w, d->h,
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
