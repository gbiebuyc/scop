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

#include <math.h>
#include <stdio.h>
#include <string.h>

void	mat_mul(float a[][4], float b[][4])
{
	float	result[4][4];
	int		i;
	int		j;
	int		k;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			result[i][j] = 0;
			k = 0;
			while (k < 4)
			{
				result[i][j] += a[i][k] * b[k][j];
				k++;
			}
			j++;
		}
		i++;
	}
	memcpy(a, result, sizeof(float) * 16);
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

void	mat_rot_z(float angle, float mat[][4])
{
	float sin_a;
	float cos_a;

	sin_a = sin(angle);
	cos_a = cos(angle);
	mat_mul(mat, (float[16]){
		cos_a, -sin_a, 0, 0,
		sin_a, cos_a, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	});
}
