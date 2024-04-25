#include <Wall.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <stdio.h>

// constructor
Wall::Wall(float startingX, float startingY, float endingX, float endingY) {

  this->startX = startingX;
  this->startY = startingY;

  this->endX = endingX;
  this->endY = endingY;

  // get length
  double dx = endX - startX;
  double dy = endY - startY;

  this->length = sqrt(dx * dx + dy * dy);
}



void Wall::HandleWallCollision(ParticleManager particles) {
    float* positions = particles.getPositions();
    float* velocities = particles.getVelocities();
    int numParticles = particles.getNumParticles();

    for (int i = 0; i < numParticles; i++) {
        float particleX = positions[i * 2];
        float particleY = positions[i * 2 + 1];
        float particleVelX = velocities[i * 2];
        float particleVelY = velocities[i * 2 + 1];

        // Check if particle hits the wall
        if (particleX <= startX || particleX >= endX || particleY <= startY || particleY >= endY) {
            // Adjust velocity based on wall normal

            // Calculate the normal vector of the wall
            float wallNormalX = endY - startY; // Normal points away from the wall
            float wallNormalY = startX - endX;
            float length = sqrt(wallNormalX * wallNormalX + wallNormalY * wallNormalY);
            wallNormalX /= length;
            wallNormalY /= length;

            // Calculate dot product between velocity and wall normal
            float dotProduct = particleVelX * wallNormalX + particleVelY * wallNormalY;

            // Reflect velocity vector
            float reflectionX = particleVelX - 2 * dotProduct * wallNormalX;
            float reflectionY = particleVelY - 2 * dotProduct * wallNormalY;

            // Update particle velocity
            velocities[i * 2] = reflectionX;
            velocities[i * 2 + 1] = reflectionY;
        }
    }
}

// Getter method for startX
float Wall::getStartX() const {
  return this->startX;
}

float Wall::getStartY() const {
  return this->startY;
}

float Wall::getEndX() const {
  return this->endX;
}

float Wall::getEndY() const {
  return this->endY;
}
