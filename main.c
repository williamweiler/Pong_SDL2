#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 700

//Option de build dans les linker setting mettre les libSDL2main.a libSDL2.dll.a
//Dans search directory
//Option de build compiler mettre les include
//Option de build linker mettre les lib

//mettre dans build linker -lmingw32



typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

typedef struct coordonnees{//creation des coordonnees

    double x;
    double y;

}coordonnees;

typedef enum SensH//coordonnes de direction de la balle haut bas
{
    Montee,Descente
} SensH;

typedef enum SensW//coordonnes de direction de la balle gauche droite
{
    Gauche,Droite
} SensW;

//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void destroy(game *myGame);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *dep3);
SensW move_BallW(coordonnees *dep3,SensW sens_w,coordonnees *dep,coordonnees *dep2);
SensH move_BallH(coordonnees *dep3,SensH sens_h,coordonnees *dep,coordonnees *dep2);
void delay(unsigned int frameLimit);
void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2 ,coordonnees *dep3);




int main(int argc, char *argv[])
{
        //initialisation des variables
     game myGame;
     gameState state;
     unsigned int frameLimit = SDL_GetTicks() + 16;
     coordonnees dep;
     coordonnees dep2;
     coordonnees dep3;
     dep3.x=(SCREEN_WIDTH/2)-10;
     dep3.y=(SCREEN_HEIGHT/2)-10;
     int choix=0;

     SensH sens_h;
     SensW sens_w;
     sens_h=Descente;
     sens_w=Droite;
     state.g_bRunning=1;
        //Create texture for drawing in texture or load picture
        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);
        while(state.g_bRunning){
        if (choix==0)
        {
            printf("1:Pong\n");
            scanf("%i",&choix);
        }
        if (choix==1){
        init("Chapter 1 setting up SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);
            while(state.g_bRunning){

                                    sens_h=move_BallH(&dep3,sens_h,&dep,&dep2);//fonction direction balle
                                    sens_w=move_BallW(&dep3,sens_w,&dep,&dep2);//fonction direction balle
                                    handleEvents(&state,&dep,&dep2,&dep3);
                                    renderTexture(&myGame,&dep,&dep2,&dep3);
                                    }

                 }
                 system ("cls");



             // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            }delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;



        //}

        destroy(&myGame);

        SDL_Quit();


    return 0;
}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }



    return 1;



}




void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *dep3) {


       //Définition du raquette1 a dessiner
        SDL_Rect rectangle;
        rectangle.x=0;//debut x
        rectangle.y=dep->y;//debut y
        rectangle.w=10; //Largeur
        rectangle.h=100; //Hauteur

        if (rectangle.y<=0){
            rectangle.y=0;
        }

        //Définition du rraquette2 a dessiner
        SDL_Rect rectangle2;
        rectangle2.x=SCREEN_WIDTH-10;//debut x
        rectangle2.y=dep2->y;//debut y
        rectangle2.w=10; //Largeur
        rectangle2.h=100; //Hauteur

        if (rectangle2.y<=0){
            rectangle2.y=0;
        }


        //Définition de la balle a dessiner
        SDL_Rect ballon;
        ballon.x=dep3->x;//debut x
        ballon.y=dep3->y;//debut y
        ballon.w=15; //Largeur
        ballon.h=15; //Hauteur

        //Draw in texture
        SDL_SetRenderDrawColor(myGame->g_pRenderer,230,230,230,230);
        SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);
        SDL_RenderFillRect(myGame->g_pRenderer, &ballon);

        SDL_SetRenderTarget(myGame->g_pRenderer, NULL);// Dorénavent, on modifie à nouveau le renderer

        SDL_Rect position;//position de la texture dans screen
        position.x = 0;
        position.y = 0;
        SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);

         SDL_RenderPresent(myGame->g_pRenderer);
         SDL_SetRenderDrawColor(myGame->g_pRenderer,800,800,800,800);
         SDL_DestroyTexture(myGame->g_ptexture);
         SDL_RenderClear(myGame->g_pRenderer);




}

void destroy(game *myGame){

      //Destroy render
     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

      //Destroy texture
     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Destroy surface
     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *dep3){


    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                        switch (event.key.keysym.sym)
                            {
                                case SDLK_UP:    dep2->y-=7; break;//controlle raquette1 vers le haut et le bas
                                case SDLK_DOWN:  dep2->y+=7; break;
                                case SDLK_z:     dep->y-=7; break;//controlle raquette2 vers le haut et le bas
                                case SDLK_s:     dep->y+=7;; break;
                            }
                            break;

        case SDL_KEYUP:;break;

        default:break;

        }
    }


}


SensH move_BallH(coordonnees *dep3,SensH sens_h,coordonnees *dep,coordonnees *dep2)//mouvement balle avec rebond haut bas
{
 if (sens_h==Montee)
{
    dep3->y-=2;
}
else if (sens_h==Descente)
{
 dep3->y+=2;
}
if (dep3->y>SCREEN_HEIGHT-20)
{
    sens_h=Montee;
}
else if (dep3->y<0)
{
    sens_h=Descente;
}

return sens_h;
}



SensW move_BallW(coordonnees *dep3,SensW sens_w,coordonnees *dep,coordonnees *dep2)//mouvement balle avec rebond gauche droite
{
 if (sens_w==Gauche)
{
    dep3->x-=2;
}
else if (sens_w==Droite)
{
 dep3->x+=2;
}
if (dep3->x>SCREEN_WIDTH-20)
{
    sens_w=Gauche;
}
if (dep3->x<0)
{
    sens_w=Droite;
}
return sens_w;
}

void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}









