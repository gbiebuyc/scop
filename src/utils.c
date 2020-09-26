/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 07:05:39 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/09/21 07:05:39 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int	modulo(int a, int b)
{
	int result;

	result = a % b;
	return ((result >= 0) ? (result) : (result + b));
}