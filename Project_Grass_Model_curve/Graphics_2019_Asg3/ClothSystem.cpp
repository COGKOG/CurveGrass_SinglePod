#define GL_SILENCE_DEPRECATION
#include "ClothSystem.h"
#include <iostream>
extern bool triangles;
extern bool wind;

//TODO: Initialize here
ClothSystem::ClothSystem(int num)
{
    vector<Vector3f> state;
    m_numParticles = num;
    
    for (int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++){
            //state.push_back(Vector3f(j, -i, 0));
            state.push_back(Vector3f(j*0.25, 2, i*0.25));
            state.push_back(Vector3f(0, 0, 0));
        }
    }
    
    this->setState(state);
}

// Helper functions
// Store a big Vector3f arry of size 2n where positions are stored at even indices and velocities at odd indices.
// It might help to write helper functions that read the position or velocity of particle i.
vector<Vector3f> ClothSystem::readPosition(vector<Vector3f>(state)){
    vector<Vector3f> position;
    for (int i = 0; i < state.size(); i+=2){
        position.push_back(state[i]);
    }
    return position;
}

vector<Vector3f> ClothSystem::readVelocity(vector<Vector3f>(state)){
    vector<Vector3f> velocity;
    for (int i = 0; i < state.size(); i+=2){
        velocity.push_back(state[i+1]);
    }
    return velocity;
}

// Helper function to compute spring force
Vector3f ClothSystem::springForce(Vector3f i,Vector3f j, float k, float r){
    Vector3f distance = Vector3f(i.x()-j.x(), i.y()-j.y(), i.z()-j.z());
    return -1.0 * k * (distance.abs() - r) * (distance.normalized());
}

