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
	d->time_loc = glGetUniformLocation(d->shader_prog, "time");
	d->cam_pos_loc = glGetUniformLocation(d->shader_prog, "cameraPos");
	glUseProgram(d->shader_prog);
	glUniform1i(glGetUniformLocation(d->shader_prog, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(d->shader_prog, "skybox"), 1);
}

void	init_effects(t_data *d)
{
	d->effects[EFFECT_SHADES_OF_GREY] = "effect_shades_of_grey();";
	d->effects[EFFECT_TEXTURE] = "effect_lighting(effect_texture().rgb);";
	d->effects[EFFECT_RAINBOW] = "effect_lighting(effect_rainbow().rgb);";
	d->effects[EFFECT_WIREFRAME] = "effect_wireframe();";
	d->effects[EFFECT_CEL_SHADING] = "effect_cel_shading();";
	d->effects[EFFECT_REFLECTION] = "effect_reflection();";
	d->effects[EFFECT_REFRACTION] = "effect_refraction();";
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
		source[3] = d->effects[d->transition[0]];
		source[4] = "vec4 effect_1 = ";
		source[5] = d->effects[d->transition[1]];
		source[6] = "FragColor = mix(effect_1, effect_0, mix_value);}";
		glShaderSource(shader, 7, (const GLchar *const *)source, NULL);
	}
	else
	{
		source[2] = "FragColor = ";
		source[3] = d->effects[d->transition[1]];
		source[4] = "}";
		glShaderSource(shader, 5, (const GLchar *const *)source, NULL);
	}
	recompile_shader_prog_2(d, shader);
}
