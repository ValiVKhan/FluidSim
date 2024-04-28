#define GRAVITY_ACCELERATION -0.01f
#define SMOOTHING_RADIUS 2.0f //idk bro, this doesnt seem like alot
#define SMOOTHING_RADIUS_POW_6 64.0f
#define PI 3.14159f
#define IDEAL_GAS_CONSTANT 100.0f //ideal gas constant? literally zero clue what this should be
#define DAMPING 0.9f;
 
class ParticleManager {

    /**
     * General Notes
     * 
     * Units:
     * time: seconds
     * distance: each coordinate/unit is a meter ig?
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
    float particleMass;
    float targetDensity;
    //float render coordinateX


    /* Pre Computes the Density for use in later calculations*/
    void precomputeDensities();

    /* Handle Collisions Basic (Boundary Collision) 
    **BAD** Adds/Removes energy into sim because of moving particle to boundary
    Additional damponing to reduce this effect
    */
    void handleCollisionsBasic();

    /* Handles basic collision for one particle */
    void handleOneCollisionBasic(int particleIndex);

    /* computer preasure force */
    void preasureForce(int particleIndex, float& forceX, float& forceY);

    /* Smoothing Kernal
    W(r) = 15/(pi * h^6) (h - r) ^ 3
    */
    float smoothingKernal(float distance);

    /* Gradient Kernal 
    gradient W(r) = 45/(pi * h^6) (h - r) ^ 2 **NOTE** SHOULD BE -45, ADJUSTED FOR POSITIVE GRADIENT TO MATCH PAPER
    */
    float gradientKernal(float distance);

    /* Laplacian Kernal 
    Laplacian W(r) = -90/(pi * h^6) (h - r)  **NOTE** SHOULD BE 90, ADJUSTED FOR POSITIVE GRADIENT TO MATCH PAPER
    */
    float laplacianKernal(float distance);


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
    float* getVelocities();


    float getLowerBoundX();
    float getUpperBoundX();
    float getLowerBoundY();
    float getUpperBoundY();

    int getNumParticles();


};
