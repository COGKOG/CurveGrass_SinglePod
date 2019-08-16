#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include "vecmath/include/vecmath.h"
#include <vector>
#include <GLUT/glut.h>

#include "particleSystem.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	vector<Vector3f> readPosition(vector<Vector3f> state);
    vector<Vector3f> readVelocity(vector<Vector3f> state);
    Vector3f springForce(Vector3f i,Vector3f j, float k, float r);
	vector<Vector3f> evalF(vector<Vector3f> state);
	
	void draw();
	
};

#endif
