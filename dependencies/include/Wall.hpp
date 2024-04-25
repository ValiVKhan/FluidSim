#include <ParticleManager.hpp>
#define WALL_THICKNESS = 0.2f;

class Wall {

    private:
        // postions
        float startX;
        float startY;
        float endX;
        float endY;

        // self data
        float length;

    public:
        // constructor
        Wall(float startingX, float startingY, float endingX, float endingY);

        // Handle wall collision, adjust velocities of all interacting particles
        void HandleWallCollision (ParticleManager particles);

        float getStartX() const;
        float getStartY() const;
        float getEndX() const;
        float getEndY() const;


};
