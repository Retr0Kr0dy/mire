struct level {
    char *name;
    int sizeX; // number of panel
    int sizeY; // number of panel
    unsigned int* background;
    unsigned int* texture;

    struct entity Entities[255];
    unsigned int *WorldBlock[255];
};

struct level levels_list[128];

// struct level MainMenu;
// MainMenu.name = "MainMenu";

// levels_list[0] = MainMenu;


struct level loadLevel(int ID) {
//    if (ID == 0) {;}
    struct level Current;

    if (ID == 1) {
        
        uint32_t* load_png_to_argb(const char* filepath, int* width, int* height);

        Current.name = "test_room";
        Current.sizeX = 1; // number of panel
        Current.sizeY = 1; // number of panel


        const char* filepath = "assets/bitmaps/levels/1/background.png"; // Replace with your PNG file path
        int width, height;

        Current.background = load_png_to_argb(filepath, &width, &height);
        //Current.texture;

        // MAIN CHARACTER (mode 1)
        filepath = "assets/bitmaps/entities/PNG/main_cahr.png"; // Replace with your PNG file path

        struct entity ett;
        
        ett.sizeX = 16;
        ett.sizeY = 16;
        ett.texture = load_png_to_argb(filepath, &width, &height);
        ett.posX = 200;
        ett.posY = 100;
        ett.velX = 0.0f;
        ett.velY = 0.0f;
        ett.gravity = 0.9f;
        ett.friction = 0.9f;

        Current.Entities[0] = ett;
        // unsigned int *WorldBlock[255];

    }

    return Current;
}