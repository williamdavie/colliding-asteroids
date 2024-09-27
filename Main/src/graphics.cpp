//
//  File for openGl functions
//
//  Created by William Davie on 03/09/2024.
//


#define GL_SILENCE_DEPRECATION

#include "graphics.hpp"

#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <cmath>
#include <thread>
#include <iostream>

using namespace Eigen;

// re-size the frame

void framebuffer_size_callback(GLFWwindow* window, int width, int height, float x, float y) {
    
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    
    glOrtho(-x, x, -y, y, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    
}

// very basic but can be ignored used for video visuals.

void draw_line(Vector2d point1, Vector2d point2) {
    
    glBegin(GL_LINES);
    
    glVertex2f(point1[0], point1[1]);
    glVertex2f(point2[0], point2[1]);
    
    glEnd();
}


