#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H
#include <glm/glm.hpp>
#include <vector>
#include <object.h>
#include <glm/gtc/matrix_transform.hpp>
void gravity(std::vector<dot>& points){
    for (auto& point : points){
        point.Force = glm::vec3(0.0f);
        for (auto& pointn : points){
            if (&point == &pointn){
                continue;
            }
            float r = glm::length(pointn.Position - point.Position);
            if (r < 1.0f) continue;
            glm::dvec3 direction = glm::normalize(pointn.Position - point.Position);
            glm::dvec3 Force = (G*point.Mass*pointn.Mass/(r*r))*direction;   
            point.Force+=Force;
        }
    }
}

void edgeBounce(std::vector<dot>& points){
    for (auto& point: points){
        if (point.Position.x > 1.0f){
            point.Position.x = 1.0f;
            point.Velocity.x*=-1.0f;
            
        }
        if (point.Position.x < -1.0f){
            point.Position.x = -1.0f;
            point.Velocity.x*=-1.0f;
            
        }
        if (point.Position.y > 1.0f){
            point.Position.y = 1.0f;
            point.Velocity.y*=-1.0f;
            
        }
        if (point.Position.y < -1.0f){
            point.Position.y = -1.0f;
            point.Velocity.y*=-1.0f;
            
        }
    }
}

#endif