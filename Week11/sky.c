/* Display blue sky for 10 seconds. */

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// Fail, printing the SDL error message, and stop the program.
void fail() {
    fprintf(stderr, "%s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Check the results from SDL functions for errors.
int NotNeg(int n) { if (n < 0) fail(); return n; }
void *NotNull(void *p) { if (p == NULL) fail(); return p; }

int main() {
    int w=640, h=480;
    NotNeg(SDL_Init(SDL_INIT_VIDEO));
    SDL_Window *window = NotNull(SDL_CreateWindow("Sky", 100, 100, w, h, 0));

    SDL_Surface *surface = NotNull(SDL_GetWindowSurface(window));
    Uint32 skyBlue = SDL_MapRGB(surface->format, 100, 149, 237);
    NotNeg(SDL_FillRect(surface, NULL, skyBlue));
    NotNeg(SDL_UpdateWindowSurface(window));

    SDL_Delay(10000);
    SDL_Quit();
    return 0;
}
