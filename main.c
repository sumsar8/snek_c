#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "linkedlist.h"

const int FPS = 10;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

struct Food {
    int x;
    int y;
};

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

void createGrid(SDL_Surface* screensurface);
void randomFoodPlacement(struct node* head, struct Food* f);
void drawGame(SDL_Renderer* renderer, struct node* head, struct Food* f);
bool checkBackward(int dirx, int diry, struct node* head);
bool headTouchesBody(struct node* head);

int main(int argc, char* args[]) {
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    srand(time(NULL));

    const float dt = 1000.0 / FPS;
    int starting_tick = 0;
    int timeoffset = 0;
    int dirx = 1;
    int diry = 0;

    SDL_Event e;
    bool quit = false;

    struct node* head = (struct node*)malloc(sizeof(struct node));
    head->next = NULL;
    head->x = SCREEN_WIDTH / 20;
    head->y = SCREEN_HEIGHT / 20;

    struct Food f;
    int eat = 0;
    randomFoodPlacement(head, &f);

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if (checkBackward(0, -1, head)) {
                            diry = -1;
                            dirx = 0;
                        }
                        break;
                    case SDLK_DOWN:
                        if (checkBackward(0, 1, head)) {
                            diry = 1;
                            dirx = 0;
                        }
                        break;
                    case SDLK_LEFT:
                        if (checkBackward(-1, 0, head)) {
                            diry = 0;
                            dirx = -1;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (checkBackward(1, 0, head)) {
                            diry = 0;
                            dirx = 1;
                        }
                        break;
                }
            }
        }

        starting_tick = SDL_GetTicks();
        timeoffset++;

        if (timeoffset == 5) {
            push(&head, dirx, diry);

            if (headTouchesBody(head)) {
                printf("\ngame over\ngame over\ngame over\n\n");
            }

            if (head->x == f.x && head->y == f.y) {
                randomFoodPlacement(head, &f);
                eat = 1;
            }

            if (eat != 1) {
                pop(head);
            }

            eat = 0;
            drawGame(renderer, head, &f);
            timeoffset = 0;
        }

        if (dt > SDL_GetTicks() - starting_tick) {
            SDL_Delay(dt - (SDL_GetTicks() - starting_tick));
        }
    }

    while (head->next != NULL) {
        pop(head);
    }
    pop(head);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawGame(SDL_Renderer* renderer, struct node* head, struct Food* f) {
    if (head == NULL) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    SDL_Rect temprect = {0, 0, SCREEN_WIDTH / 20, SCREEN_HEIGHT / 20};
    SDL_Rect frect = {f->x, f->y, SCREEN_WIDTH / 20, SCREEN_HEIGHT / 20};

    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 1);
    SDL_RenderFillRect(renderer, &frect);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 1);

    struct node* current = head;
    while (current != NULL) {
        temprect.x = current->x;
        temprect.y = current->y;
        SDL_RenderFillRect(renderer, &temprect);
        current = current->next;
    }

    SDL_RenderPresent(renderer);
}

void randomFoodPlacement(struct node* head, struct Food* f) {
    struct node* current = head;
    int success = 1;
    int x = rand() % 10;
    int y = rand() % 10;

    while (current != NULL) {
        if (current->x == x * SCREEN_WIDTH / 20 && current->y == y * SCREEN_WIDTH / 20) {
            success = 0;
            randomFoodPlacement(head, f);
        }
        current = current->next;
    }

    if (success == 1) {
        f->x = x * SCREEN_WIDTH / 20;
        f->y = y * SCREEN_WIDTH / 20;
    }
}

bool checkBackward(int dirx, int diry, struct node* head) {
    if (head->next == NULL) {
        return true;
    }

    struct node* neck = head->next;

    if (head->x + SCREEN_WIDTH / 20 * dirx == neck->x && head->y + SCREEN_WIDTH / 20 * diry == neck->y) {
        return false;
    }

    return true;
}

bool headTouchesBody(struct node* head) {
    struct node* current = head->next;
    while (current != NULL) {
        if (head->x == current->x && head->y == current->y || head->x > 499 || head->x < 0 || head->y > 499 || head->y < 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}