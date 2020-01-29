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

	now = glfwGetTime();
	speed = 6 * (now - d->last_frame);
	d->last_frame = now;
	d->mix_value = fmax(d->mix_value - 0.3 * speed, 0);
	if (glfwGetKey(d->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(d->window, true);
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
	glViewport(0, 0, width, height);
}

void	transition_toggle(t_data *d, int targetstate)
{
	if (d->mix_value)
		return ;
	d->transition[0] = d->transition[1];
	d->transition[1] = (targetstate == d->transition[0]) ? 0 : targetstate;
	d->mix_value = 1.0;
}

void	key_callback(GLFWwindow *window, int key, int scancode, int action)
{
	t_data *d;

	d = glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		transition_toggle(d, 1);
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		transition_toggle(d, 2);
}
