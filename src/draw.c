/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 14:19:18 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/09/28 14:19:18 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	draw_background(t_data *d)
{
	glUseProgram(d->bg_shader_prog);
	glUniform1i(glGetUniformLocation(d->bg_shader_prog, "isCellShading"),
			(d->transition[1] == EFFECT_CEL_SHADING) ? 1 : 0);
	glBindVertexArray(d->screen_quad_vao);
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

void	draw_model(t_data *d, float *view, float *projection)
{
	float	*model;

	glEnable(GL_DEPTH_TEST);
	if (d->transition[1] == EFFECT_WIREFRAME)
	{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
	model = mat_identity((float[16]){0});
	mat_rotate('x', (float)d->rot_vertical, model);
	mat_rotate('y', (float)d->rot_horizontal, model);
	glUseProgram(d->shader_prog);
	glBindVertexArray(d->model_vao);
	glUniform1f(d->mix_value_loc, d->mix_value);
	glUniform1f(d->time_loc, (float)d->time);
	glUniformMatrix4fv(d->view_loc, 1, GL_TRUE, view);
	glUniformMatrix4fv(d->projection_loc, 1, GL_TRUE, projection);
	glUniformMatrix4fv(d->model_loc, 1, GL_TRUE, model);
	glUniform3f(d->cam_pos_loc, d->pos[0], d->pos[1], d->pos[2]);
	glDrawArrays(GL_TRIANGLES, 0, d->gl_arr_buf.count * 3);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}
