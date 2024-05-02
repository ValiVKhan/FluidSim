#include <Wall.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <glad/glad.h>

// constructor
Wall::Wall(float startingX, float startingY, float endingX, float endingY, ParticleManager particleManager) {

  this->startX = startingX;
  this->startY = startingY;

  this->endX = endingX;
  this->endY = endingY;

  // get length
  double dx = endX - startX;
  double dy = endY - startY;

  this->renderCoordinateStartX = (startingX + particleManager.getLowerBoundX()) / (particleManager.getUpperBoundX() + particleManager.getLowerBoundX()) * 2.0f - 1.0f;
  this->renderCoordinateStartY = (startingY + particleManager.getLowerBoundY()) / (particleManager.getUpperBoundY() + particleManager.getLowerBoundY()) * 2.0f - 1.0f;
  this->renderCoordinateEndX = (endingX + particleManager.getLowerBoundY()) / (particleManager.getUpperBoundY() + particleManager.getLowerBoundY()) * 2.0f - 1.0f;
  this->renderCoordinateEndY = (endingY + particleManager.getLowerBoundY()) / (particleManager.getUpperBoundY() + particleManager.getLowerBoundY()) * 2.0f - 1.0f;


  this->length = sqrt(dx * dx + dy * dy);

  // Rendering
  // // Generate vertices for the wall line
    std::vector<float> vertices = {renderCoordinateStartX, renderCoordinateStartY, 0.0f, renderCoordinateEndX, renderCoordinateEndY, 0.0f};

    // Create OpenGL buffers and objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and buffer data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Wall::render(){
    
    glBindVertexArray(VAO);

    // Draw the line
    glDrawArrays(GL_LINES, 0, 2);

    // Unbind VAO
    glBindVertexArray(0);
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

        // Check if the particle intersects with the line segment
        float deltaX = endX - startX;
        float deltaY = endY - startY;
        float lenSq = deltaX * deltaX + deltaY * deltaY;
        float dotProduct = ((particleX - startX) * deltaX + (particleY - startY) * deltaY) / lenSq;

        // Get the closest point on the line
        float closestX, closestY;
        if (dotProduct < 0) {
            closestX = startX;
            closestY = startY;
        } else if (dotProduct > 1) {
            closestX = endX;
            closestY = endY;
        } else {
            closestX = startX + dotProduct * deltaX;
            closestY = startY + dotProduct * deltaY;
        }

        // Check if the distance from particle to closest point is less than particle radius
        float distSq = (particleX - closestX) * (particleX - closestX) + (particleY - closestY) * (particleY - closestY);
        float particleRadius = 0.01;
        if (distSq <= particleRadius + 0.5) {
            // Calculate the normal vector of the wall
            float wallNormalX = -deltaY;
            float wallNormalY = deltaX;
            float wallNormalLength = sqrt(wallNormalX * wallNormalX + wallNormalY * wallNormalY);
            wallNormalX /= wallNormalLength;
            wallNormalY /= wallNormalLength;

            // Check if the particle is moving towards the wall
            float dotProduct = particleVelX * wallNormalX + particleVelY * wallNormalY;
            if (dotProduct < 0) {
                // Reflect the velocity of the particle
                float reflectionX = particleVelX - 2 * dotProduct * wallNormalX;
                float reflectionY = particleVelY - 2 * dotProduct * wallNormalY;

                velocities[i * 2] = reflectionX;
                velocities[i * 2 + 1] = reflectionY;
            }
            wallNormalX = deltaY;
            wallNormalY = -deltaX;
            wallNormalLength = sqrt(wallNormalX * wallNormalX + wallNormalY * wallNormalY);
            wallNormalX /= wallNormalLength;
            wallNormalY /= wallNormalLength;

            // Check if the particle is moving towards the wall
            dotProduct = particleVelX * wallNormalX + particleVelY * wallNormalY;
            if (dotProduct < 0) {
                // Reflect the velocity of the particle
                float reflectionX = particleVelX - 2 * dotProduct * wallNormalX;
                float reflectionY = particleVelY - 2 * dotProduct * wallNormalY;

                velocities[i * 2] = reflectionX;
                velocities[i * 2 + 1] = reflectionY;
            }
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
