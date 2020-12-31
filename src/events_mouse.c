/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 06:26:29 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/12/31 06:26:29 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	t_data	*d;
	double	zoom;

	(void)xoffset;
	d = glfwGetWindowUserPointer(window);
	if (yoffset)
	{
		zoom = d->pos[Z] * 0.05;
		zoom = fmax(zoom, 0.02);
		d->pos[Z] -= yoffset * zoom;
	}
}

void	handle_mouse(t_data *d, double dt)
{
	double xpos;
	double ypos;

	if (glfwGetMouseButton(d->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwGetCursorPos(d->window, &xpos, &ypos);
		if (!isnan(d->mouse_xpos))
		{
			d->rot_horizontal += (xpos - d->mouse_xpos) * 0.002;
			d->rot_vertical += (ypos - d->mouse_ypos) * 0.002;
			if (d->rot_vertical < -M_PI_2)
				d->rot_vertical = -M_PI_2;
			else if (d->rot_vertical > M_PI_2)
				d->rot_vertical = M_PI_2;
		}
		d->mouse_xpos = xpos;
		d->mouse_ypos = ypos;
	}
	else
	{
		d->mouse_xpos = NAN;
		d->rot_horizontal += fmin(dt, 0.060) * 0.5;
	}
}
