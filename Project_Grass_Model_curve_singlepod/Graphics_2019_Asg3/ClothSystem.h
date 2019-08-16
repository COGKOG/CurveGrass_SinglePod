#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "vecmath/include/vecmath.h"
#include <vector>
#include <GLUT/glut.h>
#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int num);
	vector<Vector3f> evalF(vector<Vector3f> state);
    vector<Vector3f> readPosition(vector<Vector3f>(state));
    vector<Vector3f> readVelocity(vector<Vector3f>(state));
    int indexOf(int i, int j);
    Vector3f springForce(Vector3f i,Vector3f j, float k, float r);
	void draw();

private:
};


#endif
