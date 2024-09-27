//
//  Functions for acting on Particle object
//
//  Created by William Davie on 16/07/2024.
//
#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include "particle.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

using namespace Eigen;


void Particle::update_position(float step){
    
    // Euler method #1
    
    position += velocity * step;
    
};

void Particle::apply_force(Vector2d force, float step){
    
    // Euler method #2
    
    Vector2d acceleration = force/mass;

    velocity += acceleration * step;


};

void Particle::draw(float Red, float Green, float Blue, float graphical_radius)
{
    float angle;
    
    int num_verticies = 200;
    
    // enable blending, required if opacity < 1.0
    
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // colour function:
    
    float colour_multiplier = 0.25/powf(velocity.norm(),1.1);
    
    if (colour_multiplier < 1){
        colour_multiplier = 1;
    };
    
    // begin drawing
    
    glBegin(GL_TRIANGLE_FAN);
    
    glColor4f(Red*colour_multiplier, Green*colour_multiplier, Blue*colour_multiplier,1.0);
    
    //glColor4f(1.0, 1.0, 1.0,1.0);
    
    glVertex2f(position[0], position[1]);
    
    // draw circle
    
    for (int i = 0; i <= num_verticies; ++i) {
        
        // work with float for openGL

        angle = 2.0f * M_PI * static_cast<float>(i) / num_verticies;
        
        float x_new = graphical_radius * std::cos(angle);
        
        float y_new = graphical_radius * std::sin(angle);
        
        glVertex2f(position[0] + x_new, position[1] + y_new);
        
    }
    
    glEnd();
}

//
