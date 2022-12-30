#version 420

in vec3 vertexPos;

uniform mat4 viewProj;
uniform vec3 cameraPos;

out vec3 texCoord;

void main(){
	texCoord = normalize(vertexPos);
	gl_Position = viewProj * vec4( vertexPos + cameraPos, 1.0f);

}