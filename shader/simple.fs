#version 330 core

uniform vec4 color;
in vec4 vertexColor;        // the variable inputted by .vs (same name & type)
out vec4 fragColor;         // for last output`      

void main() {
    //fragColor = vertexColor;
    fragColor = color;
}