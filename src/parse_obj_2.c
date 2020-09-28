/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 14:59:53 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/09/28 14:59:53 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	center_and_scale_vertices(t_data *d)
{
	int		i;
	int		count;
	float	offset[3];
	float	scale;
	float	magnitude;

	offset[0] = -(d->vertex_extremes[3] + d->vertex_extremes[0]) / 2.0;
	offset[1] = -(d->vertex_extremes[4] + d->vertex_extremes[1]) / 2.0;
	offset[2] = -(d->vertex_extremes[5] + d->vertex_extremes[2]) / 2.0;
	magnitude = d->vertex_extremes[3] - d->vertex_extremes[0];
	magnitude = fmaxf(magnitude, d->vertex_extremes[4] - d->vertex_extremes[1]);
	magnitude = fmaxf(magnitude, d->vertex_extremes[5] - d->vertex_extremes[2]);
	scale = 1.0f / magnitude;
	i = 0;
	count = d->gl_arr_buf.count * 3 * 8;
	while (i < count)
	{
		((float*)d->gl_arr_buf.array)[i + 0] += offset[0];
		((float*)d->gl_arr_buf.array)[i + 0] *= scale;
		((float*)d->gl_arr_buf.array)[i + 1] += offset[1];
		((float*)d->gl_arr_buf.array)[i + 1] *= scale;
		((float*)d->gl_arr_buf.array)[i + 2] += offset[2];
		((float*)d->gl_arr_buf.array)[i + 2] *= scale;
		i += 8;
	}
}

void	assemble_tex_path(t_data *d, char *tex_file)
{
	char *tmp;
	char *obj_dirname;

	if (!(tmp = strdup(d->obj_path)))
		exit_scop(d, printf("strdup fail\n"));
	obj_dirname = dirname(tmp);
	if (!strlen(tex_file))
		tex_file = "textures/default.ppm";
	if (!(d->tex_path = malloc(strlen(obj_dirname) + strlen(tex_file) + 2)))
		exit_scop(d, printf("malloc fail\n"));
	strcpy(d->tex_path, obj_dirname);
	strcat(d->tex_path, "/");
	strcat(d->tex_path, tex_file);
	free(tmp);
}
