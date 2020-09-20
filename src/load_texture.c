/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 16:13:09 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/29 16:13:27 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

uint8_t	*read_ppm(t_data *d, char *filename, int *w, int *h)
{
	FILE	*f;
	char	buf[255];
	uint8_t *img_buf;

	if (!(f = fopen(filename, "rb")))
		exit_scop(d, printf("fopen failed %s\n", filename));
	if (!fgets(buf, 255, f) || (strncmp(buf, "P6\n", 3) != 0))
		exit_scop_2(d, f, printf("image read error 1"));
	if (!fgets(buf, 255, f))
		exit_scop_2(d, f, printf("image read error 1 bis"));
	while (strncmp(buf, "#", 1) == 0)
		if (!fgets(buf, 255, f))
			exit_scop_2(d, f, printf("image read error 2"));
	if (sscanf(buf, "%u %u", w, h) < 2)
		exit_scop_2(d, f, printf("image read error 3"));
	if (!fgets(buf, 255, f))
		exit_scop_2(d, f, printf("image read error 4"));
	if (!(img_buf = malloc(*w * *h * 3)))
		exit_scop_2(d, f, printf("image read error 5"));
	if (!(fread(img_buf, 1, *w * *h * 3, f)))
		exit_scop_2(d, f, printf("image read error 6"));
	fclose(f);
	return (img_buf);
}

void	load_texture(t_data *d)
{
	int		w;
	int		h;
	uint8_t	*data;

	data = read_ppm(d, d->tex_path, &w, &h);
	glGenTextures(1, &d->texture);
	glBindTexture(GL_TEXTURE_2D, d->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(data);
}
