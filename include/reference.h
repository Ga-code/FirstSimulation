#ifndef REFERENCE_H
#define REFERENCE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/ext/vector_double1_precision.hpp>
struct reference {
    unsigned int VBO;
    unsigned int VAO;
   shader referenceProgram;
    glm::mat4 rotation;
    reference(glm::mat4 rotation) : referenceProgram("referenceVertex.vs", "referenceFragment.fs"){
        this->rotation = rotation;
        std::vector<float> vertices = {
            100.0f, 0.0f, 0.0f,
            -100.0f, 0.0f, 0.0f
        };
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER ,VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        
    }
    void draw(glm::mat4 view, glm::mat4 projection, bool notAxes){
        referenceProgram.use();
        referenceProgram.setMatrix4("view", view);
        referenceProgram.setMatrix4("projection", projection);
        referenceProgram.setMatrix4("rotation", rotation);
        glEnableVertexAttribArray(0);
        glBindVertexArray(VAO);
        glLineWidth(2.5);
        if (notAxes) {
            glLineWidth(0.01);
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_LINES, 0, 2);
    }

};

#endif