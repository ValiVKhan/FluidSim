#include <ParticleManager.hpp>
#include <random>


ParticleManager::ParticleManager(int numParticles) {
    this->numParticles = numParticles;
    positions = new float[numParticles * 2];
    velocities = new float[numParticles * 2]();
    densities = new float[numParticles]();

    lowerBoundX = 0;
    upperBoundX = 800;
    lowerBoundY = 0;
    upperBoundY = 800;

    //randomly initialize positions
    for (int i = 0; i < numParticles; i++) {
        positions[i * 2] =  static_cast<float>(std::rand()) / RAND_MAX * (upperBoundX - lowerBoundX) - lowerBoundX;
        positions[i * 2 + 1] = static_cast<float>(std::rand()) / RAND_MAX * (upperBoundY - lowerBoundY) - lowerBoundY;
    }

    //Delete this later, just for shits and giggles tbh
    for (int i = 0; i < numParticles; i++) {
        velocities[i * 2] =  static_cast<float>(std::rand()) / RAND_MAX * 500 - 250;
        velocities[i * 2 + 1] = static_cast<float>(std::rand()) / RAND_MAX * 500 - 250;
    }
}

void ParticleManager::step(float delta) {

    float halfStep = delta / 2;
    int range = numParticles * 2;

    /* (1) update positions half step */
    for (int i = 0; i < range; i++) {
        positions[i] += velocities[i] * halfStep;
    }

    /* (2 & 3) calc acceleration and calc velocities */

    // gravity
    for (int i = 1; i < range; i += 2) {
        velocities[i] += GRAVITY_ACCELERATION;
    }

    /* (4) update positions half step */

    for (int i = 0; i < range; i++) {
        positions[i] += velocities[i] * halfStep;
    }

    handleCollisionsBasic();
}

void ParticleManager::handleCollisionsBasic() {

    for (int i = 0; i < numParticles; i++) {
        handleOneCollisionBasic(i);
    }
    
}

void ParticleManager::handleOneCollisionBasic(int particleIndex) {
    
    /* x bound check */
    if (positions[2 * particleIndex] < lowerBoundX) {

        positions[2 * particleIndex] = lowerBoundX;
        velocities[2 * particleIndex] *= -1;

    } else if (positions[2 * particleIndex] > upperBoundX) {
        
        positions[2 * particleIndex] = upperBoundX;
        velocities[2 * particleIndex] *= -1;
    }

    /* y bound check */
    if (positions[2 * particleIndex + 1] < lowerBoundY) {

         positions[2 * particleIndex + 1] = lowerBoundY;
        velocities[2 * particleIndex + 1] *= -1;

    } else if (positions[2 * particleIndex + 1] > upperBoundY) {

        positions[2 * particleIndex + 1] = upperBoundY;
        velocities[2 * particleIndex + 1] *= -1;
        
    }

}


void ParticleManager::precomputeDensities() {
    return;
}

float* ParticleManager::getPositions() {
    return positions;
}

float ParticleManager::getLowerBoundX() {
    return lowerBoundX;
}

float ParticleManager::getUpperBoundX() {
    return upperBoundX;
}

float ParticleManager::getLowerBoundY() {
    return lowerBoundY;
}

float ParticleManager::getUpperBoundY() {
    return upperBoundY;
}

int ParticleManager::getNumParticles() {
    return numParticles;
}


