/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 17:37:33 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/29 17:37:47 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	handle_events(t_data *d)
{
	float	speed;
	double	now;
	double	dt;

	now = glfwGetTime();
	dt = (now - d->last_frame);
	d->last_frame = now;
	speed = 0.6 * dt;
	if (d->mix_value)
	{
		d->mix_value = fmax(d->mix_value - 1.8 * dt, 0);
		if (d->mix_value == 0)
			recompile_shader_prog(d);
	}
	if (glfwGetKey(d->window, GLFW_KEY_W) == GLFW_PRESS)
		d->pos[Z] -= speed;
	if (glfwGetKey(d->window, GLFW_KEY_S) == GLFW_PRESS)
		d->pos[Z] += speed;
	if (glfwGetKey(d->window, GLFW_KEY_A) == GLFW_PRESS)
		d->pos[X] -= speed;
	if (glfwGetKey(d->window, GLFW_KEY_D) == GLFW_PRESS)
		d->pos[X] += speed;
	if (glfwGetKey(d->window, GLFW_KEY_SPACE) == GLFW_PRESS)
		d->pos[Y] += speed;
	if (glfwGetKey(d->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		d->pos[Y] -= speed;
}

void	framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void	transition_smooth(t_data *d, int targeteffect)
{
	if (d->mix_value)
		return ;
	d->transition[0] = d->transition[1];
	d->transition[1] = (targeteffect == d->transition[0]) ? 0 : targeteffect;
	d->mix_value = 1.0;
	recompile_shader_prog(d);
}

void	transition_switch(t_data *d, int targeteffect)
{
	d->transition[1] = targeteffect;
	recompile_shader_prog(d);
}

#define NUM_EFFECTS 5

void	key_callback(GLFWwindow *window, int key, int scancode, int action)
{
	t_data *d;

	(void)scancode;
	d = glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(d->window, true);
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		transition_smooth(d, 1);
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		transition_switch(d, modulo(d->transition[1] - 1, NUM_EFFECTS));
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		transition_switch(d, modulo(d->transition[1] + 1, NUM_EFFECTS));
}
