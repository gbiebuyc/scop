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
		1, 0, 0, vec[0],
		0, 1, 0, vec[1],
		0, 0, 1, vec[2],
		0, 0, 0, 1});
	return (mat);
}

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

float	*mat_projection(float ratio, float mat[16])
{
	mat_identity(mat);
	mat[5] = 1 / tan(0.5 * FOV * M_PI / 180.0);
	mat[0] = mat[5] / ratio;
	mat[10] = -FAR / (FAR - NEAR);
	mat[14] = -1;
	mat[11] = -(FAR * NEAR) / (FAR - NEAR);
	mat[15] = 0;
	return mat;
}
