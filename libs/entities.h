struct entity {
    int sizeX;
    int sizeY;
    unsigned int* texture;
    float posX;
    float posY;
    float velX;
    float velY;
    float gravity;
    float friction;
};

struct entity entities_list[1024];
int entities_count = 0;

void addEntity(float posX, float posY, float velX, float velY, float gravity, float friction) {
    struct entity ett;

    ett.posX = posX;
    ett.posY = posY;
    ett.velX = velX;
    ett.velY = velY;
    ett.gravity = gravity;
    ett.friction = friction;

    entities_list[entities_count] = ett;
    entities_count++;
}

