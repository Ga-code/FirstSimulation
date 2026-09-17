#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <shader.h>
#include <camera.h>
#include <object.h>
#include <path.h>
#include <textRender.h>
#include <PhysicsWorld.h>
#include <iostream>
#include <reference.h>
#include <map>
#include <cmath>
#include <vector>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}



double lastTime = 0.0f;
double dTime = 0.0f;
Camera camera(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -180.0f, 0.0f);
float lastX = 1000.0f;
float lastY = 1000.0f;
bool firstmouse = true;
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, dTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, dTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, dTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, dTime);
    }

}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstmouse){
        lastX = xpos;
        lastY = ypos;
        firstmouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
int main()
{
    
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to initalize window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initailze GLAD\n";
        return -1;
    }
   
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    shader program("vertex.vs", "fragment.fs");
    glViewport(0,0, 1920, 1080);
    program.use();
    std::vector<dot> points;
    glm::mat4 model = glm::mat4(1.0f);
    /*double earth = 5.97e24;
    double moon = 7.35e22;
    double sun = 1.9891e30;
    glm::dvec3 positione = glm::dvec3(-9.158945011264966e10,-1.213249218816741e11, 2.442833909864724e7);
    glm::dvec3 velocitye = glm::dvec3(2.327945667348206e4, -1.810535077464547e4, -1.293854325554733e-1);
    glm::dvec3 positionm = glm::dvec3(-9.123121462130207e10, -1.212383037718505e11, 4.448416548321396e7);
    glm::dvec3 velocitym = glm::dvec3(2.296410158263309e4, -1.708994617665874e4, 6.946946210704397e1);
    glm::dvec3 positions = glm::dvec3(-3.196345940451375e8, -8.113489176240250e8, 1.692638198580331e7);
    glm::dvec3 velocitys = glm::dvec3(1.167988717311920e1, 2.576034237262590e0, -2.413529037243172e-1);
    points.emplace_back(positions, velocitys, glm::vec3(0.0f), sun);
    points.emplace_back(positionm, velocitym, glm::vec3(0.0f), moon);
    points.emplace_back(positione, velocitye, glm::vec3(0.0f), earth);
    */
    points.emplace_back(glm::dvec3(0.0), glm::dvec3(1.0e2, 1.0e2, 0.0), glm::dvec3(0.0), 1.9891e25);
    points.emplace_back(glm::dvec3(0.1e12), glm::dvec3(0.0), glm::dvec3(0.0), 1.9891e26);
    points.emplace_back(glm::dvec3(-0.1e12), glm::dvec3(0.0), glm::dvec3(0.0), 1.9891e26);
    //points.emplace_back(glm::dvec3(0.0, 0.0f, 0.0f), 5.0*glm::dvec3(velocitye.x, 0.0f, 0.0f), glm::vec3(0.0f), sun);
    glPointSize(5.0f);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    lastTime = glfwGetTime();
    reference xAxis(glm::mat4(1.0f));
    reference zAxis(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    reference yAxis(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    textRender text;
    shader textprogram("textVertex.vs", "textFragment.fs");
    textprogram.use();
    
    glm::mat4 textprojection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(textprogram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textprojection));
    program.use();
    //double totalEnergy = 0.5*moon*glm::dot(points[0].Velocity, points[0].Velocity) + 0.5*earth*glm::dot(points[1].Velocity, points[1].Velocity) - G*moon*earth/(METERS_PER_UNIT*glm::length(points[0].Position - points[1].Position));
    //double e0 = totalEnergy;
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        processInput(window);
        textprogram.use();
        std::string position = "Moon Position: ("+std::to_string(points[0].Position.x)+", "+std::to_string(points[0].Position.y)+", "+std::to_string(points[0].Position.z) + ")";
        text.renderText(textprogram, position, 25.0f, 750.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        std::string eposition = "Earth Position: ("+std::to_string(points[1].Position.x)+", "+std::to_string(points[1].Position.y)+", "+std::to_string(points[1].Position.z) + ")";
        text.renderText(textprogram, eposition, 25.0f, 700.0f, 0.5f, glm::vec3(0.5f, 1.0f, 1.0f));
        double speedMoon = glm::sqrt(glm::pow(points[0].Velocity.x, 2) + glm::pow(points[0].Velocity.y, 2) + glm::pow(points[0].Velocity.z, 2));
        std::string speedMoonString = "Moon speed: "+std::to_string(speedMoon) + "m/s";
        text.renderText(textprogram, speedMoonString, 25.0f, 400.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        double speedEarth = glm::sqrt(glm::pow(points[1].Velocity.x, 2) + glm::pow(points[1].Velocity.y, 2) + glm::pow(points[1].Velocity.z, 2));
        std::string speedEarthString = "Earth speed: "+std::to_string(speedEarth);
        text.renderText(textprogram, speedEarthString, 25.0f, 350.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        text.renderText(textprogram, std::to_string(glm::length(points[1].Position - points[2].Position)),1200.0f, 750.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        //totalEnergy = 0.5*moon*glm::dot(points[0].Velocity, points[0].Velocity) + 0.5*earth*glm::dot(points[1].Velocity, points[1].Velocity) - G*moon*earth/(METERS_PER_UNIT*glm::length(points[0].Position - points[1].Position));
        //std::string totalEnergyString = "Total Energy: "+std::to_string(totalEnergy)+"J";
        //double drift = (e0 - totalEnergy)/1e25;
        //std::string driftString = "Drift: "+std::to_string(drift);
        //text.renderText(textprogram, totalEnergyString, 1200.0f, 750.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        //text.renderText(textprogram, driftString, 1200.0f, 700.0f, 0.5f, glm::vec3(1.0f, 0.5f, 1.0f));
        program.use();
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), 1920.0f/1080.0f, 0.1f, 200000.0f);
        program.setMatrix4("projection", projection);
        program.setMatrix4("view", view);
        xAxis.draw(view, projection, false);
        yAxis.draw(view, projection, false);
        zAxis.draw(view, projection, false);
        double currentTime = glfwGetTime();
        dTime = currentTime - lastTime;
        lastTime = currentTime;
        double speed = 3.0e8f;
        int substep = 1000;
        double subdt = dTime*speed/substep;
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
            projection = glm::perspective(glm::radians(5.0f), 1920.0f/1080.0f, 0.1f, 100.0f);
            view = glm::lookAt(glm::vec3(points[2].Position.x, points[2].Position.y, points[2].Position.z +0.5e12f)/(float)METERS_PER_UNIT,glm::vec3(points[2].Position)/(float)METERS_PER_UNIT, glm::vec3(0.0f, 1.0f, 0.0f));
            program.use();
            program.setMatrix4("view", view);
            program.setMatrix4("projection", projection);
        }
        for (auto& point : points){
            program.use();
            model = glm::translate(glm::mat4(1.0f), glm::vec3(point.Position/METERS_PER_UNIT));
            program.setMatrix4("model", model);
            point.draw();
            point.drawTrail(projection, view);
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            // wow I guess I don't need anything here
        } else {
            for (int i =0; i<substep; i++){
                for (auto& point: points){
                    point.step(subdt, points);
                }
            }
        } 
        
        
        //edgeBounce(points);
     
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}