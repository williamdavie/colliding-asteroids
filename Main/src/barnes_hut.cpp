//
//  source code for barnes hut algo
//
//  Created by William Davie on 20/07/2024.
//

#include "barnes_hut.hpp"
#include <Eigen/Dense>
#include "particle.hpp"
#include <iostream>

// simulation parameters

double grav_const = 0.01;
double step = 0.005;
double theta = 0.9;
double stiffness = 200;
//double damping = 5.0;
double friction_const = 0.0;


// all forces, gravity, spring, damping and friction are calculated within a single function to reduce calculations


Vector2d compute_force(Particle& p1, Particle& p2, float damping){
    
    
    Vector2d gravity = Vector2d(0,0);
    Vector2d spring_force = Vector2d(0,0);
    Vector2d damping_force = Vector2d(0,0);
    Vector2d friction_force = Vector2d(0,0);
    
    
    // only calculate forces if p1 != p2.
    
    if (p1.position != p2.position){
        
        Vector2d distance = p1.position - p2.position;
        
        double norm = distance.norm();
        
        Vector2d unit_distance = distance / norm;
        
        // newtons law of gravitation
        
        gravity = - (grav_const * p1.mass * p2.mass) / (powf(norm,2)) * unit_distance;
        

        if (!(norm >= p1.radius + p2.radius)){
            
            // rest length of spring = sum of radii
            
            float spring_distance = norm - (p1.radius + p2.radius);
            
            // spring force - hookes law
            
            spring_force = - (stiffness * spring_distance)*unit_distance;
            
            // damping
            
            damping_force = damping * (p2.velocity - p1.velocity);
            
            // friction - acts perpendicular to velocity with magnitude of normal force (spring + damping)
            
            Vector2d v_km = p1.velocity - p2.velocity;
            
            if (v_km.norm() != 0){
                
                Vector2d v_t = v_km - v_km.dot(unit_distance)*unit_distance;
                
                Vector2d t_hat = v_t / v_t.norm();
                
                friction_force = -friction_const * (spring_force + damping_force).norm() * t_hat;
                
                }

            }
            
        }
    
    return gravity + spring_force + damping_force + friction_force;
    
};


// insert a particle into the current node, create subnodes if necessary

// Note: 'existingParticle' refers to the previously considered particle.

void TreeNode::insert_to_node(std::shared_ptr<Particle> newParticle){
    
    if (numParticles > 1){
        // if number of particles in this node are > 1, need to create a subnode for this new particle, incoming particle.

        int quad = fetch_quadrant(newParticle); // finds correct quad
        
        if (!subnodes[quad]) {
            
            create_subnodes(quad); // creates a new Treenode object with smaller dimensions.
        }
        
        subnodes[quad]->insert_to_node(newParticle); // recursive process, insert_to_node is called to subnode pointed to.
        
        
    } else if (numParticles == 1){
        
        // if num particles == 1 and another particle is called, we need to subdivide
        
        // first fetch and create the quadrant for the existing particle
        
        int quad = fetch_quadrant(existingParticle);

        if (!subnodes[quad]) {
            
            create_subnodes(quad);
            
        }
        
        subnodes[quad]->insert_to_node(existingParticle);
        
        // now fetch and create the quadrant for the newParticle.
        
        quad = fetch_quadrant(newParticle);

        if (!subnodes[quad]) {
            
            create_subnodes(quad);
            
        }
        
        subnodes[quad]->insert_to_node(newParticle);
        
    } else if (numParticles == 0){
        existingParticle = newParticle;
    }
    
    // since we are 'inserting a particle' once completed the counter for current node increases
    
    numParticles++;
    
    
};



int TreeNode::fetch_quadrant(std::shared_ptr<Particle> particle){
    
    int quad = 0;
    
    Vector2d midpoint = (max_position + min_position) / 2.0;
    
    
    if (particle->position[0] < midpoint[0]){
        
        if (particle->position[1] < midpoint[1]){
            
            quad = 2;
            // SW
     
        } else {
            
            quad = 0;
            // NW
        }
        
    } else if (particle->position[0]  > midpoint[0]){
        
        if (particle->position[1] > midpoint[1]){
            
            quad = 1;
            // NE
            
        } else {
            
            quad = 3;
            // SE
        }
        
    };
    
    return quad;
};

// simply defines redefines our subnode to an active TreeNode object (rather than nullptr) pointers if needed.

void TreeNode::create_subnodes(int quad){
    
    Vector2d midpoint = (max_position + min_position) / 2.0;
    
    switch (quad) {
            // different subnode for each quadrant, use switch statement.
            
        case 0: // NW
            // make_shared allocates a single block of memory to new Treenode object.
            
            subnodes[quad] = std::make_shared<TreeNode>(Vector2d(min_position[0],midpoint[1]),
                                                   Vector2d(midpoint[0],max_position[1]));
            break;
        case 1: // NE
            subnodes[quad] = std::make_shared<TreeNode>(midpoint,
                                                   max_position);
            break;
        case 2: // SW
            subnodes[quad] = std::make_shared<TreeNode>(min_position,
                                                   midpoint);
            break;
        case 3: // SE
            subnodes[quad] = std::make_shared<TreeNode>(Vector2d(midpoint[0],min_position[1]),
                                                   Vector2d(max_position[0],midpoint[1]));

            break;
        
        default:
            
            throw std::runtime_error("Invalid quad");
    }
    
};


// compute the mass distribution of the current node (required for forces between nodes/quadrants)

void TreeNode::compute_mass_distribution(){
    
    // if only 1 particle, mass is just centred on that particle.
    
    if (numParticles == 1){
        
        centre_of_mass = existingParticle->position;
        
        mass = existingParticle->mass;
        
    } else { // else more than 1 particle thus subnodes exist
        
        mass = 0;
        centre_of_mass = Vector2d(0,0);
        
        // loop throgh all subnodes, recursively applying 'compute mass'
        
        for (int i = 0; i < 4; ++i){
            
 
            if (subnodes[i]) {
                
                // if not exists
                
                subnodes[i]->compute_mass_distribution();
                
                // now add mass of each subnode to current node.
                
                mass += subnodes[i]->mass;
                
                centre_of_mass += subnodes[i]->mass * subnodes[i]->centre_of_mass;
            }
        }
            
        centre_of_mass /= mass;
    }

};

// apply forces on target particles (NlogN)

Vector2d TreeNode::calculate_force(Particle& targetParticle, float damping) {
    
    Vector2d force = Vector2d(0, 0);
    
    // again if single particle, easy calculation.

    if (numParticles == 1) {
        
        force = compute_force(targetParticle, *existingParticle, damping);
        
    } else {
        
        // key condition for barnes hut, Multipole-Acceptance-Criterion (MAC)
        
        Vector2d r = targetParticle.position - centre_of_mass;
        
        double d = max_position[0] - min_position[0];
        
        // generate a temporary particle corrosponding to our node.

        if ((d / r.norm()) < theta) {
            
            Particle temporary_particle;
            
            temporary_particle.position = centre_of_mass;
            
            temporary_particle.mass = mass;
            
            // compute force
            
            force = compute_force(targetParticle, temporary_particle, damping);
            
        } else {
            
            for (int i = 0; i < 4; ++i) {
                
                if (subnodes[i]) {
                    
                    force += subnodes[i]->calculate_force(targetParticle, damping);
                }
            }
        }
    }
    return force;
}
