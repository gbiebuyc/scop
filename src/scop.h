/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:21:51 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/01/24 15:21:53 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>
# include <stdint.h>
# include <unistd.h>
# include <string.h>

# define X 0
# define Y 1
# define Z 2

typedef struct	s_dynarray
{
	void		*array;
	size_t		size;
	size_t		capacity;
}				t_dynarray;

typedef struct	s_data
{
	char		*objfilename;
	float		pos[3];
	double		last_frame;
	GLFWwindow	*window;
	t_dynarray	vertices;
	t_dynarray	faces;
	float		vertex_extremes[6];
	float		offset[3];
	int			transition[2];
	float		mix_value;
	GLuint		texture;
	GLuint		shader_prog;
	GLint		model_loc;
	GLint		view_loc;
	GLint		projection_loc;
	GLint		mix_value_loc;
	GLint		transition_loc;
}				t_data;

void			mat_mul(float a[16], float b[16]);
void			mat_print(float mat[16]);
float			*mat_rotate(char axis, float angle, float mat[16]);
float			*mat_translate(float vec[3], float mat[16]);
float			*mat_identity(float mat[16]);
float			*mat_projection(float ratio);
float			*mat_look_at(float pos[3]);
void			parse_obj(t_data *d);
void			load_texture(t_data *d);
void			load_shader_prog(t_data *d);
void			handle_events(t_data *d);
void			framebuffer_size_callback(
	GLFWwindow *window, int width, int height);
void			transition_toggle(t_data *d, int targetstate);
void			key_callback(
	GLFWwindow *window, int key, int scancode, int action);
void			init_gl(t_data *d);

#endif
