//
//  main.cpp
//
//  Created by William Davie on 15/07/2024.
//


#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <iostream>
#include <chrono>
#include <thread>

#include "particle.hpp"
#include "graphics.hpp"
#include "barnes_hut.hpp"

using namespace Eigen;

typedef Array<Particle, Dynamic, 1> particleArray;


// particle parameters - constant radius

float particle_radius = 0.03;
float particle_density = 36.5;

float particle_mass = particle_density * M_PI * powf(particle_radius,2);


int main(void)
{
    
    // initialize Particles
    
    int N = 45;
    
    float astroid_1 = -1;
    
    particleArray particles;
    
    particles.resize(N);
    
    // define parameters of asteriod 1 and asteriod 2 separately

    for (int i = 0; i < N; ++i){
        
        float random_x = (float)(rand()) / RAND_MAX * 2.0f - 1.0f;
        float random_y = (float)(rand()) / RAND_MAX * 2.0f - 1.0f;
        
        Vector2d velocity_vector = Vector2d(0.0,0);
        
        if (i <= astroid_1){
            
            random_x = (float)(rand()) / RAND_MAX * 1.0f + 5.0f;
            random_y = (float)(rand()) / RAND_MAX * 1.0f - 0.5f;
            
            velocity_vector = Vector2d(-1.0,-0.0);
        }

        
        // set particle parameters
        
        particles[i].position = Vector2d(random_x,random_y);
        particles[i].velocity = velocity_vector;
        particles[i].mass = particle_mass;
        particles[i].radius = particle_radius;
    
    }
    
    
    GLFWwindow* window;
    
    float step = 0.005;
    
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    
    window = glfwCreateWindow(1400, 700, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    // ortho projection of current context. 
    
    framebuffer_size_callback(window, 2800, 1400, 3.0, 1.51);
    
    // begin loop
    
    // generate empty quadtree
    
    BarnesHut bh;
    
    float j = 0;
    
    // loop until the user closes the window
    

    
    //glClearColor(0.18, 0.333, 0.40, 1.0);
    
    while (!glfwWindowShouldClose(window))
        
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);
        
        // build quadtree for current context
        
        bh.BuildTree(particles);
        
        #pragma omp parallel for
        for (int i = 0; i < N; ++i){ // N steps
            
            if (i <= astroid_1){
                
                particles[i].draw(0.31, 0.59, 0.95, particle_radius);
                
            } else{
    
                particles[i].draw(0.75, 0.22, 0.17, particle_radius);
                
            };
            
            Vector2d force_tot = Vector2d(0,0);
            
            // calculate total force on target particle (logN steps)
            
            float damping = 1.0;
            
            force_tot += bh.rootNode->calculate_force(particles[i], damping);
        
            particles[i].apply_force(force_tot, step);
            
            particles[i].update_position(step);
            
            
        };
        
        j++;
        
        // prevents the simulation from instantly starting.
        
        if (j==10){
            
            using std::operator""s;
              for (int i = 10; i > 0; --i)
              {
                std::cout << i << ' ' << std::flush;
                std::this_thread::sleep_for(1s);
              }
            
            
        }

        // Swap front and back buffers
        
        glfwSwapBuffers(window);

        // Poll for and process events
        
        glfwPollEvents();
        
        
    }

    glfwTerminate();
    return 0;
}



