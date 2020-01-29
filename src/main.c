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

void	draw(t_data *d)
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
	model = mat_translate(d->model_center_offset, model);
	glUniform1f(d->mix_value_loc, d->mix_value);
	glUniform1iv(d->transition_loc, 2, d->transition);
	glUniformMatrix4fv(d->view_loc, 1, GL_TRUE, view);
	glUniformMatrix4fv(d->projection_loc, 1, GL_TRUE, projection);
	glUniformMatrix4fv(d->model_loc, 1, GL_TRUE, model);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, d->faces.size / sizeof(uint32_t),
		GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(d->window);
}

void	loop(t_data *d)
{
	while (!glfwWindowShouldClose(d->window))
	{
		glfwPollEvents();
		handle_events(d);
		draw(d);
	}
}

int		main(int ac, char **av)
{
	t_data	*d;

	d = calloc(1, sizeof(t_data));
	d->pos[2] = 3;
	parse_args(d, ac, av);
	parse_obj(d);
	init_gl(d);
	load_shader_prog(d);
	load_texture(d);
	loop(d);
	glfwTerminate();
	free(d);
	return (0);
}
