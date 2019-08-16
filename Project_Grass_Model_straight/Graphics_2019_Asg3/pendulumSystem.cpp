#define GL_SILENCE_DEPRECATION
#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
    vector<Vector3f> state;
    
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		// for this system, we care about the position and the velocity
        state.push_back(Vector3f(i, 0, 0));
        state.push_back(Vector3f(0, 0, 0));
	}
    
    this->setState(state);
}

// Helper functions
// Store a big Vector3f arry of size 2n where positions are stored at even indices and velocities at odd indices.
// It might help to write helper functions that read the position or velocity of particle i.
vector<Vector3f> PendulumSystem::readPosition(vector<Vector3f>(state)){
    vector<Vector3f> position;
    for (int i = 0; i < state.size(); i+=2){
        position.push_back(state[i]);
    }
    return position;
}

vector<Vector3f> PendulumSystem::readVelocity(vector<Vector3f>(state)){
    vector<Vector3f> velocity;
    for (int i = 0; i < state.size(); i+=2){
        velocity.push_back(state[i+1]);
    }
    return velocity;
}

// Helper function to compute spring force
Vector3f PendulumSystem::springForce(Vector3f i,Vector3f j, float k, float r){
    Vector3f distance = Vector3f(i.x()-j.x(), i.y()-j.y(), i.z()-j.z());
    return -1.0 * k * (distance.abs() - r) * (distance.normalized());
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
    // call helper function to get position and velocity
    vector<Vector3f> position = readPosition(state);
    vector<Vector3f> velocity = readVelocity(state);
    
	// YOUR CODE HERE
    // constants: mass(m), gravity(g), drag(k), rest length (r), spring constant(k)
    float mass = 0.5;
    Vector3f gravity = Vector3f(0, -9.81, 0);
    float drag = 0.5;
    float rest_length = 0.5;
    float spring_constant = 500.0;
    
    // Initial velocity and acceleration is zero
    f.push_back(Vector3f(0,0,0));
    f.push_back(Vector3f(0,0,0));
    
    for (int i = 1; i < m_numParticles; i++) {
        f.push_back(velocity[i]);
        Vector3f force = (gravity * mass) + (-1 * drag * velocity[i]);
        // calculate spring force
        if (i == m_numParticles-1){
            force += springForce(position[i],position[i-1],spring_constant, rest_length);
            f.push_back(force/mass);
        }
        else{
            force += springForce(position[i],position[i+1],spring_constant, rest_length);
            force += springForce(position[i],position[i-1],spring_constant, rest_length);
            f.push_back (force/mass);
        }
    }
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
    vector<Vector3f> position = this->readPosition(this->getState());
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = position[i];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
        if (i<m_numParticles-1){
            Vector3f p1 = position[i];
            Vector3f p2 = position[i+1];
            glLineWidth(3.5);
            glBegin(GL_LINES);
            glVertex3f(p1[0], p1[1], p1[2]);
            glVertex3f(p2[0], p2[1], p2[2]);
            glEnd();
        }
	}
}
