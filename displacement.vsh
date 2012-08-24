uniform sampler2D texture;
uniform sampler2D height_map;

void main(void) {
	vec4 pos = gl_Vertex;
	vec4 color = texture2D(height_map, gl_MultiTexCoord0.st);
	gl_FrontColor = texture2D(texture, gl_MultiTexCoord0.st);
	pos.z += color.r*10.0;
	gl_Position = gl_ModelViewProjectionMatrix * pos;
}
