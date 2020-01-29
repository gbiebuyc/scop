/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrices.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 11:01:27 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/24 11:01:30 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

/*
**	Matrices are stored in row-major order.
*/

float	*mat_identity(float mat[16])
{
	return (memcpy(mat, ((float[16]){
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1}), sizeof(float) * 16));
}

#define FOV 60
#define NEAR 0.1
#define FAR 100

float	*mat_projection(float ratio)
{
	static float mat[16];

	mat_identity(mat);
	mat[5] = 1 / tan(0.5 * FOV * M_PI / 180.0);
	mat[0] = mat[5] / ratio;
	mat[10] = -FAR / (FAR - NEAR);
	mat[14] = -1;
	mat[11] = -(FAR * NEAR) / (FAR - NEAR);
	mat[15] = 0;
	return (mat);
}

float	*mat_look_at(float pos[3])
{
	static float mat[16];

	mat_identity(mat);
	mat_translate((float[3]){-pos[X], -pos[Y], -pos[Z]}, mat);
	return (mat);
}

void	mat_print(float mat[16])
{
	int i;

	i = 0;
	while (i < 16)
	{
		printf("%f%c", mat[i], (i % 4 == 3) ? '\n' : ' ');
		i++;
	}
}
