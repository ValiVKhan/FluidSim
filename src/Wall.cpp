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

        // Check if particle hits the wall
        if ((particleX >= startX && particleX <= endX && particleY >= startY && particleY <= endY) ||
        
            (particleX >= startX && particleX <= endX && particleY <= startY && particleY >= endY)) {
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
