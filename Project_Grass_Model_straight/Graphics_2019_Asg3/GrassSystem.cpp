#define GL_SILENCE_DEPRECATION
#include "GrassSystem.h"
#include <iostream>
#include <array>
extern bool triangles;
extern bool wind;
int array_size;

vector<int> j_array;
vector<int> lean_array;
//TODO: Initialize here
GrassSystem::GrassSystem(int num)
{
    vector<Vector3f> state;
    m_numParticles = num;
    array_size = num*num;
    int counter = 1;
    //array<int,5> j_array {9,8,5,10,9};
    int randomVal=0;
    int randomLean=0;
    for(int i=0; i<array_size; i++){
        // random value of [4, 12]
        randomVal = rand() % (12- 4 + 1) + 4;
        j_array.push_back(randomVal);
        
        // random value of [1, 4]
        randomLean = rand() % (4 - 1 + 1) + 1;
        lean_array.push_back(randomLean);
    }
    
    // initialize particle positions
    counter=0;
    for(int k=0; k<m_numParticles;k++){
        for (int j = 0; j < m_numParticles; j++){
            for (int i = 0; i < j_array[counter]; i++) {
                state.push_back(Vector3f(j*0.3, i*0.5, k*0.3));
                state.push_back(Vector3f(0, 0, 0));
            }
            counter++;
        }
    }
    
    this->setState(state);
}

// Helper functions
// Store a big Vector3f arry of size 2n where positions are stored at even indices and velocities at odd indices.
// It might help to write helper functions that read the position or velocity of particle i.
vector<Vector3f> GrassSystem::readPosition(vector<Vector3f>(state)){
    vector<Vector3f> position;
    for (int i = 0; i < state.size(); i+=2){
        position.push_back(state[i]);
    }
    return position;
}

vector<Vector3f> GrassSystem::readVelocity(vector<Vector3f>(state)){
    vector<Vector3f> velocity;
    for (int i = 0; i < state.size(); i+=2){
        velocity.push_back(state[i+1]);
    }
    return velocity;
}

// Helper function to compute spring force
Vector3f GrassSystem::springForce(Vector3f i,Vector3f j, float k, float r){
    Vector3f distance = Vector3f(i.x()-j.x(), i.y()-j.y(), i.z()-j.z());
    return -1.0 * k * (distance.abs() - r) * (distance.normalized());
}

