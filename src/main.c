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
		exit_scop(d, printf("usage example: ./scop resources/42.obj\n\n"));
	d->obj_path = av[1];
}

void	draw_background(t_data *d)
{
	glUseProgram(d->bg_shader_prog);
	glBindVertexArray(d->bg_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void	draw_skybox(t_data *d, float *view, float *projection)
{
	glUseProgram(d->skybox_shader_prog);
	glBindVertexArray(d->skybox_vao);
	glUniformMatrix4fv(d->skybox_view_loc, 1, GL_TRUE, view);
	glUniformMatrix4fv(d->skybox_projection_loc, 1, GL_TRUE, projection);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
}

void	draw_model(t_data *d)
{
	float	*model;
	float	*view;
	float	*projection;
	int		width;
	int		height;

	glfwGetFramebufferSize(d->window, &width, &height);
	view = mat_look_at(d->pos);
	projection = mat_projection(width / (float)height);
	model = mat_identity((float[16]){0});
	model = mat_rotate('y', (float)glfwGetTime() * 0.5, model);
	if (d->transition[1] >= 3)
		draw_skybox(d, view, projection);
	else
		draw_background(d);
	glEnable(GL_DEPTH_TEST);
	if (d->transition[1] == 2) {
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
	glUseProgram(d->shader_prog);
	glBindVertexArray(d->model_vao);
	glUniform1f(d->mix_value_loc, d->mix_value);
	glUniform1iv(d->transition_loc, 2, d->transition);
	glUniformMatrix4fv(d->view_loc, 1, GL_TRUE, view);
	glUniformMatrix4fv(d->projection_loc, 1, GL_TRUE, projection);
	glUniformMatrix4fv(d->model_loc, 1, GL_TRUE, model);
	glUniform3f(d->cam_pos_loc, d->pos[0], d->pos[1], d->pos[2]);
	glDrawArrays(GL_TRIANGLES, 0, d->gl_arr_buf.count * 3);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void	loop(t_data *d)
{
	while (!glfwWindowShouldClose(d->window))
	{
		glfwPollEvents();
		handle_events(d);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_model(d);
		glfwSwapBuffers(d->window);
	}
}

uint8_t	*read_ppm(t_data *d, char *filename, int *w, int *h);

void	cubemap(t_data *d) {
	glGenTextures(1, &d->cubemap_id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, d->cubemap_id);
	int		w;
	int		h;
	uint8_t	*data;
	char *faces[] = {
		"./resources/textures/skybox/right.ppm",
		"./resources/textures/skybox/left.ppm",
		"./resources/textures/skybox/top.ppm",
		"./resources/textures/skybox/bottom.ppm",
		"./resources/textures/skybox/front.ppm",
		"./resources/textures/skybox/back.ppm",
	};
	for (int i=0; i<6; i++) {
		data = read_ppm(d, faces[i], &w, &h);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	float skybox_vertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	GLuint vbo;
	glGenVertexArrays(1, &d->skybox_vao);
	glBindVertexArray(d->skybox_vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6 * 3, skybox_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	d->skybox_shader_prog = create_shader_prog(
			d, "./shaders/skybox.vert", "./shaders/skybox.frag");
	d->skybox_view_loc = glGetUniformLocation(d->skybox_shader_prog, "view");
	d->skybox_projection_loc = glGetUniformLocation(d->skybox_shader_prog, "projection");
	glUseProgram(d->skybox_shader_prog);
	glUniform1i(glGetUniformLocation(d->skybox_shader_prog, "skybox"), 1);
}

void	get_uniform_locations(t_data *d)
{
	d->model_loc = glGetUniformLocation(d->shader_prog, "model");
	d->view_loc = glGetUniformLocation(d->shader_prog, "view");
	d->projection_loc = glGetUniformLocation(d->shader_prog, "projection");
	d->mix_value_loc = glGetUniformLocation(d->shader_prog, "mix_value");
	d->transition_loc = glGetUniformLocation(d->shader_prog, "transition");
	d->cam_pos_loc = glGetUniformLocation(d->shader_prog, "cameraPos");
	glUseProgram(d->shader_prog);
	glUniform1i(glGetUniformLocation(d->shader_prog, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(d->shader_prog, "skybox"), 1);
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

char	*get_effect(int i)
{
	if (i == 0)
		return ("effect_shades_of_grey();");
	if (i == 1)
		return ("effect_lighting(effect_texture().rgb);");
	if (i == 2)
		return ("effect_wireframe();");
	if (i == 3)
		return ("effect_reflection();");
	if (i == 4)
		return ("effect_refraction();");
	return NULL;
}

void	recompile_shader_prog(t_data *d)
{
	GLuint	shader;
	char	*source[7];

	if (d->shader_prog)
		glDeleteProgram(d->shader_prog);
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	source[0] = d->model_fs_source;
	source[1] = "void main() {";
	if (d->mix_value) {
		source[2] = "vec4 effect_0 = ";
		source[3] = get_effect(d->transition[0]);
		source[4] = "vec4 effect_1 = ";
		source[5] = get_effect(d->transition[1]);
		source[6] = "FragColor = mix(effect_1, effect_0, mix_value);}";
		glShaderSource(shader, 7, (const GLchar *const *)source, NULL);
	} else {
		source[2] = "FragColor = ";
		source[3] = get_effect(d->transition[1]);
		source[4] = "}";
		glShaderSource(shader, 5, (const GLchar *const *)source, NULL);
	}
	compile_shader(shader);
	d->shader_prog = glCreateProgram();
	glAttachShader(d->shader_prog, d->model_vs);
	glAttachShader(d->shader_prog, shader);
	link_program(d->shader_prog);
	glDeleteShader(shader);
	get_uniform_locations(d);
	puts("recompiled yay");
	fflush(stdout);
}

int		main(int ac, char **av)
{
	t_data	*d;

	if (!(d = calloc(1, sizeof(t_data))))
		exit(printf("calloc fail\n"));
	parse_args(d, ac, av);
	parse_obj(d);
	init_gl(d);
	d->model_vs = load_shader(d, "./shaders/model.vert", GL_VERTEX_SHADER);
	d->model_fs_source = read_file_into_mem(d, "./shaders/model.frag");
	recompile_shader_prog(d);
	load_texture(d);
	cubemap(d);
	init_background(d);
	d->pos[X] = 0;
	d->pos[Y] = 0;
	d->pos[Z] = 1;
	loop(d);
	exit_scop(d, EXIT_SUCCESS);
	return (0);
}
