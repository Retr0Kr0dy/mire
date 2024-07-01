#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libs/entities.h"
#include "libs/graphics/basic_bitmap.h"
#include "libs/logics/basic_collisions.h"

int CHAR_SIZE = 16;
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 960;
int PIXEL_WIDTH = 256;
int PIXEL_HEIGHT = 240;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("mire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 960, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetSwapInterval(1);  // Enable vsync

    // Initialize OpenGL settings
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1024, 960, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // OpenGL texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Load font
    TTF_Font *font = TTF_OpenFont("assets/fonts/font.ttf", CHAR_SIZE);  // Change the path to your font file
    if (!font) {
        printf("TTF_OpenFont failed: %s\n", TTF_GetError());
        return 1;
    }


    // Create a pixel buffer
    //unsigned int pixels[256 * 240];

    unsigned int* pixels = (unsigned int*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(unsigned int));
    if (!pixels) {
        SDL_Log("Failed to allocate memory for pixel buffer");
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    memset(pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(unsigned int));


    SDL_Color white = {255, 255, 255, 255};

    // Main loop
    int running = 1;
    int mir = 1;

    SDL_Event event;
    Uint32 startTime = SDL_GetTicks();
    float fps = 0;
    int frameCount = 0;
    char debugText[32];

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        running = 0;
                        break;
                    case SDLK_a:
                        mir ^= 1;
                        break;
                }
            }
        }

        if (mir) { drawMir(pixels); }



        // Calculate FPS
        frameCount++;
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime >= 1000) {
            fps = frameCount / (elapsedTime / 1000.0f);
            startTime = SDL_GetTicks();
            frameCount = 0;
        }

        snprintf(debugText, sizeof(debugText), "FPS: %.2f", fps);

        renderText(debugText, font, white, 4, 4, pixels, PIXEL_WIDTH, PIXEL_HEIGHT);



        // Update the texture with the new pixel buffer
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 240, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

        // Clear the screen and render the texture
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
        for (int y = 0; y < 240; ++y) {
            for (int x = 0; x < 256; ++x) {
                float tx = (float)x / 256.0f;
                float ty = (float)y / 240.0f;
                float txNext = (float)(x + 1) / 256.0f;
                float tyNext = (float)(y + 1) / 240.0f;

                float vx = x * 4.0f;
                float vy = y * 4.0f;
                float vxNext = (x + 1) * 4.0f;
                float vyNext = (y + 1) * 4.0f;

                glTexCoord2f(tx, ty); glVertex2f(vx, vy);
                glTexCoord2f(txNext, ty); glVertex2f(vxNext, vy);
                glTexCoord2f(txNext, tyNext); glVertex2f(vxNext, vyNext);
                glTexCoord2f(tx, tyNext); glVertex2f(vx, vyNext);
            }
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    TTF_CloseFont(font);
    TTF_Quit();
    glDeleteTextures(1, &texture);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
