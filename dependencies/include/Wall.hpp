#include <ParticleManager.hpp>
#define WALL_THICKNESS 0.2f

class Wall {
private:
    // positions
    float startX;
    float startY;
    float endX;
    float endY;
    float renderCoordinateStartX;
    float renderCoordinateStartY;
    float renderCoordinateEndX;
    float renderCoordinateEndY;

    unsigned int VBO, VAO;

    // self data
    float length;

public:
    // constructor
    Wall(float startingX, float startingY, float endingX, float endingY, ParticleManager particleManger);

    // Handle wall collision, adjust velocities of all interacting particles
    void HandleWallCollision(ParticleManager particles);

    void render();

    float getStartX() const;
    float getStartY() const;
    float getEndX() const;
    float getEndY() const;
};