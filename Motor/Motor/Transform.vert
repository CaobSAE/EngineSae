//Pedir GLSL 3.3
#version 330

//Variables uniformes son variables globales que se mantienen  con el mismo valor a través de múltiples invocaciones al programa shader
//Necesitamos matrices 4x4 debido a las coordenadas homogeneas
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

//Vertex attributes para cada vértice
//Corresponden a los datos que tenemis guardados en el vertex buffer(vertex attributes)
in vec3 inPosition;

void main(){
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}