  //pong
//
//(c) etrvic
//
//28 Feb 2022
//
//
//                * --------------------------------------------------- *
//                | For this game I created a window and everything     |
//                | just as i would do for a sdl project. Then I made   |
//                | the ball move in cos and sin direction. Every       |
//                | time it would go to the right and left border       |
//                | I would calculate the new angle based on the old    |
//                | one and check if the paddle is beyond the ball      |
//                * --------------------------------------------------- *
//
//----------include----------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>// for cos and sin
#include <SDL2/SDL.h>

//-------runtime const--------
#define SH  480                  //screen height
#define SW  680                 //screen width
#define SPEED 10               // paddle speed
#define DR 0.0174533          // degree to rad

//-----SDL objects---------
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int game_on = 1; //while game
int KEYS[355];  //keys snapshot

int v = 20; // a value to add to the ball's possible outputs
SDL_Rect p1, p2; //players pad
SDL_Rect b; //ball
int bangle, xstep, ystep; // ball angle, ball next step



//-------init function--------
void init() {
    if(SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "can't initialize sdl:%s", SDL_GetError()); // catch error
        exit(1);
    } else {
        window  = SDL_CreateWindow("-PONG-", //window name
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, //screen pos
                                   SW,SH,//width, height
                                   0);
        if(window == NULL) {
            fprintf(stderr, "can't initialize create window:%s", SDL_GetError()); // error check
            exit(1);
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //create a renderer object
        }
    }
    
}


//-------set player position-------
void set_positions() {
    p1.h = p2.h = 32; //pedal height
    p1.w = p2.w = 8; //pedal width
    p1.y = p2.y = SH/2 - p1.h/2; // first coordonates
    p1.x = 16;
    p2.x = SW - p1.x;
    //init ball position
    b.h = 8;                    //height
    b.w = b.h;                  //width
    b.x = p1.x+p1.w+1;          //x coord
    b.y = p1.y -4 + p1.h/2 ;    //y coord
    bangle = rand() % 90 + 45;  //ball angle, first random case
}

//----------function to register key input---------
void get_keys() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: //if window is closed
                game_on = 0;
                break;
            case SDL_KEYDOWN: //if is a key down, then register it
                if(event.key.keysym.sym < 355)
                    KEYS[event.key.keysym.sym] = 1;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym < 355)// if it's up set the keys value to false
                    KEYS[event.key.keysym.sym] = 0; //event.key.keysym.sym gives the current key code
                break;
            default:
                break;
        }
    }
}

//---------function to act acordingly to the input----------
void read_keys() {  // add the speed to the coord
    if(KEYS[SDLK_w] == 1 && p1.y > 10) {
        p1.y -= SPEED;
    }
    if(KEYS[SDLK_s] == 1 && p1.y < SH - 38) {
        p1.y += SPEED;
    }
    if(KEYS[SDLK_i] == 1 && p2.y > 10) {
        p2.y-= SPEED;
    }
    if(KEYS[SDLK_k] == 1 && p2.y < SH - 38) {
        p2.y += SPEED;
    }
}

void set_background() { //set backround color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer); //clear previsios screen, should be called every frame at the begginig
}

void set_frame_background() {
    int i = 0;
    SDL_SetRenderDrawColor(renderer, 255,255,255,SDL_ALPHA_OPAQUE); // field color
    SDL_Rect r; //create a rectangle for the field
    r.h = SH/32; r.w = 8; r.x = SW/2 - 4; r.y = -SH/16.5; //set the fields coord
    for(i = 0; i <= 16; i++) {
        r.y = r.y + SH / 16.5;
        SDL_RenderFillRect(renderer, &r); //draw to screen
    }
}

void draw() {
    SDL_SetRenderDrawColor(renderer, 255,255,255,SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &p1);              //draw the p1
    SDL_RenderFillRect(renderer, &p2);              //draw the p2
    SDL_RenderFillRect(renderer, &b);               //draw the ball
    b.x += xstep;                                   //add the steps
    b.y += ystep;                                   //----,,-----
}

void move_ball() {
    xstep = sin(bangle*DR)*9;                      //calculate the correct xstep based on the sin
    ystep = cos(bangle*DR)*9;                      //calculate the correct xstep based on the cos
}

void end_ball() {
    while(b.x<SW && b.x > 0) { //incrment the ball until it reches the end of the screen
        b.x += xstep;
        b.y += ystep;
        set_background();
        set_frame_background();
        SDL_RenderFillRect(renderer, &b);
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
}

void check_new_angle() {
    //you chave for each border 2 checks:
    //one is for one ball direction
    //the other one for the other direction
    
    if(b.x > SW - 24 - 7) { //right border check
        if(b.y < p2.y || b.y > (p2.y + 32)) { // check if the paddel is in the right position
            printf("you_lost: %i %i ", b.y, p2.y);
            game_on = 0;
            end_ball();
        } else {
            if(bangle < 90)
                bangle = 270 + 45 - v  + rand() % 45;
            else
                bangle = 180 + v + rand() % 45;
            b.x -=2;
        }
    }
    else if (b.x < 24) { //left border check
        if(b.y < p1.y || b.y > p1.y + 32) {
            printf("you_lost\n");
            game_on = 0;
            end_ball();
        } else {
            if(bangle < 270)
                bangle = 90 + 45 - v + rand()%45;
            else
                bangle = v + rand()%45;
            b.x +=2;
        }
    }
    else if(b.y < 0) { // upper check
        if(bangle < 180)
            bangle = 45 - v + rand() % 45;
        else
            bangle = (270 + v + rand() %45 );
        b.y +=2;
    }
    else if(b.y > SH - 8) { // lower check
        if(bangle < 90)
            bangle = 90 + v +rand() % 45;
        else
            bangle  = (180 + 45 - v + rand()%45);
        b.y -= 2;
    }
    move_ball();
}

//-------close function--------

void close() {
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}


int main() {
    init();
    set_positions();
    move_ball();
    while (game_on) {
        set_background();
        set_frame_background();
        get_keys();
        read_keys();
        draw();
        check_new_angle();
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
    close();
}
