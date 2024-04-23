#define GRAVITY_ACCELERATION -9.8
 
class ParticleManager {

    /**
     * General Notes
     * 
     * Units:
     * time: seconds
     * distance: each coordinate is a meter ig?
     * 
     * Coordinate Space:
     * bottom left of GUI is (lowerBoundX, lowerBoundY)
     * top right of GUI is (upperBoundX, upperBoundY)
     * 
     * Positions and Velocites:
     * x position of particle i denoted as positions[2 * i]
     * y position of particle i denoted as positions[2 * i + 1]
     * 
     * x velocity of particle i denoted as velocites[2 * i]
     * y velocity of particle i denoted as velocites[2 * i + 1]
     * 
     * Densities
     * Used as a buffer to precomute values to reduce recalcualations
     * Density of particle i denoted as density[i]
    */

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