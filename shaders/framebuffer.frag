#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2DMS screenTexture;
uniform sampler2DMS normalsTexture;
uniform int transition[2];

void main(void)
{
	// Multisample anti-aliasing
	ivec2 coords = ivec2(gl_FragCoord.xy);
	vec4 sample1 = texelFetch(screenTexture, coords, 0);
	vec4 sample2 = texelFetch(screenTexture, coords, 1);
	vec4 sample3 = texelFetch(screenTexture, coords, 2);
	vec4 sample4 = texelFetch(screenTexture, coords, 3);
	vec4 col = (sample1 + sample2 + sample3 + sample4) / 4.0f;

	if (transition[1] != 3) {
		FragColor = col;
		return ;
	}

	// Cel shading
	col = vec4(ivec4(col * 5)) / 5.0f;

	// Outline
	vec4 center = texelFetch( normalsTexture, coords, 0 );

	// sampling just these 3 neighboring fragments keeps the outline thin.
	vec4 top = texelFetch( normalsTexture, coords + ivec2(0, 1), 0 );
	vec4 topRight = texelFetch( normalsTexture, coords + ivec2(1, 1), 0 );
	vec4 right = texelFetch( normalsTexture, coords + ivec2(1, 0), 0 );

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
