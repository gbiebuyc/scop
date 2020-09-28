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
	long	size;
	char	*buf;

	if (!(f = fopen(filename, "rb")))
		exit_scop(d, printf("open fail: %s\n", filename));
	if (fseek(f, 0, SEEK_END) == -1)
		exit_scop_2(d, f, printf("fseek fail\n"));
	if ((size = ftell(f)) == -1)
		exit_scop_2(d, f, printf("ftell fail\n"));
	if (fseek(f, 0, SEEK_SET) == -1)
		exit_scop_2(d, f, printf("fseek fail\n"));
	if (!(buf = malloc(size + 1)))
		exit_scop_2(d, f, printf("malloc fail\n"));
	if (!(fread(buf, size, 1, f)))
		exit_scop_2(d, f, printf("fread fail\n"));
	buf[size] = '\0';
	fclose(f);
	return (buf);
}

void	compile_shader(GLuint shader)
{
	int		success;
	char	info_log[512];

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		printf("%s", info_log);
	}
}

void	link_program(GLuint prog)
{
	int		success;
	char	info_log[512];

	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(prog, 512, NULL, info_log);
		printf("%s", info_log);
	}
}

GLuint	load_shader(t_data *d, char *filename, GLenum shadertype)
{
	GLuint	shader;
	GLchar	*source;

	source = read_file_into_mem(d, filename);
	shader = glCreateShader(shadertype);
	glShaderSource(shader, 1, (const GLchar *const *)&source, NULL);
	compile_shader(shader);
	free(source);
	return (shader);
}

GLuint	create_shader_prog(t_data *d, char *vs, char *fs)
{
	int		vertex_shader;
	int		fragment_shader;
	GLuint	prog;

	vertex_shader = load_shader(d, vs, GL_VERTEX_SHADER);
	fragment_shader = load_shader(d, fs, GL_FRAGMENT_SHADER);
	prog = glCreateProgram();
	glAttachShader(prog, vertex_shader);
	glAttachShader(prog, fragment_shader);
	link_program(prog);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return (prog);
}
