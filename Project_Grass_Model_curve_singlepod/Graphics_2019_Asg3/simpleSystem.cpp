#define GL_SILENCE_DEPRECATION
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
    Vector3f startPos = (1, 1, 1);
    vector<Vector3f> state;
    
    state.push_back(startPos);
    this->setState(state);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

	// YOUR CODE HERE
    // Implement evalF in SimpleSystem which ineherits from the abstract Particle System class
    // Pass the system to the takeStep method, which takes care of updating the system's state
    
    for(int i=0; i<state.size();i++){
        Vector3f fx = Vector3f(-state[i].y(), state[i].x(), 0);
        f.push_back(fx);
    }
    return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
    for(int i=0; i < this->getState().size(); i++){
        Vector3f pos = this ->m_vVecState[i] ;//YOUR PARTICLE POSITION
        glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
    }
}
