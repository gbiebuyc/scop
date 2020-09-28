/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   model.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 14:01:37 by gbiebuyc          #+#    #+#             */
/*   Updated: 2020/09/28 14:01:37 by gbiebuyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	get_uniform_locations(t_data *d)
{
	d->model_loc = glGetUniformLocation(d->shader_prog, "model");
	d->view_loc = glGetUniformLocation(d->shader_prog, "view");
	d->projection_loc = glGetUniformLocation(d->shader_prog, "projection");
	d->mix_value_loc = glGetUniformLocation(d->shader_prog, "mix_value");
	d->transition_loc = glGetUniformLocation(d->shader_prog, "transition");
	d->cam_pos_loc = glGetUniformLocation(d->shader_prog, "cameraPos");
	glUseProgram(d->shader_prog);
	glUniform1i(glGetUniformLocation(d->shader_prog, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(d->shader_prog, "skybox"), 1);
}

char	*get_effect(int i)
{
	if (i == 0)
		return ("effect_shades_of_grey();");
	if (i == 1)
		return ("effect_lighting(effect_texture().rgb);");
	if (i == 2)
		return ("effect_wireframe();");
	if (i == 3)
		return ("effect_reflection();");
	if (i == 4)
		return ("effect_refraction();");
	return (NULL);
}

void	recompile_shader_prog_2(t_data *d, GLuint shader)
{
	compile_shader(shader);
	d->shader_prog = glCreateProgram();
	glAttachShader(d->shader_prog, d->model_vs);
	glAttachShader(d->shader_prog, shader);
	link_program(d->shader_prog);
	glDeleteShader(shader);
	get_uniform_locations(d);
}

void	recompile_shader_prog(t_data *d)
{
	GLuint	shader;
	char	*source[7];

	if (d->shader_prog)
		glDeleteProgram(d->shader_prog);
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	source[0] = d->model_fs_source;
	source[1] = "void main() {";
	if (d->mix_value)
	{
		source[2] = "vec4 effect_0 = ";
		source[3] = get_effect(d->transition[0]);
		source[4] = "vec4 effect_1 = ";
		source[5] = get_effect(d->transition[1]);
		source[6] = "FragColor = mix(effect_1, effect_0, mix_value);}";
		glShaderSource(shader, 7, (const GLchar *const *)source, NULL);
	}
	else
	{
		source[2] = "FragColor = ";
		source[3] = get_effect(d->transition[1]);
		source[4] = "}";
		glShaderSource(shader, 5, (const GLchar *const *)source, NULL);
	}
	recompile_shader_prog_2(d, shader);
}
