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

void    array_append(t_dynarray *da, void *elem, size_t elemsize)
{
    if (da->capacity == 0)
    {
        da->capacity = 64;
        if (!(da->array = malloc(da->capacity)))
            exit(printf("malloc fail\n"));
    }
    if (da->size + elemsize > da->capacity)
    {
        da->capacity *= 2;
        if (!(da->array = realloc(da->array, da->capacity)))
            exit(printf("realloc fail\n"));
    }
    memcpy((char*)da->array + da->size, elem, elemsize);
    da->size += elemsize;
}

void	face(t_data *d, int f[3])
{
    f[0]--;
    f[1]--;
    f[2]--;
	array_append(&d->faces, f, sizeof(int) * 3);
}

void    parse_obj(t_data *d)
{
    FILE    *fp;
    char    line[100];
    float   v[3];
    int     f[3];

    fp = fopen(d->objfilename, "r");
    while(fgets(line, 100, fp))
    {
        if (sscanf(line, "v %f %f %f", v, &v[1], &v[2]) == 3)
			array_append(&d->vertices, v, sizeof(v));
        else if (sscanf(line, "f %d %d %d", f, &f[1], &f[2]) == 3)
			face(d, f);
    }
    fclose(fp);
}
