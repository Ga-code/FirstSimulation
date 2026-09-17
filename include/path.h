#ifndef PATH_H
#define PATH_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/vector_double1_precision.hpp>
constexpr double METERS_PER_UNIT = 1.0e12; 
struct trail{
    unsigned int VBO, VAO;
    trail(){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
    void draw(std::vector<glm::vec3> paths){
        std::vector<float> vertices(4 * paths.size());
        for(int i=0; i<paths.size(); i++){
            vertices[4*i] = paths.at(i).x/METERS_PER_UNIT;
            vertices[4*i+1] = paths.at(i).y/METERS_PER_UNIT;
            vertices[4*i+2] = paths.at(i).z/METERS_PER_UNIT;
            vertices[4*i+3] = (float)i/paths.size();
        
        }
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_LINE_STRIP, 0, paths.size());
    }
};
   
    



#endif