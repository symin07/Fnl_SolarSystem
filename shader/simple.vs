#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;   // This value is sended to fragmentShader

void main() { 
    // gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position = vec4(aPos, 1.0);                      // vec3(aPos) is used to make vec4 constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);              // dark red 
}