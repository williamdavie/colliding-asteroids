//
// Particle header file
//
// Created by William Davie on 16/07/2024.
//

#ifndef particle_hpp
#define particle_hpp

// Using Eigen for Vectors
#include <Eigen/Dense>

using namespace Eigen;

struct Particle{
    
    Vector2d position;
    Vector2d velocity;

    float mass;
    float radius;
    
    float current_cluster = -1.0;
    
    // numerical evolution
    
    void update_position(float step);
    
    void apply_force(Vector2d force, float step);
    
    // draw function
    
    void draw(float Red, float Green, float Blue, float graphical_radius);

};

#endif /* Particle_hpp */
