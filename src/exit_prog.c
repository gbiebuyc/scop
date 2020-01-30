/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_prog.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 01:03:33 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/30 01:03:33 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	exit_scop(t_data *d, int exit_status)
{
	glfwDestroyWindow(d->window);
	glfwTerminate();
	free(d->vertices.array);
	free(d->faces.array);
	free(d);
	exit(exit_status);
}

void	exit_scop_2(t_data *d, FILE *f, int exit_status)
{
	fclose(f);
	exit_scop(d, exit_status);
}
