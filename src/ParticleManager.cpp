#include <ParticleManager.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <stdio.h>

ParticleManager::ParticleManager(int numParticles) {
    this->numParticles = numParticles;
    positions = new float[numParticles * 2];
    velocities = new float[numParticles * 2]();
    densities = new float[numParticles];

    lowerBoundX = 0;
    upperBoundX = 100;
    lowerBoundY = 0;
    upperBoundY = 100;

    targetDensity = numParticles / ((upperBoundX - lowerBoundX) * (upperBoundY - lowerBoundY));

    particleMass = 1;

    //randomly initialize positions
    for (int i = 0; i < numParticles; i++) {
        positions[i * 2] =  static_cast<float>(std::rand()) / RAND_MAX * (upperBoundX - lowerBoundX) - lowerBoundX;
        positions[i * 2 + 1] = static_cast<float>(std::rand()) / RAND_MAX * (upperBoundY - lowerBoundY) - lowerBoundY;
    }

    // // Delete this later, just for shits and giggles
    // for (int i = 0; i < numParticles; i++) {
    //     velocities[i * 2] =  static_cast<float>(std::rand()) / RAND_MAX * 10 - 5;
    //     velocities[i * 2 + 1] = static_cast<float>(std::rand()) / RAND_MAX * 10 - 5;
    // }
}

void ParticleManager::step(float delta) {

    precomputeDensities();

    float halfStep = delta / 2;
    int range = numParticles * 2;

    /* (1) update positions half step */
    for (int i = 0; i < range; i++) {
        // printf("POSITION: (%f)\n", positions[i]);
        positions[i] += velocities[i] * halfStep;
    }

    /* (2 & 3) calc acceleration and calc velocities */

    // gravity
    for (int i = 1; i < range; i += 2) {
        velocities[i] += GRAVITY_ACCELERATION * delta;
    }

    float forceX;
    float forceY;

    for (int i = 0; i < numParticles; i++) {

        preasureForce(i, forceX, forceY);

        // std::cout << forceX << "\n";

        // velocities[i * 2] += forceX / densities[i] * delta; //TODO: divide by densities
        // velocities[i * 2 + 1] += forceY / densities[i] * delta;

        velocities[i * 2] += forceX * delta; //TODO: divide by densities
        velocities[i * 2 + 1] += forceY * delta;

        // printf("PFORCE Y\n: (%f):", forceY);
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
        velocities[2 * particleIndex] *= -DAMPING;

    } else if (positions[2 * particleIndex] > upperBoundX) {
        
        positions[2 * particleIndex] = upperBoundX;
        velocities[2 * particleIndex] *= -DAMPING;
    }

    /* y bound check */
    if (positions[2 * particleIndex + 1] < lowerBoundY) {

         positions[2 * particleIndex + 1] = lowerBoundY;
        velocities[2 * particleIndex + 1] *= -DAMPING;

    } else if (positions[2 * particleIndex + 1] > upperBoundY) {

        positions[2 * particleIndex + 1] = upperBoundY;
        velocities[2 * particleIndex + 1] *= -DAMPING;
        
    }

}

float ParticleManager::smoothingKernal(float distance) { //TODO: DIVIDE KERNAL BY THE VOLUME OF THE KERNAL
    if (distance > SMOOTHING_RADIUS) return 0;
    float val = SMOOTHING_RADIUS - distance;
    return 15.0f / (PI * SMOOTHING_RADIUS_POW_6) * val * val * val / 1.875;
}

float ParticleManager::gradientKernal(float distance) {
    if (distance > SMOOTHING_RADIUS) return 0;
    float val = SMOOTHING_RADIUS - distance;
    return 45.0f / (PI * SMOOTHING_RADIUS_POW_6) * val * val; // /120
}

float ParticleManager::laplacianKernal(float distance) {
    if (distance > SMOOTHING_RADIUS) return 0;
    float val = SMOOTHING_RADIUS - distance;
    return -90.0f / (PI * SMOOTHING_RADIUS_POW_6) * val;
}

void ParticleManager::preasureForce(int particleIndex, float& forceX, float& forceY) {

    float pi = (densities[particleIndex] - targetDensity) * IDEAL_GAS_CONSTANT;

    forceX = 0.0;
    forceY = 0.0;

    for (int i = 0; i < numParticles; i++) {
        
        if (particleIndex == i) continue;

        float pj = (densities[i] - targetDensity) * IDEAL_GAS_CONSTANT;

        float deltaX = positions[2 * i] - positions[2 * particleIndex];
        float deltaY = positions[2 * i + 1] - positions[2 * particleIndex + 1];
        float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);

        float dirX = deltaX / magnitude;
        float dirY = deltaY / magnitude;

        // printf("DIRX (%f), DIRY (%f), MAGNITUDE (%f), CALCULATED MAGNITUDE of DIR(%f)\n", dirX, dirY, magnitude, dirX * dirX + dirY * dirY);

        float gradient = gradientKernal(magnitude);


        // forceX += particleMass * (pi + pj) / (2 * pj) * gradient * dirX / densities[i];
        // forceY += particleMass * (pi + pj) / (2 * pj) * gradient * dirY / densities[i];

        forceX += particleMass * (pi + pj) / (2 * pj) * gradient * dirX;
        forceY += particleMass * (pi + pj) / (2 * pj) * gradient * dirY; 

        // printf("WHAT GOES INTO ONE FORCE (%f): MASS (%f), PI (%f), PJ (%f), KERNAL (%f), DIRECTION (%f), DISTANCE (%f)\n", forceX, particleMass, pi, pj, gradientKernal(magnitude), dirX, magnitude);

    }

    forceX *= -1;
    forceY *= -1;

}


void ParticleManager::precomputeDensities() {

    for (int currParticle = 0; currParticle < numParticles; currParticle++) {

        float density = 0;

        for (int i = 0; i < numParticles; i++) {

            float delta_x = positions[2 * i] - positions[2 * currParticle];
            float delta_y = positions[2 * i + 1] - positions[2 * currParticle + 1];
            float distance = sqrt(delta_x * delta_x + delta_y * delta_y);
            density += smoothingKernal(distance);
        }

        // std::cout << smoothingKernal(density) << "\n";

        densities[currParticle] = density * particleMass;
    }
}

float* ParticleManager::getPositions() {
    return positions;
}

float* ParticleManager::getVelocities() {
    return velocities;
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


