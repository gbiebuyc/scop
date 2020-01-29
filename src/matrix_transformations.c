/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_transformations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 15:56:22 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/29 15:56:53 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	mat_mul(float a[16], float b[16])
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
				result[i][j] += a[i * 4 + k] * b[k * 4 + j];
				k++;
			}
			j++;
		}
		i++;
	}
	memcpy(a, result, sizeof(float) * 16);
}

float	*mat_rotate(char axis, float angle, float mat[16])
{
	float si;
	float co;

	si = sin(angle);
	co = cos(angle);
	if (axis == 'x')
		mat_mul(mat, (float[16]){
			1, 0, 0, 0,
			0, co, -si, 0,
			0, si, co, 0,
			0, 0, 0, 1});
	else if (axis == 'y')
		mat_mul(mat, (float[16]){
			co, 0, si, 0,
			0, 1, 0, 0,
			-si, 0, co, 0,
			0, 0, 0, 1});
	else if (axis == 'z')
		mat_mul(mat, (float[16]){
			co, -si, 0, 0,
			si, co, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1});
	return (mat);
}

float	*mat_translate(float vec[3], float mat[16])
{
	mat_mul(mat, (float[16]){
		1, 0, 0, vec[X],
		0, 1, 0, vec[Y],
		0, 0, 1, vec[Z],
		0, 0, 0, 1});
	return (mat);
}
