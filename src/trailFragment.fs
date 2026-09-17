#version 330 core
out vec4 FragColor;
in float alpha;
void main(){
    FragColor = vec4(0.27, 0.6, 0.79, alpha);
}