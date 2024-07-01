extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int PIXEL_WIDTH;
extern int PIXEL_HEIGHT;

//#include "font.h"

#include <string.h>

extern int CHAR_WIDTH;
extern int CHAR_HEIGHT;


void drawMir(unsigned int *pixels) {
    for (int i = 0; i < 256 * 240; ++i) {
        unsigned char r = rand() % 256;
        unsigned char g = rand() % 256;
        unsigned char b = rand() % 256;
        pixels[i] = 0xFF000000 | (r << 16) | (g << 8) | b;
    }
}


// void renderText(const char* text, SDL_Color color, int x, int y, unsigned int *pixels) {
    
// }



// void renderText(const char* text, SDL_Color color, int x, int y, unsigned int *pixels, int screenWidth) {
//     int text_length = strlen(text);

//     for (int i = 0; i < text_length; i++) {
//         const uint8_t *bitmap = get_char_bitmap(text[i]);
//         if (bitmap == NULL) continue;

//         for (int row = 0; row < CHAR_HEIGHT; row++) {
//             for (int col = 0; col < CHAR_WIDTH; col++) {
//                 if (bitmap[row] & (0x80 >> col)) {
//                     for (int dy = 0; dy < 4; dy++) {
//                         for (int dx = 0; dx < 4; dx++) {
//                             int pixel_index = (y + row * 4 + dy) * screenWidth + (x + col * 4 + dx);
//                             pixels[pixel_index] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), color.r, color.g, color.b, color.a);
//                         }
//                     }
//                 }
//             }
//         }
//         x += CHAR_WIDTH * 4;
//     }
// }





































void renderText(const char* text, TTF_Font* font, SDL_Color color, int x, int y, unsigned int* pixels, int screenWidth, int screenHeight) {
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

            // Only update the pixel if it is not fully transparent
            if (alpha > 200) {
                pixels[dstIndex] = pixel;
            }
        }
    }

    // Unlock and free the formatted surface
    SDL_UnlockSurface(formattedSurface);
    SDL_FreeSurface(formattedSurface);
}