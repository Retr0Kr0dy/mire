extern int CHAR_SIZE;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int PIXEL_WIDTH;
extern int PIXEL_HEIGHT;
extern int PIXEL_MULTIPLIER;


extern char *FONT_1;
extern char *FONT_2;
extern char *FONT_3;

#include <string.h>

extern int CHAR_WIDTH;
extern int CHAR_HEIGHT;


SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

SDL_Color blue = {255, 0, 0, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color red = {0, 0, 255, 255};

void drawMir(unsigned int *pixels) {
    for (int i = 0; i < 256 * 240; ++i) {
        unsigned char r = rand() % 256;
        unsigned char g = rand() % 256;
        unsigned char b = rand() % 256;
        pixels[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
    }
}


void renderText(const char* text, int CHAR_SIZE , char *font_name, SDL_Color color, int x, int y, unsigned int* pixels, int screenWidth, int screenHeight) {
    // Load font
    TTF_Font *font = TTF_OpenFont(font_name, CHAR_SIZE);  // Change the path to your font file
    if (!font) {
        printf("TTF_OpenFont failed: %s\n", TTF_GetError());
        return;
    }

    // Render the text to an SDL surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (!textSurface) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Get the text surface dimensions
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    // Create a new SDL surface in the format of the screen
    SDL_Surface* formattedSurface = SDL_CreateRGBSurfaceWithFormat(0, textWidth, textHeight, 32, SDL_PIXELFORMAT_RGBA32);
    if (!formattedSurface) {
        printf("Unable to create formatted surface! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Blit the text surface onto the formatted surface
    SDL_BlitSurface(textSurface, NULL, formattedSurface, NULL);
    SDL_FreeSurface(textSurface);

    // Lock the formatted surface for raw pixel access
    SDL_LockSurface(formattedSurface);

    // Get the pixel data from the surface
    Uint32* surfacePixels = (Uint32*)formattedSurface->pixels;

    if (x == -1) { x = (screenWidth / 2) - (textWidth / 2); }
    y = y - PIXEL_MULTIPLIER;

    // Iterate over each pixel in the text surface and modify the corresponding pixel in the output array
    for (int j = 0; j < textHeight; ++j) {
        for (int i = 0; i < textWidth; ++i) {
            int srcIndex = j * textWidth + i;
            int dstIndex = (y + j) * screenWidth + (x + i);

            if (x + i < 0 || x + i >= screenWidth || y + j < 0 || y + j >= screenHeight) {
                continue; // Skip pixels that are out of bounds
            }

            Uint32 pixel = surfacePixels[srcIndex];
            Uint8 alpha = pixel >> 24; // Extract the alpha value

            // // Only update the pixel if it is not fully transparent
            // if (alpha > 200) {
            //     pixels[dstIndex] = pixel;
            // }

            if (alpha > 0) { // Only blend if the pixel is not fully transparent
                Uint8 srcR = (pixel >> 16) & 0xFF;
                Uint8 srcG = (pixel >> 8) & 0xFF;
                Uint8 srcB = pixel & 0xFF;

                Uint32 dstPixel = pixels[dstIndex];
                Uint8 dstR = (dstPixel >> 16) & 0xFF;
                Uint8 dstG = (dstPixel >> 8) & 0xFF;
                Uint8 dstB = dstPixel & 0xFF;
                Uint8 dstA = (dstPixel >> 24) & 0xFF;

                Uint8 outR = (srcR * alpha + dstR * (255 - alpha)) / 255;
                Uint8 outG = (srcG * alpha + dstG * (255 - alpha)) / 255;
                Uint8 outB = (srcB * alpha + dstB * (255 - alpha)) / 255;
                Uint8 outA = alpha + (dstA * (255 - alpha)) / 255;

                pixels[dstIndex] = (outA << 24) | (outR << 16) | (outG << 8) | outB;
            }
        }
    }

    // Unlock and free the formatted surface
    SDL_UnlockSurface(formattedSurface);
    SDL_FreeSurface(formattedSurface);
    TTF_CloseFont(font);
}


//void drawMenu(char* menuName, char** menuItem, unsigned int* pixels, int screenWidth, int screenHeight) {





// Function to draw a pixel in the pixels array
void drawPixel(int x, int y, Uint32 color, unsigned int* pixels, int screenWidth, int screenHeight) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        pixels[y * screenWidth + x] = color;
    }
}

// Function to draw a filled circle
void drawCircle(int centerX, int centerY, int radius, Uint32 color, unsigned int* pixels, int screenWidth, int screenHeight) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                drawPixel(centerX + x, centerY + y, color, pixels, screenWidth, screenHeight);
            }
        }
    }
}

// Function to draw a filled rectangle
void drawFilledRect(int x, int y, int width, int height, Uint32 color, unsigned int* pixels, int screenWidth, int screenHeight) {
    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            drawPixel(j, i, color, pixels, screenWidth, screenHeight);
        }
    }
}


// Function to draw the border of a circle (outline)
void drawCircleBorder(int centerX, int centerY, int radius, Uint32 color, unsigned int* pixels, int screenWidth, int screenHeight) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius && x * x + y * y >= (radius - 1) * (radius - 1)) {
                drawPixel(centerX + x, centerY + y, color, pixels, screenWidth, screenHeight);
            }
        }
    }
}

