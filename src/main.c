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
	while (!glfwWindowShouldClose(d->window))
	{
		glfwPollEvents();
		handle_events(d);
		glBindFramebuffer(GL_FRAMEBUFFER, d->fbo);
		glViewport(0, 0, d->w, d->h);
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		d->view = mat_look_at(d->pos);
		d->projection = mat_projection(d->w / (float)d->h);
		if (d->transition[1] == EFFECT_REFLECTION ||
				d->transition[1] == EFFECT_REFRACTION)
			draw_skybox(d, d->view, d->projection);
		else
			draw_background(d);
		draw_model(d, d->view, d->projection);
		draw_framebuffer(d);
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
	init_effects(d);
	d->model_vs = load_shader(d, "./shaders/model.vert", GL_VERTEX_SHADER);
	d->model_fs_source = read_file_into_mem(d, "./shaders/model.frag");
	recompile_shader_prog(d);
	load_texture(d);
	init_cubemap(d);
	init_background(d);
	d->pos[X] = 0;
	d->pos[Y] = 0;
	d->pos[Z] = 1;
	d->mouse_xpos = NAN;
	loop(d);
	exit_scop(d, EXIT_SUCCESS);
	return (0);
}
