uniform sampler2D height_field;

void main(void) {
	vec4 pos = gl_Vertex;
	vec4 color = texture2D(height_field, gl_MultiTexCoord0.st);
	pos.z += color.r*10.0;
	gl_Position = gl_ModelViewProjectionMatrix * pos;
	gl_FrontColor = gl_Color;
}
