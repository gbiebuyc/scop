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

void	loop(t_data *d)
{
	float	*view;
	float	*projection;

	while (!glfwWindowShouldClose(d->window))
	{
		glfwPollEvents();
		handle_events(d);
		glBindFramebuffer(GL_FRAMEBUFFER, d->fbo);
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetFramebufferSize(d->window, &d->w, &d->h);
		view = mat_look_at(d->pos);
		projection = mat_projection(d->w / (float)d->h);
		if (d->transition[1] >= 3)
			draw_skybox(d, view, projection);
		else
			draw_background(d);
		draw_model(d, view, projection);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(d->framebuffer_shader_prog);
		glBindVertexArray(d->screen_quad_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
	d->model_vs = load_shader(d, "./shaders/model.vert", GL_VERTEX_SHADER);
	d->model_fs_source = read_file_into_mem(d, "./shaders/model.frag");
	recompile_shader_prog(d);
	load_texture(d);
	init_cubemap(d);
	init_background(d);
	d->pos[X] = 0;
	d->pos[Y] = 0;
	d->pos[Z] = 1;
	loop(d);
	exit_scop(d, EXIT_SUCCESS);
	return (0);
}
