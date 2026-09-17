#ifndef OBJECT_H
#define OBJECT_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/vector_double1_precision.hpp>
#include <path.h>
#include <map>
#include <cmath>
#include <vector>
constexpr double G = 6.67430e-11;
struct dot {
    glm::dvec3 Position;
    glm::dvec3 Velocity;
    glm::dvec3 Force;
    double Mass;
    unsigned int VBO, VAO;
    std::vector<glm::vec3> paths = {};
    trail path;
    shader trailProgram = shader("trailVertex.vs", "trailFragment.fs");
    dot(glm::dvec3 Position, glm::dvec3 Velocity, glm::dvec3 Force, float Mass) 
    {
        this->Position = Position;
        this->Velocity = Velocity;
        this->Force = Force;
        this->Mass = Mass;
        float vertices[3] = {0.0f};
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
    
    void step(double dTime, std::vector<dot>& points){
        glm::dvec3 k1v = gravity2(Position, points)/Mass;
        glm::dvec3 k1x = Velocity;
        glm::dvec3 k2v = gravity2(Position + k1x*dTime/2.0, points)/Mass;
        glm::dvec3 k2x = Velocity + k1v*dTime/2.0;
        glm::dvec3 k3v = gravity2(Position + k2x*dTime/2.0, points)/Mass;
        glm::dvec3 k3x = Velocity + k2v*dTime/2.0;
        glm::dvec3 k4v = gravity2(Position + k3x*dTime, points)/Mass;
        glm::dvec3 k4x = Velocity + k3v*dTime;
        Position+=(k1x + 2.0*k2x + 2.0*k3x + k4x)*dTime/6.0;
        Velocity+=(k1v + 2.0*k2v + 2.0*k3v + k4v)*dTime/6.0;
    }
    void draw(){
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawArrays(GL_POINTS, 0, 1);
    }
    
    glm::dvec3 gravity2(glm::dvec3 position, std::vector<dot>& points){
        glm::dvec3 force = glm::dvec3(0.0f);
        for (auto& pointn : points){
            if (&pointn == this) continue;
            double r = glm::length(pointn.Position - position);
            glm::dvec3 direction = glm::normalize(pointn.Position - position);
            force+=(G*Mass*pointn.Mass/(r*r))*direction;   
        }
        return force;
    }
    void drawTrail(glm::mat4 projection, glm::mat4 view){
        trailProgram.use();
        paths.push_back(Position);
        if(paths.size() > 5000){
            paths.erase(paths.begin());

        }
        trailProgram.setMatrix4("projection", projection);
        trailProgram.setMatrix4("view", view);
        path.draw(paths);
    }
};

#endif 