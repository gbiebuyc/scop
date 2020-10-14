#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D normalsTexture;
uniform int transition[2];

const float width = 800;
const float height = 600;

void main(void)
{
	vec4 col = texture(screenTexture, TexCoords);

	if (transition[1] != 3) {
		FragColor = col;
		return ;
	}

	// Cel shading
	col = vec4(ivec4(col * 5)) / 5.0f;

	// Outline
	// https://gamedev.stackexchange.com/a/86413
	float dx = 1.0 / width;
	float dy = 1.0 / height;

	vec4 center = texture( normalsTexture, TexCoords );

	// sampling just these 3 neighboring fragments keeps the outline thin.
	vec4 top = texture( normalsTexture, TexCoords + vec2(0.0, dy) );
	vec4 topRight = texture( normalsTexture, TexCoords + vec2(dx, dy) );
	vec4 right = texture( normalsTexture, TexCoords + vec2(dx, 0.0) );

	// the rest is pretty arbitrary, but seemed to give me the
	// best-looking results for whatever reason.

	vec4 t = center - top;
	vec4 r = center - right;
	vec4 tr = center - topRight;

	t = abs( t );
	r = abs( r );
	tr = abs( tr );

	float n;
	n = max( n, t.x );
	n = max( n, t.y );
	n = max( n, t.z );
	n = max( n, r.x );
	n = max( n, r.y );
	n = max( n, r.z );
	n = max( n, tr.x );
	n = max( n, tr.y );
	n = max( n, tr.z );

	// threshold and scale.
	n = 1.0 - clamp( clamp((n * 2.0) - 0.8, 0.0, 1.0) * 1.5, 0.0, 1.0 );

	FragColor = col * (0.1 + 0.9*n);
}
