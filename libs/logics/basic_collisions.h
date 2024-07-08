extern int CHAR_SIZE;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int PIXEL_WIDTH;
extern int PIXEL_HEIGHT;
extern int PIXEL_MULTIPLIER;

void handleEntityLogic(struct entity *Character) {
    // Character->posX = Character->posX + Character->velX;
    // Character->posY = Character->posY + Character->velY;
    Character->posX = Character->posX + Character->velX;
    Character->posY = Character->posY + Character->velY;

    if (Character->posX < 0) {Character->posX + 1;}
    if (Character->posX > PIXEL_WIDTH) {Character->posX - 1;}
    if (Character->posY < 0) {Character->posX + 1;}
    if (Character->posY > PIXEL_HEIGHT) {Character->posX - 1;}
}
