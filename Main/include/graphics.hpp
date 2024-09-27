//
//  graphics.hpp
//
//  Created by William Davie on 03/09/2024.
//

#ifndef graphics_hpp
#define graphics_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

using namespace Eigen;

void framebuffer_size_callback(GLFWwindow* window, int width, int height, float x, float y);

void draw_line(Vector2d point1, Vector2d point2);

#endif /* graphics_hpp */
