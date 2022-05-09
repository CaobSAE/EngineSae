//Pedir GLSL 3.3
#version 330

//Vertex attributes para cada v�rtice
//Corresponden a los datos que tenemis guardados en el vertex buffer(vertex attributes)
in vec3 inPosition;

void main(){
	//gl_Position define la posici�n final de mi v�rtice
	//El output es una coordenada de 4D. Por ahora asumieremos que w es 1
	gl_Position = vec4(inPosition, 1.0);
}