int GrassSystem::indexOf(int i, int j){
    return (i * m_numParticles) + j;
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> GrassSystem::evalF(vector<Vector3f> state)
{
    vector<Vector3f> f;
    // call helper function to get position and velocity
    vector<Vector3f> position = readPosition(state);
    vector<Vector3f> velocity = readVelocity(state);
    
    // YOUR CODE HERE
    // constants: mass(m), gravity(g), drag(k), rest length (r), spring constant(k)
    Vector3f leaning_force_px = Vector3f(0.5, 10.8, 0);
    Vector3f leaning_force_nx = Vector3f(-0.5, 10.8, 0);
    Vector3f leaning_force_pz = Vector3f(0, 10.8, 0.5);
    Vector3f leaning_force_nz = Vector3f(0, 10.8, -0.5);
    
    float mass = 0.5;
    float drag = 0.5;
    float r_struc = 0.5;
    float r_bend = 2.5*r_struc;
    float k_struc = 500.0;
    float k_bend = 50.0;
    Vector3f gravity = Vector3f(0,-9.81,0);
    
    //array<int,5> j_array {9,8,5,10,9};
    //array<int,5> lean_array {1,2,3,4,3};
    int counter = 0;
    
    for(int j = 0; j < array_size; j++){
        Vector3f expo_force= gravity*mass;
        f.push_back(Vector3f(0,0,0));
        f.push_back(Vector3f(0,0,0));
        // random value of [1, 4]
        //int randomVal = rand() % 4 + 1;
        //cout << "Random: " << randomVal << endl;
        counter++;
        for (int i = 1; i < j_array[j]; i++) {
            f.push_back(velocity[counter]);
            Vector3f force;
            //force += (leaning_force_px * mass) + (-1 * drag * velocity[counter]);
            
            // Randomize blade leaning direction
            if(lean_array[j]==1){
                force += (leaning_force_px * mass) + (-1 * drag * velocity[counter]);
            }
            else if(lean_array[j]==2){
                force += (leaning_force_nx * mass) + (-1 * drag * velocity[counter]);
            }
            else if(lean_array[j]==3){
                force += (leaning_force_nz * mass) + (-1 * drag * velocity[counter]);
            }
            else{
                force += (leaning_force_pz * mass) + (-1 * drag * velocity[counter]);
            }
            
            
            // calculate structural spring force
            if (i == j_array[j]-1){
                force += springForce(position[counter], position[counter-1], k_struc, r_struc);
            }
            else{
                force += springForce(position[counter], position[counter+1], k_struc, r_struc);
                force += springForce(position[counter], position[counter-1], k_struc, r_struc);
            }
            
            // calculate bending spring force
            if (i < j_array[j]-2){
                force+= springForce(position[counter], position[counter+2], k_bend, r_bend);
            }
            if (i > 1){
                force+= springForce(position[counter], position[counter-2], k_bend, r_bend);
            }
            
            // curvature force
            if (i > j_array[j]/1.5 && j_array[j] >5){
                force+=expo_force;
                expo_force = expo_force*1.0;
            }
            
            // wind force
            if(wind){
                float fraction = float(rand() / static_cast <float> (RAND_MAX));
                force+=Vector3f(0.0f, 0.0f, 5.0f*(fraction));
            }
            
            f.push_back(force/mass);
            counter++;
        }
    }
    
    return f;
}

///TODO: render the system (ie draw the particles)
void GrassSystem::draw()
{
    //array<int,5> lean_array {1,2,3,4,3};
    vector<Vector3f> position = this->readPosition(this->getState());
    //array<int,5> j_array {9,8,5,10,9};
    int counter =0;
    
    for (int j = 0; j < array_size; j++){
        Vector3f gap_x = Vector3f(0,0,0.1);
        Vector3f gap_z = Vector3f(0.1,0,0);
        for (int i = 0; i < j_array[j]; i++) {
            
            if (triangles){
                if(i<j_array[j]-1){
                    if (lean_array[j]==1 || lean_array[j]==2){
                        Vector3f p1 = position[counter]-gap_x;
                        Vector3f p2 = position[counter]+gap_x;
                        float temp = gap_x.z();
                        float reduction = 0.1/(j_array[j]-1);
                        gap_x = Vector3f(0,0,temp-reduction);
                        Vector3f p3 = position[counter+1]-gap_x;
                        Vector3f p4 = position[counter+1]+gap_x;
                        
                        // top left, top right, bottom right, bottom left
                        glBegin(GL_QUADS);
                        glVertex3f(p3.x(), p3.y(), p3.z());
                        glVertex3f(p4.x(), p4.y(), p4.z());
                        glVertex3f(p2.x(), p2.y(), p2.z());
                        glVertex3f(p1.x(), p1.y(), p1.z());
                        glEnd();
                        
                        glBegin(GL_QUADS);
                        glVertex3f(p1.x(), p1.y(), p1.z());
                        glVertex3f(p2.x(), p2.y(), p2.z());
                        glVertex3f(p4.x(), p4.y(), p4.z());
                        glVertex3f(p3.x(), p3.y(), p3.z());
                        glEnd();
                        
                    }
                    else{
                        Vector3f p1 = position[counter]-gap_z;
                        Vector3f p2 = position[counter]+gap_z;
                        float temp = gap_z.x();
                        float reduction = 0.1/(j_array[j]-1);
                        gap_z = Vector3f(temp-reduction,0,0);
                        Vector3f p3 = position[counter+1]-gap_z;
                        Vector3f p4 = position[counter+1]+gap_z;
                        
                        // top left, top right, bottom right, bottom left
                        glBegin(GL_QUADS);
                        glVertex3f(p3.x(), p3.y(), p3.z());
                        glVertex3f(p4.x(), p4.y(), p4.z());
                        glVertex3f(p2.x(), p2.y(), p2.z());
                        glVertex3f(p1.x(), p1.y(), p1.z());
                        glEnd();
                        
                        glBegin(GL_QUADS);
                        glVertex3f(p1.x(), p1.y(), p1.z());
                        glVertex3f(p2.x(), p2.y(), p2.z());
                        glVertex3f(p4.x(), p4.y(), p4.z());
                        glVertex3f(p3.x(), p3.y(), p3.z());
                        glEnd();
                        
                    }
                    
                }
                
            }
            else{
                
                 Vector3f pos = position[counter];//  position of particle i. YOUR CODE HERE
                 glPushMatrix();
                 glTranslatef(pos[0], pos[1], pos[2]);
                 glutSolidSphere(0.075f,10.0f,10.0f);
                 glPopMatrix();
                 
                if (i<j_array[j]-1){
                    Vector3f p1 = position[counter];
                    Vector3f p2 = position[counter+1];
                    glLineWidth(3.5);
                    glBegin(GL_LINES);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glEnd();
                }
            }
            counter++;
        }
    }
}


