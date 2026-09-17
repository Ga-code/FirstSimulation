#version 330 core
layout (location = 0) in vec4 aPos;
out float alpha;
uniform mat4 projection;
uniform mat4 view;
void main(){
    gl_Position = projection*view*vec4(aPos.xyz, 1.0f);
    alpha = aPos.w;
}
