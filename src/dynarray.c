/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynarray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 16:49:19 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/09/19 16:49:19 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	dynarray_append(t_data *d, t_dynarray *da, void *elem, size_t elemsize)
{
	size_t new_capacity;

	if (!(new_capacity = da->capacity))
		new_capacity = 64;
	while (new_capacity < da->size + elemsize)
		new_capacity *= 2;
	if (da->capacity != new_capacity)
	{
		da->array = (da->capacity == 0) ?
			malloc(new_capacity) :
			realloc(da->array, new_capacity);
		if (!da->array)
			exit_scop_2(d, d->f, printf("malloc fail\n"));
		da->capacity = new_capacity;
	}
	memcpy((char*)da->array + da->size, elem, elemsize);
	da->size += elemsize;
	da->count++;
}
