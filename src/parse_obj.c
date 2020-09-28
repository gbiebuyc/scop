/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 12:17:00 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/27 12:17:02 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	add_vertex(t_data *d, float v[3])
{
	d->vertex_extremes[0] = fminf(v[0], d->vertex_extremes[0]);
	d->vertex_extremes[1] = fminf(v[1], d->vertex_extremes[1]);
	d->vertex_extremes[2] = fminf(v[2], d->vertex_extremes[2]);
	d->vertex_extremes[3] = fmaxf(v[0], d->vertex_extremes[3]);
	d->vertex_extremes[4] = fmaxf(v[1], d->vertex_extremes[4]);
	d->vertex_extremes[5] = fmaxf(v[2], d->vertex_extremes[5]);
	dynarray_append(d, &d->vertices, v, sizeof(float) * 3);
}

char	*parse_vertex(t_data *d, float *dest, char *line)
{
	int i;

	dest[3] = NAN;
	dest[6] = NAN;
	if (!(line = strchr(line, ' ')))
		return (NULL);
	i = strtol(line, &line, 10) - 1;
	if (i < 0 || i >= d->vertices.count)
		return (NULL);
	memcpy(dest, ((float*)d->vertices.array) + i * 3, 3 * sizeof(float));
	if (*line != '/')
		return (line);
	i = strtol(line + 1, &line, 10) - 1;
	if (i < 0 || i >= d->tex_coords.count)
		return (NULL);
	memcpy(dest + 6, ((float*)d->tex_coords.array) + i * 2, 2 * sizeof(float));
	if (*line != '/')
		return (line);
	i = strtol(line + 1, &line, 10) - 1;
	if (i < 0 || i >= d->normals.count)
		return (NULL);
	memcpy(dest + 3, ((float*)d->normals.array) + i * 3, 3 * sizeof(float));
	return (line);
}

void	face(t_data *d, char *line)
{
	float v[3][8];

	if (!(line = parse_vertex(d, v[0], line)))
		return ;
	if (!(line = parse_vertex(d, v[1], line)))
		return ;
	while ((line = parse_vertex(d, v[2], line)))
	{
		dynarray_append(d, &d->gl_arr_buf, v, sizeof(v));
		memcpy(v[1], v[2], sizeof(v[2]));
	}
}

void	parse_obj(t_data *d)
{
	char	line[256];
	float	v[3];
	char	tex_file[256];

	if (!(d->f = fopen(d->obj_path, "rb")))
		exit_scop(d, printf("fopen error: %s\n", d->obj_path));
	tex_file[0] = '\0';
	while (fgets(line, 256, d->f))
	{
		if (sscanf(line, "v %f %f %f", v, &v[1], &v[2]) == 3)
			add_vertex(d, v);
		else if (sscanf(line, "vn %f %f %f", v, &v[1], &v[2]) == 3)
			dynarray_append(d, &d->normals, v, sizeof(float) * 3);
		else if (sscanf(line, "vt %f %f", v, &v[1]) == 2)
			dynarray_append(d, &d->tex_coords, v, sizeof(float) * 2);
		else if (strncmp(line, "f ", 2) == 0)
			face(d, line);
		else
			sscanf(line, "scopTexture %255s", tex_file);
	}
	fclose(d->f);
	if (d->vertices.size == 0)
		printf("This file contains no geometry data.\n");
	center_and_scale_vertices(d);
	assemble_tex_path(d, tex_file);
}
