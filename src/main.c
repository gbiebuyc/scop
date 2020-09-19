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
	d->objfilename = av[1];
}

void	draw_background(t_data *d)
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(d->bg_shader_prog);
	glBindVertexArray(d->bg_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glEnable(GL_DEPTH_TEST);
}

void	draw_model(t_data *d)
{
	float	*model;
	float	*view;
	float	*projection;
	int		width;
	int		height;

	glUseProgram(d->shader_prog);
	glBindVertexArray(d->model_vao);
	glfwGetFramebufferSize(d->window, &width, &height);
	view = mat_look_at(d->pos);
	projection = mat_projection(width / (float)height);
	model = mat_identity((float[16]){0});
	model = mat_rotate('y', (float)glfwGetTime() * 0.5, model);
	glUniform1f(d->mix_value_loc, d->mix_value);
	glUniform1iv(d->transition_loc, 2, d->transition);
	glUniformMatrix4fv(d->view_loc, 1, GL_TRUE, view);
	glUniformMatrix4fv(d->projection_loc, 1, GL_TRUE, projection);
	glUniformMatrix4fv(d->model_loc, 1, GL_TRUE, model);
	glDrawElements(GL_TRIANGLES, d->faces.size / sizeof(uint32_t),
		GL_UNSIGNED_INT, 0);
}

void	loop(t_data *d)
{
	while (!glfwWindowShouldClose(d->window))
	{
		glfwPollEvents();
		handle_events(d);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw_background(d);
		draw_model(d);
		glfwSwapBuffers(d->window);
	}
}

int		main(int ac, char **av)
{
	t_data	*d;

	if (!(d = calloc(1, sizeof(t_data))))
		exit(printf("calloc fail\n"));
	parse_args(d, ac, av);
	parse_obj(d);
	init_gl(d);
	load_shader_prog(d);
	load_texture(d);
	init_background(d);
	d->pos[X] = 0;
	d->pos[Y] = 0;
	d->pos[Z] = 10;
	loop(d);
	exit_scop(d, EXIT_SUCCESS);
	return (0);
}
