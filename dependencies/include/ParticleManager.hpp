#define GRAVITY_ACCELERATION -9.8
 
class ParticleManager {

private:
    float* positions;
    float* velocities;
    float* densities;
    int numParticles;
    float lowerBoundX;
    float upperBoundX;
    float lowerBoundY;
    float upperBoundY;

    /* Pre Computes the Density for use in later calculations*/
    void precomputeDensities();

    /* Handle Collisions Basic (Boundary Collision) 
    
    **BAD** Adds/Removes energy into sim because of moving particle to boundary
    */
    void handleCollisionsBasic();

    /* Handles basic collision for one particle */
    void handleOneCollisionBasic(int particleIndex);


public:

    ParticleManager(int numParticles);


    /* Simulate one step through time of size delta

    Uses Leapfrog integration method:

    1) Position update by half delta step
    2) Acceleration calculated using updated position
    3) Velocity Updated by full delta step
    4) Position reupdated by another half delta step using new Velocity

    Position 
    */
    void step(float delta); 

 

    /* Do Not Modify Returned Pointer or Positions */
    float* getPositions();

    float getLowerBoundX();
    float getUpperBoundX();
    float getLowerBoundY();
    float getUpperBoundY();

    int getNumParticles();


};