// Function to draw the border of a rectangle with rounded corners
void drawRectBorder(int x, int y, int width, int height, Uint32 color, unsigned int* pixels, int screenWidth, int screenHeight, int cornerRadius) {
    // Draw horizontal lines of the border
    for (int j = x + cornerRadius; j < x + width - cornerRadius; j++) {
        drawPixel(j, y, color, pixels, screenWidth, screenHeight);
        drawPixel(j, y + height - 1, color, pixels, screenWidth, screenHeight);
    }
    // Draw vertical lines of the border
    for (int i = y + cornerRadius; i < y + height - cornerRadius; i++) {
        drawPixel(x, i, color, pixels, screenWidth, screenHeight);
        drawPixel(x + width - 1, i, color, pixels, screenWidth, screenHeight);
    }
    // Draw the rounded corners
    drawCircleBorder(x + cornerRadius, y + cornerRadius, cornerRadius, color, pixels, screenWidth, screenHeight);
    drawCircleBorder(x + width - cornerRadius - 1, y + cornerRadius, cornerRadius, color, pixels, screenWidth, screenHeight);
    drawCircleBorder(x + cornerRadius, y + height - cornerRadius - 1, cornerRadius, color, pixels, screenWidth, screenHeight);
    drawCircleBorder(x + width - cornerRadius - 1, y + height - cornerRadius - 1, cornerRadius, color, pixels, screenWidth, screenHeight);
}

// Function to draw a rectangle with rounded corners
void drawRoundedRect(int x, int y, int width, int height, int cornerRadius, Uint32 color, unsigned int* pixels, int screenWidth, int screenHeight) {
    // Draw central filled rectangle
    drawFilledRect(x + cornerRadius, y, width - 2 * cornerRadius, height, color, pixels, screenWidth, screenHeight);
    drawFilledRect(x, y + cornerRadius, width, height - 2 * cornerRadius, color, pixels, screenWidth, screenHeight);

    // Draw corner circles
    drawCircle(x + cornerRadius, y + cornerRadius, cornerRadius, color, pixels, screenWidth, screenHeight);
    drawCircle(x + width - cornerRadius - 1, y + cornerRadius, cornerRadius, color, pixels, screenWidth, screenHeight);
    drawCircle(x + cornerRadius, y + height - cornerRadius - 1, cornerRadius, color, pixels, screenWidth, screenHeight);
    drawCircle(x + width - cornerRadius - 1, y + height - cornerRadius - 1, cornerRadius, color, pixels, screenWidth, screenHeight);

    // Draw the border (1 pixel inside)
    Uint32 borderColor = 0xFFB0B0B0; // Black color
    drawRectBorder(x + PIXEL_MULTIPLIER, y + PIXEL_MULTIPLIER, width - PIXEL_MULTIPLIER * 2, height - PIXEL_MULTIPLIER*2, borderColor, pixels, screenWidth, screenHeight, cornerRadius);

    drawFilledRect(x + cornerRadius, y + PIXEL_MULTIPLIER + 2 , width - 2 * cornerRadius, height - 3 * PIXEL_MULTIPLIER, color, pixels, screenWidth, screenHeight);
    drawFilledRect(x + PIXEL_MULTIPLIER + 2 , y + cornerRadius, width - 3 * PIXEL_MULTIPLIER, height - 2 * cornerRadius, color, pixels, screenWidth, screenHeight);


}

// Main function to draw the menu
void drawMenu(char* menuName, char* menuDesc, char** menuItem, int menuSize, unsigned int* pixels, int screenWidth, int screenHeight, int selector) {
    // Define menu properties
    int menuWidth = screenWidth * 0.9;
    int menuHeight = screenHeight * 0.9 ;
    int cornerRadius = 20;
    int menuX = (screenWidth - menuWidth) / 2;
    int menuY = (screenHeight - menuHeight) / 2;
    Uint32 menuColor = 0xFFC0C0C0; // ARGB color for the menu background



    // Draw the rounded rectangle for the menu background
    drawRoundedRect(menuX, menuY, menuWidth, menuHeight, cornerRadius, menuColor, pixels, screenWidth, screenHeight);

//    menuY = menuY - (menuY * PIXEL_MULTIPLIER);
    renderText(menuName, CHAR_SIZE * 3, FONT_3, black, -1, menuY + (CHAR_SIZE / 2) + 8, pixels, PIXEL_WIDTH, PIXEL_HEIGHT);
    renderText(menuDesc, CHAR_SIZE, FONT_3, black, -1, menuY + (CHAR_SIZE*4) + (CHAR_SIZE / 2), pixels, PIXEL_WIDTH, PIXEL_HEIGHT);

    for (int i = 0; i < 10; i++) {
        if (i > menuSize - 1) { 
            return; 
        }

        SDL_Color color = black;
        if (i == selector) {
            color = red;
        }
        char *elem = menuItem[i];
        if (elem == NULL) {
            elem = "";
        }

        renderText(elem, CHAR_SIZE*2, FONT_3, color, menuX + CHAR_SIZE*2, menuY + ((CHAR_SIZE*2)*4) + ((CHAR_SIZE*2)*(i*2)), pixels, PIXEL_WIDTH, PIXEL_HEIGHT);
    
    }
}



void drawMainMenu(unsigned int* pixels, int selector) {
    char *mainMenuName = "mire";
    char *mainMenuDesc = "Analog horror spaghetti shit ?";
    char *mainMenuItem[3] = {"Start game", "Settings", "Quit"};
    int mainMenySize = sizeof(mainMenuItem)/sizeof(mainMenuItem[0]);


    drawMenu(mainMenuName, mainMenuDesc ,mainMenuItem, mainMenySize, pixels, PIXEL_WIDTH, PIXEL_HEIGHT, selector);
}
