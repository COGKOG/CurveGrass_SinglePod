#ifndef GrassSystem_h
#define GrassSystem_h
#include "vecmath/include/vecmath.h"
#include <vector>
#include <GLUT/glut.h>
#include "particleSystem.h"

class GrassSystem: public ParticleSystem
{
    ///ADD MORE FUNCTION AND FIELDS HERE
    public:
    GrassSystem(int num);
    vector<Vector3f> evalF(vector<Vector3f> state);
    vector<Vector3f> readPosition(vector<Vector3f>(state));
    vector<Vector3f> readVelocity(vector<Vector3f>(state));
    Vector3f springForce(Vector3f i,Vector3f j, float k, float r);
    void draw();
    
    private:
    
};


#endif
