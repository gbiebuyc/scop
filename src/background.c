/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 13:01:29 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/23 13:01:29 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	bg_load_shader_prog(t_data *d)
{
	int		success;
	char	info_log[512];
	int		vertex_shader;
	int		fragment_shader;

	vertex_shader = load_shader(d,
		"./resources/bg_vshader.glsl", GL_VERTEX_SHADER);
	fragment_shader = load_shader(d,
		"./resources/bg_fshader.glsl", GL_FRAGMENT_SHADER);
	d->bg_shader_prog = glCreateProgram();
	glAttachShader(d->bg_shader_prog, vertex_shader);
	glAttachShader(d->bg_shader_prog, fragment_shader);
	glLinkProgram(d->bg_shader_prog);
	glGetProgramiv(d->bg_shader_prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(d->bg_shader_prog, 512, NULL, info_log);
		printf("%s", info_log);
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void	init_background(t_data *d)
{
	GLuint vbo;

	glGenVertexArrays(1, &d->bg_vao);
	glBindVertexArray(d->bg_vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2,
		(float[8]){-1, -1, -1, 1, 1, 1, 1, -1}, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	bg_load_shader_prog(d);
}
