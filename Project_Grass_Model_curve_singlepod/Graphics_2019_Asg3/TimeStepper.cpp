#include "TimeStepper.hpp"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    // request from particle system to compute the derivatives using evalF
    vector<Vector3f>state = particleSystem->getState();
    vector<Vector3f>derivatives = particleSystem->evalF(state);
    vector<Vector3f>newState;
    
    //update system state
    for(int i=0; i<state.size();i++){
        Vector3f next_state = state[i] + stepSize * derivatives[i];
        newState.push_back(next_state);
    }
    particleSystem->setState(newState);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f>current_state = particleSystem->getState();
    vector<Vector3f>f0 = particleSystem->evalF(current_state);
    
    // Get Next step of state from the forward euler method
    vector<Vector3f>f1_state;
    for(int i=0; i<current_state.size();i++){
        Vector3f next_state = current_state[i] + stepSize * f0[i];
        f1_state.push_back(next_state);
    }
    vector<Vector3f>f1 = particleSystem->evalF(f1_state);
    
    // Calculate new state
    vector<Vector3f>newState;
    for (int i=0; i<f1_state.size();i++){
        Vector3f temp = current_state[i] + 0.5 * stepSize * (f0[i] + f1[i]);
        newState.push_back(temp);
    }
    particleSystem->setState(newState);
}

///TODO: implement RK4 rule here
void RK4::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    // Get k1
    vector<Vector3f>current_state = particleSystem->getState();
    vector<Vector3f>k1 = particleSystem->evalF(current_state);
    
    // Get k2
    vector<Vector3f>k2_state;
    for(int i=0; i<current_state.size();i++){
        Vector3f k2_temp = current_state[i] + 0.5 * k1[i] * stepSize;
        k2_state.push_back(k2_temp);
    }
    vector<Vector3f>k2 = particleSystem->evalF(k2_state);
    
    // Get k3
    vector<Vector3f>k3_state;
    for(int i=0; i<k2_state.size();i++){
        Vector3f k3_temp = current_state[i] + 0.5 * k2[i] * stepSize;
        k3_state.push_back(k3_temp);
    }
    vector<Vector3f>k3 = particleSystem->evalF(k3_state);
    
    //Get k4
    vector<Vector3f>k4_state;
    for (int i=0; i<k3_state.size();i++){
        Vector3f k4_temp = current_state[i] + k3[i] * stepSize;
        k4_state.push_back(k4_temp);
    }
    vector<Vector3f>k4 = particleSystem->evalF(k4_state);
    
    //New State
    vector<Vector3f> newState;
    for(int i=0; i<current_state.size();i++){
        Vector3f temp = current_state[i] + 1/6.0 * (k1[i] + k2[i] + k3[i] + k4[i]) * stepSize;
        newState.push_back(temp);
    }
    particleSystem->setState(newState);
}
