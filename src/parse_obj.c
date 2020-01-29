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

void	array_append(t_data *d, t_dynarray *da, void *elem, size_t elemsize)
{
	if (da->capacity == 0)
	{
		da->capacity = 64;
		if (!(da->array = malloc(da->capacity)))
			exit_scop(d, printf("malloc fail\n"));
	}
	if (da->size + elemsize > da->capacity)
	{
		da->capacity *= 2;
		if (!(da->array = realloc(da->array, da->capacity)))
			exit_scop(d, printf("realloc fail\n"));
	}
	memcpy((char*)da->array + da->size, elem, elemsize);
	da->size += elemsize;
}

void	add_vertex(t_data *d, float v[3])
{
	d->vertex_extremes[0] = fminf(v[0], d->vertex_extremes[0]);
	d->vertex_extremes[1] = fminf(v[1], d->vertex_extremes[1]);
	d->vertex_extremes[2] = fminf(v[2], d->vertex_extremes[2]);
	d->vertex_extremes[3] = fmaxf(v[0], d->vertex_extremes[3]);
	d->vertex_extremes[4] = fmaxf(v[1], d->vertex_extremes[4]);
	d->vertex_extremes[5] = fmaxf(v[2], d->vertex_extremes[5]);
	array_append(d, &d->vertices, v, sizeof(float) * 3);
}

void	face(t_data *d, char *line)
{
	int f[3];

	f[0] = strtol(line, &line, 10) - 1;
	line = strchr(line, ' ');
	f[1] = strtol(line, &line, 10) - 1;
	line = strchr(line, ' ');
	while (line && (f[2] = strtol(line, &line, 10)))
	{
		f[2]--;
		array_append(d, &d->faces, f, sizeof(int) * 3);
		f[1] = f[2];
		line = strchr(line, ' ');
	}
}

void	parse_obj(t_data *d)
{
	FILE	*fp;
	char	line[100];
	float	v[3];

	fp = fopen(d->objfilename, "rb");
	while (fgets(line, 100, fp))
	{
		if (sscanf(line, "v %f %f %f", v, &v[1], &v[2]) == 3)
			add_vertex(d, v);
		else if (strncmp(line, "f ", 2) == 0)
			face(d, line + 2);
	}
	d->offset[0] = -(d->vertex_extremes[3] + d->vertex_extremes[0]) / 2.0;
	d->offset[1] = -(d->vertex_extremes[4] + d->vertex_extremes[1]) / 2.0;
	d->offset[2] = -(d->vertex_extremes[5] + d->vertex_extremes[2]) / 2.0;
	d->pos[Z] = fmaxf(d->vertex_extremes[3] - d->vertex_extremes[0],
		d->vertex_extremes[5] - d->vertex_extremes[2]);
	fclose(fp);
}
