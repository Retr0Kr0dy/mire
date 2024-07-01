struct entity {
    float posX;
    float posY;
    float velX;
    float velY;
    float gravity;
    float friction;
};

struct entity entities_list[1024];
int entities_count = 0;

void addEntity(float posX, float posY, float velX, float velY, float gravity, float friciton) {
    struct entity ett;

    ett.posX = posX;
    ett.posY = posY;
    ett.velX = velX;
    ett.velY = velY;
    ett.gravity = gravity;
    ett.friction = gravity;

    entities_list[entities_count] = ett;
    entities_count++;
}

