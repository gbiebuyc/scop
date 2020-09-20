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
# include <libgen.h>

# define X 0
# define Y 1
# define Z 2

typedef struct	s_dynarray
{
	void		*array;
	size_t		size;
	size_t		capacity;
	long long	count;
}				t_dynarray;

typedef struct	s_data
{
	char		*obj_path;
	char		*tex_path;
	float		pos[3];
	double		last_frame;
	GLFWwindow	*window;
	t_dynarray	vertices;
	t_dynarray	normals;
	t_dynarray	tex_coords;
	t_dynarray	gl_arr_buf;
	float		vertex_extremes[6];
	int			transition[2];
	float		mix_value;
	GLuint		texture;
	GLuint		shader_prog;
	GLuint		bg_shader_prog;
	GLint		model_loc;
	GLint		view_loc;
	GLint		projection_loc;
	GLint		mix_value_loc;
	GLint		transition_loc;
	FILE		*f;
	GLuint		model_vao;
	GLuint		bg_vao;
}				t_data;

void			mat_mul(float a[16], float b[16]);
void			mat_print(float mat[16]);
float			*mat_rotate(char axis, float angle, float mat[16]);
float			*mat_translate(float vec[3], float mat[16]);
float			*mat_scale(float scale, float mat[16]);
float			*mat_identity(float mat[16]);
float			*mat_projection(float ratio);
float			*mat_look_at(float pos[3]);
void			parse_obj(t_data *d);
void			load_texture(t_data *d);
GLuint			load_shader(t_data *d, char *filename, GLenum shadertype);
void			load_shader_prog(t_data *d);
void			handle_events(t_data *d);
void			framebuffer_size_callback(GLFWwindow *window, int w, int h);
void			transition_toggle(t_data *d, int targetstate);
void			init_gl(t_data *d);
void			exit_scop(t_data *d, int exit_status);
void			exit_scop_2(t_data *d, FILE *f, int exit_status);
void			init_background(t_data *d);
void			dynarray_append(t_data *d, t_dynarray *da, void *e, size_t sz);

#endif