int ClothSystem::indexOf(int i, int j){
    return (i * m_numParticles) + j;
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
    vector<Vector3f> f;
    // call helper function to get position and velocity
    vector<Vector3f> position = readPosition(state);
    vector<Vector3f> velocity = readVelocity(state);
    
    // YOUR CODE HERE
    // constants: mass(m), gravity(g), drag(k), rest length (r), spring constant(k)
    float mass = 0.3;
    Vector3f gravity = Vector3f(0, -9.81, 0);
    float drag = 0.7;
    float r_struc = 0.5;
    float r_shear = r_struc * sqrt(2.0);
    float r_flex = 2 * r_struc;
    float k_struc = 100.0;
    float k_shear = 40.0;
    float k_flex = 40.0;
    
    
    for (int i = 0; i < m_numParticles; i++) {
        for(int j = 0; j < m_numParticles; j++){
            int index = indexOf(i, j);
            if ((i==0 && j==0) || (i==0 && j == m_numParticles-1)){
                // top right and left corners velocity and acceleration is zero
                f.push_back(Vector3f(0,0,0));
                f.push_back(Vector3f(0,0,0));
            }
            else{
                f.push_back(velocity[index]);
                Vector3f force = (gravity * mass) + (-1 * drag * velocity[index]);
                
                // calculate structural force
                if (i > 0){
                    force += springForce(position[index], position[index-m_numParticles], k_struc, r_struc);
                }
                if (i < m_numParticles-1){
                    force += springForce(position[index], position[index+m_numParticles], k_struc, r_struc);
                }
                if (j < m_numParticles-1){
                    force += springForce(position[index], position[index+1], k_struc, r_struc);
                }
                if (j > 0){
                    force += springForce(position[index], position[index-1], k_struc, r_struc);
                }
                
                // calculate shear force
                if (i > 0  && j > 0){
                    force += springForce(position[index], position[index-(m_numParticles+1)], k_shear, r_shear);
                    //force += springForce(position[index], position[index-4], k_shear, r_shear);
                }
                if (i < m_numParticles-1 && j < m_numParticles-1){
                    force += springForce(position[index], position[index+(m_numParticles+1)], k_shear, r_shear);
                    //force += springForce(position[index], position[index+4], k_shear, r_shear);
                }
                if (i < m_numParticles-1 && j > 0){
                    force += springForce(position[index], position[index+(m_numParticles-1)], k_shear, r_shear);
                    //force += springForce(position[index], position[index+2], k_shear, r_shear);
                }
                if (i > 0 && j < m_numParticles-1){
                    force += springForce(position[index], position[index-(m_numParticles-1)], k_shear, r_shear);
                    //force += springForce(position[index], position[index-2], k_shear, r_shear);
                }
                
                // calculate flex force
                if (j > 1){
                    force += springForce(position[index], position[index-2], k_flex, r_flex);
                }
                if (j < m_numParticles-2){
                    force += springForce(position[index], position[index+2], k_flex, r_flex);
                }
                if (i < m_numParticles-2){
                    force += springForce(position[index], position[index+m_numParticles*2], k_flex, r_flex);
                }
                if (i > 1){
                    force += springForce(position[index], position[index-m_numParticles*2], k_flex, r_flex);
                }
                
                if(wind){
                    float fraction = float(rand() / static_cast <float> (RAND_MAX));
                    force+=Vector3f(0.0f, 0.0f, 5.0f*(fraction));
                }
                
                f.push_back (force/mass);
            }
        }
    }
    
    return f;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
    /*
    vector<Vector3f> position = this->readPosition(this->getState());
    for (int i = 0; i < m_numParticles; i++) {
        for (int j = 0; j < m_numParticles; j++){
            int index = indexOf(i, j);
            cout << "Particle " << index << endl;
            Vector3f pos = position[index];//  position of particle i. YOUR CODE HERE
            cout << "Pos: " << pos[0] << ", " << pos[1] << endl;
            glPushMatrix();
            glTranslatef(pos[0], pos[1], pos[2] );
            glutSolidSphere(0.075f,10.0f,10.0f);
            glPopMatrix();
        }
    }
    */
    
    vector<Vector3f> position = this->readPosition(this->getState());
    for (int i = 0; i < m_numParticles; i++) {
        for (int j = 0; j < m_numParticles; j++){
            int index = indexOf(i, j);
            if (triangles){
                if(i<m_numParticles-1 && j<m_numParticles-1){
                    Vector3f p1 = position[index];
                    Vector3f p2 = position[index+1];
                    Vector3f p3 = position[index+m_numParticles];
                    Vector3f p4 = position[index+(m_numParticles+1)];
            
                    Vector3f n1 = Vector3f::cross(p3-p1,p2-p1).normalized();
                    Vector3f n2 = Vector3f::cross(p2-p3,p1-p3).normalized();
                    Vector3f n3 = Vector3f::cross(p1-p2,p3-p2).normalized();
                    Vector3f n4 = Vector3f::cross(p3-p2,p4-p2).normalized();
                    Vector3f n5 = Vector3f::cross(p4-p3,p2-p3).normalized();
                    Vector3f n6 = Vector3f::cross(p2-p4,p3-p4).normalized();
            
                    glBegin(GL_TRIANGLES);
                    glNormal3f(n1.x(), n1.y(), n1.z());
                    glVertex3f(p1.x(), p1.y(), p1.z());
                    glNormal3f(n2.x(), n2.y(), n2.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glNormal3f(n3.x(), n3.y(), n3.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glEnd();
                    glBegin(GL_TRIANGLES);
                    glNormal3f(n4.x(), n4.y(), n4.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glNormal3f(n5.x(), n5.y(), n5.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glNormal3f(n6.x(), n6.y(), n6.z());
                    glVertex3f(p4.x(), p4.y(), p4.z());
                    glEnd();
                }
            }
            else{
                Vector3f pos = position[index];//  position of particle i. YOUR CODE HERE
                glPushMatrix();
                glTranslatef(pos[0], pos[1], pos[2] );
                glutSolidSphere(0.075f,10.0f,10.0f);
                glPopMatrix();
                
                // Wire Frame
                if (i<m_numParticles-1){
                    Vector3f p1 = position[index];
                    Vector3f p2 = position[index+m_numParticles];
                    glLineWidth(3.5);
                    glBegin(GL_LINES);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glEnd();
                }
                if (j<m_numParticles-1){
                    Vector3f p1 = position[index];
                    Vector3f p2 = position[index+1];
                    glLineWidth(3.5);
                    glBegin(GL_LINES);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glEnd();
                }
            }
            
        }
    }
}

