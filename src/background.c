/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 13:01:29 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/02/23 13:01:29 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	init_background(t_data *d)
{
	GLuint vbo;

	glGenVertexArrays(1, &d->bg_vao);
	glBindVertexArray(d->bg_vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2,
		(float[8]){-1, -1, -1, 1, 1, 1, 1, -1}, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	d->bg_shader_prog = create_shader_prog(d, "./shaders/background.vert",
			"./shaders/background.frag");
}
