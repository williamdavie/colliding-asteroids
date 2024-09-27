//
//
//  Header for the barnes hut alogirthm
//
//  Created by William Davie on 20/07/2024.
//

#ifndef barnes_hut_hpp
#define barnes_hut_hpp

#include <stdio.h>
#include <Eigen/Dense>
#include "particle.hpp"
#include <array>

typedef Eigen::Array<Particle, Dynamic, 1> particleArray;

// defining a node

class TreeNode{
    
public:
    
    // Constructor
    
    TreeNode(Vector2d min_position, Vector2d max_position)
        : min_position(min_position), max_position(max_position), numParticles(0) {
            
        // sub nodes initialised as pointers to nothing
            
        subnodes[0] = subnodes[1] = subnodes[2] = subnodes[3] = nullptr;
    }
    
    // function to insert particle into current node or corrosponding subnode.
    
    void insert_to_node(std::shared_ptr<Particle> newParticle);
    
    // compute mass distribution of current node
    
    void compute_mass_distribution();
    
    // calculate force on given particle.
    
    Vector2d calculate_force(Particle& targetParticle, float damping);
    
    Vector2d min_position;
    
    Vector2d max_position;
    
private:
    
    // simple function to fetch quadrant of inserted particle.
    
    int fetch_quadrant(std::shared_ptr<Particle> newParticle);
    
    // function to create subnodes if required (uses results from fetch quad)
    
    void create_subnodes(int quad);
    
    int numParticles;
    
    // subnodes of this class are pointers to new TreeNode objects
    
    // we only allocate memory to these subnodes if required.
    
    std::array<std::shared_ptr<TreeNode>, 4> subnodes;
    
    // nodes particle
    
    std::shared_ptr<Particle> existingParticle;
    
    Vector2d centre_of_mass;
    
    double mass;
    
};

// main Barnes Hut class, this defines the entire quad tree object

class BarnesHut{
    
public:
    
    void BuildTree(particleArray& particles) {

        // reset tree to single root node
        ResetTree();
        
        // insert each particle to root node and subnodes thereafter
        
        for (auto& particle : particles) {
            std::shared_ptr<Particle> particlePtr = std::make_shared<Particle>(particle);
            rootNode->insert_to_node(particlePtr);

        }
        
        
        rootNode->compute_mass_distribution();
        
    }
    
    std::shared_ptr<TreeNode> rootNode;
    
private:
    
    void ResetTree(){
        
        rootNode = std::make_shared<TreeNode>(Vector2d(-100,-100),Vector2d(100,100));
        
    };
    
};


// additional functions for simulation customization.

void apply_force_to_particle(BarnesHut bh, Particle p1, float step);



#endif /* BH_algorithm_hpp */

