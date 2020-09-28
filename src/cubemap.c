/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubemap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 14:03:43 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/09/28 14:03:43 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	init_cubemap_1(t_data *d)
{
	int			i;
	int			w;
	int			h;
	uint8_t		*data;
	static char *faces[] = {
		"./resources/textures/skybox/right.ppm",
		"./resources/textures/skybox/left.ppm",
		"./resources/textures/skybox/top.ppm",
		"./resources/textures/skybox/bottom.ppm",
		"./resources/textures/skybox/front.ppm",
		"./resources/textures/skybox/back.ppm",
	};

	glGenTextures(1, &d->cubemap_id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, d->cubemap_id);
	i = 0;
	while (i < 6)
	{
		data = read_ppm(d, faces[i], &w, &h);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);
		i++;
	}
}

void	init_cubemap_2(t_data *d)
{
	GLuint vbo;

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenVertexArrays(1, &d->skybox_vao);
	glBindVertexArray(d->skybox_vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6 * 3, (float[]){
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
	}, GL_STATIC_DRAW);
}

void	init_cubemap_3(t_data *d)
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	d->skybox_shader_prog = create_shader_prog(
			d, "./shaders/skybox.vert", "./shaders/skybox.frag");
	d->skybox_view_loc = glGetUniformLocation(d->skybox_shader_prog, "view");
	d->skybox_projection_loc = glGetUniformLocation(
			d->skybox_shader_prog, "projection");
	glUseProgram(d->skybox_shader_prog);
	glUniform1i(glGetUniformLocation(d->skybox_shader_prog, "skybox"), 1);
}

void	init_cubemap(t_data *d)
{
	init_cubemap_1(d);
	init_cubemap_2(d);
	init_cubemap_3(d);
}
