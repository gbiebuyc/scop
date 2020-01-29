/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_shader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 16:27:24 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/29 16:27:27 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

char	*read_file_into_mem(t_data *d, char *filename)
{
	FILE	*f;
	size_t	size;
	char	*buf;

	if (!(f = fopen(filename, "rb")))
		exit_scop(d, printf("open fail: %s\n", filename));
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (!(buf = malloc(size + 1)))
		exit_scop_2(d, f, printf("malloc fail\n"));
	fread(buf, size, 1, f);
	buf[size] = '\0';
	fclose(f);
	return (buf);
}

GLuint	load_shader(t_data *d, char *filename, GLenum shadertype)
{
	int		success;
	char	info_log[512];
	GLuint	shader;
	GLchar	*source;

	source = read_file_into_mem(d, filename);
	shader = glCreateShader(shadertype);
	glShaderSource(shader, 1, (const GLchar *const *)&source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		printf("error in %s:\n%s", filename, info_log);
	}
	free(source);
	return (shader);
}

void	get_uniform_locations(t_data *d)
{
	d->model_loc = glGetUniformLocation(d->shader_prog, "model");
	d->view_loc = glGetUniformLocation(d->shader_prog, "view");
	d->projection_loc = glGetUniformLocation(d->shader_prog, "projection");
	d->mix_value_loc = glGetUniformLocation(d->shader_prog, "mix_value");
	d->transition_loc = glGetUniformLocation(d->shader_prog, "transition");
}

void	load_shader_prog(t_data *d)
{
	int		success;
	char	info_log[512];
	int		vertex_shader;
	int		fragment_shader;

	vertex_shader = load_shader(d,
		"./resources/vertexshader.glsl", GL_VERTEX_SHADER);
	fragment_shader = load_shader(d,
		"./resources/fragmentshader.glsl", GL_FRAGMENT_SHADER);
	d->shader_prog = glCreateProgram();
	glAttachShader(d->shader_prog, vertex_shader);
	glAttachShader(d->shader_prog, fragment_shader);
	glLinkProgram(d->shader_prog);
	glGetProgramiv(d->shader_prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(d->shader_prog, 512, NULL, info_log);
		printf("%s", info_log);
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glUseProgram(d->shader_prog);
	get_uniform_locations(d);
}
