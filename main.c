#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

const int FPS = 10;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

struct node{
    int x;
    int y;
    struct node *next;
}; 

struct food{
    int x;
    int y;  
};

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

void createGrid(SDL_Surface *screensurface);
void push(struct node** head, int dirx, int diry);
void pop(struct node* head);
void pushatend(struct node* head);
void randomfoodplacement(struct node* head, struct food* f);
void drawgame(SDL_Renderer* renderer, struct node* head, struct food* f);
bool checkbackward(int dirx, int diry, struct node* head);
bool headtouchbody(struct node* head);
int main( int argc, char* args[] )
{
	SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1), SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    srand(time(NULL));
    int x = 50;
    SDL_Rect rect = {x,100,25,25};
	const float dt = 1000.0 / FPS;
    int starting_tick = 0;
    int timeoffset = 0;
    int dirx = 1;
    int diry = 0;
    SDL_Event e; bool quit = false; 

    struct node* head = (struct node*)malloc(sizeof(struct node));
    head->next = NULL;
    head->x = 50;
    head->y = 50;

    struct food f;
    int eat = 0;
    randomfoodplacement(head, &f);


    while(quit == false){
        while(SDL_PollEvent( &e )){
            if(e.type == SDL_QUIT){
                quit = true;
            }else if( e.type == SDL_KEYDOWN ){
						switch( e.key.keysym.sym ){

							case SDLK_UP:
                            
                            if(checkbackward(0,-1,head)){
                                diry = -1;
                                dirx = 0;
                            }
                            

                            //printf("up\n");
							break;

							case SDLK_DOWN:
                            if(checkbackward(0,1,head)){
                                diry = 1;
                                dirx = 0;
                            }
                            
                            //printf("down\n");
							break;

							case SDLK_LEFT:
                            if(checkbackward(-1,0,head)){
                                diry = 0;
                                dirx = -1;
                            }
                            
                            //printf("left\n");
							break;

							case SDLK_RIGHT:
                            if(checkbackward(1,0,head)){
                                diry = 0;
                                dirx = 1;
                            }
                            //printf("right\n");
							break;
						}
					}
        }
        starting_tick = SDL_GetTicks();

        timeoffset++;
        if(timeoffset == 5){
            rect.x+=rect.w;

            push(&head,dirx,diry);

            if(headtouchbody(head)){
                printf("\ngame over\ngameover\ngameover\n\n");
            }
            if(head->x == f.x && head->y == f.y){
                
                randomfoodplacement(head, &f);
                eat = 1;
            }
            if(eat != 1){
                pop(head);
            }
            eat = 0;
            //printf("%d %d\n",f.x,f.y);
            drawgame(renderer,head,&f);
            
            timeoffset = 0;
        }


           
        if ((dt) > SDL_GetTicks() - starting_tick){
                SDL_Delay(dt - (SDL_GetTicks() - starting_tick));
            }    
        }
    
    while(head->next != NULL){
        pop(head);
    }
    pop(head);
            //printf("%s",SDL_GetError());

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
void push(struct node** head, int dirx, int diry){
    struct node* newnode = (struct node*)malloc(sizeof(struct node));

    newnode->next = *head;
    newnode->x = (*head)->x + 50 * dirx;
    newnode->y = (*head)->y + 50 * diry;
    *head = newnode;
    //printf("add node at %d %d\n",(*head)->x,(*head)->y);
}
void pushatend(struct node* head) {
    struct node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = (struct node *) malloc(sizeof(struct node));
    current->next->x = head->x + 50;
    current->next->y = head->y + 50;
    current->next->next = NULL;
}
void pop(struct node* head){
    struct node* current = head;
    if (head->next == NULL) {
        free(head);
        //printf("remove head\n");
        return;
    }
    while(current->next->next != NULL){
        current = current->next;
    }
    //printf("remove node\n");
    free(current->next->next);
    current->next = NULL;
}

void drawgame(SDL_Renderer* renderer, struct node* head, struct food* f){
    if (head == NULL){
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    SDL_Rect temprect = {0, 0, 50, 50};
    SDL_Rect frect = {f->x,f->y,50,50};

    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 1);
    SDL_RenderFillRect(renderer,&frect);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 1);

    struct node* current = head;
    while (current != NULL) {
        temprect.x = current->x;
        temprect.y = current->y;
        printf("%d %d   ",temprect.x,temprect.y);
        SDL_RenderFillRect(renderer, &temprect);
        
        current = current->next;
    }
        printf("\n");

    SDL_RenderPresent(renderer);
}
void randomfoodplacement(struct node* head, struct food* f){
    struct node* current = head;
    int success = 1;
    int x = rand() % 10;
    int y = rand() % 10;
    while(current != NULL){
        if(current->x == x*50 && current->y == y*50){
            success = 0;
            randomfoodplacement(head,f);
            
        }
        current = current->next;
    }
    if(success == 1){
        f->x = x * 50;
        f->y = y * 50;
    }
}
bool checkbackward(int dirx, int diry, struct node* head){
    if(head->next == NULL){
        return true;
    }
    struct node* neck = head->next;
    if(head->x + 50 * dirx == neck->x && head->y + 50 * diry == neck->y){
        return false;
    }
    return true;
}
bool headtouchbody(struct node* head){
    struct node* current = head->next;
    while(current != NULL){
        if(head->x == current->x && head->y == current->y || head->x > 499 || head->x < 0 || head->y > 499 || head->y < 0){
            return true;
        }
        current = current->next;
    }
    return false;
}