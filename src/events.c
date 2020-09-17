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

int		check_key_press_no_repeat(GLFWwindow *window, int key)
{
	static char	keys[GLFW_KEY_LAST + 1];
	int			action;

	action = glfwGetKey(window, key);
	if (action == GLFW_PRESS && keys[key] == GLFW_PRESS)
		return (false);
	keys[key] = (char)action;
	return (action);
}

void	handle_events_bis(t_data *d)
{
	if (check_key_press_no_repeat(d->window, GLFW_KEY_T))
		transition_toggle(d, 1);
	if (check_key_press_no_repeat(d->window, GLFW_KEY_L))
		transition_toggle(d, 2);
}

void	handle_events(t_data *d)
{
	float	speed;
	double	now;
	double	dt;

	now = glfwGetTime();
	dt = (now - d->last_frame);
	d->last_frame = now;
	speed = 3.0 * dt;
	d->mix_value = fmax(d->mix_value - 1.8 * dt, 0);
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
	handle_events_bis(d);
}

void	framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
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
