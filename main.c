#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "gamelogic.h"

int main() {
  srand(time(0));
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  SDL_Event event;
  SDL_Init(SDL_INIT_EVERYTHING);
  
  SDL_CreateWindowAndRenderer(SCREENWIDTH, SCREENHEIGHT, 0, &window, &renderer);
  SDL_SetWindowTitle(window, "Click to move; Click to start.");
  
  // SDL_Surface *icon = IMG_Load("chase.jpg");
  // SDL_SetWindowIcon(window, icon);
  // I'm not getting the SDL2_image dev just for an icon...

  int mouseX = 0, mouseY = 0;
  int running = 1;
  int prevTicks = SDL_GetTicks();
  int ticks = SDL_GetTicks();
  float deltaTime = 0.0;

  // main game loop
  while (running) {
    ticks = SDL_GetTicks();
    deltaTime = (ticks - prevTicks)*0.2;

    while (SDL_PollEvent(&event)) {      
      if (event.type == SDL_QUIT) { running = 0; }
      
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&mouseX, &mouseY);
        onMouseDown(event.button, mouseX, mouseY);
      }
    }

    follow(&player, cursor);
    chase();
    unCollideWall(&cursor);
    unCollideWall(&player);
    healthBar.w = (player.health*(SCREENWIDTH/100))-10;
    
    if (player.health <= 0) {
      player.health = 0;
      running = 0;
      printf("Score: %d\n", SDL_GetTicks());
    } 


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    prevTicks = SDL_GetTicks();

    draw(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);

  }

  return 0;
}

